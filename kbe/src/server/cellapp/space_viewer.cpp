/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2018 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "cellapp.h"
#include "space.h"	
#include "entity.h"
#include "space_viewer.h"
#include "network/network_interface.h"
#include "network/event_dispatcher.h"
#include "network/address.h"
#include "network/network_stats.h"
#include "network/bundle.h"
#include "network/message_handler.h"
#include "common/memorystream.h"
#include "helper/console_helper.h"
#include "helper/profile.h"
#include "server/serverconfig.h"

namespace KBEngine { 

//-------------------------------------------------------------------------------------
SpaceViewers::SpaceViewers():
reportLimitTimerHandle_(),
spaceViews_()
{
}

//-------------------------------------------------------------------------------------
SpaceViewers::~SpaceViewers()
{
	finalise();
}

//-------------------------------------------------------------------------------------
bool SpaceViewers::addTimer()
{
	if (!reportLimitTimerHandle_.isSet())
	{
		reportLimitTimerHandle_ = Cellapp::getSingleton().networkInterface().dispatcher().addTimer(
			1000000 / 10, this);

		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------
void SpaceViewers::finalise()
{
	clear();
	reportLimitTimerHandle_.cancel();
}

//-------------------------------------------------------------------------------------
void SpaceViewers::updateSpaceViewer(const Network::Address& addr, SPACE_ID spaceID, CELL_ID cellID, bool del)
{
	if (del)
	{
		spaceViews_.erase(addr);
		return;
	}

	SpaceViewer& viewer = spaceViews_[addr];
	viewer.updateViewer(addr, spaceID, cellID);

	addTimer();
}

//-------------------------------------------------------------------------------------
void SpaceViewers::handleTimeout(TimerHandle handle, void * arg)
{
	if (spaceViews_.size() == 0)
	{
		reportLimitTimerHandle_.cancel();
		return;
	}

	std::map< Network::Address, SpaceViewer>::iterator iter = spaceViews_.begin();
	for (; iter != spaceViews_.end(); )
	{
		// Erase the viewer if it is not found
		Network::Channel* pChannel = Cellapp::getSingleton().networkInterface().findChannel(iter->second.addr());
		if (pChannel == NULL)
		{
			spaceViews_.erase(iter++);
		}
		else
		{
			iter->second.timeout();
			++iter;
		}
	}
}

//-------------------------------------------------------------------------------------
SpaceViewer::SpaceViewer():
addr_(),
spaceID_(0),
cellID_(0),
viewedEntities(),
updateType_(0),
lastUpdateVersion_(0)
{
}

//-------------------------------------------------------------------------------------
SpaceViewer::~SpaceViewer()
{
}

//-------------------------------------------------------------------------------------
void SpaceViewer::resetViewer()
{
	viewedEntities.clear();
	lastUpdateVersion_ = 0;
}

//-------------------------------------------------------------------------------------
void SpaceViewer::updateViewer(const Network::Address& addr, SPACE_ID spaceID, CELL_ID cellID)
{
	addr_ = addr;

	bool chagnedSpace = spaceID_ != spaceID;

	if (chagnedSpace)
	{
		onChangedSpaceOrCell();
		spaceID_ = spaceID;
	}

	if (cellID_ != cellID)
	{
		if (!chagnedSpace)
			onChangedSpaceOrCell();

		cellID_ = cellID;
	}
}

//-------------------------------------------------------------------------------------
void SpaceViewer::onChangedSpaceOrCell()
{
	resetViewer();
}

//-------------------------------------------------------------------------------------
void SpaceViewer::timeout()
{
	switch (updateType_)
	{
	case 0: // Initialization
		initClient();
		break;
	default: // Update entity
		updateClient();
	};
}

//-------------------------------------------------------------------------------------
void SpaceViewer::sendStream(MemoryStream* s, int type)
{
	Network::Channel* pChannel = Cellapp::getSingleton().networkInterface().findChannel(addr_);
	if(pChannel == NULL)
	{
		WARNING_MSG(fmt::format("SpaceViewer::sendStream: not found addr({})\n",
			addr_.c_str()));

		return;
	}

	Network::Bundle* pBundle = Network::Bundle::createPoolObject();

	ConsoleInterface::ConsoleQuerySpacesHandler msgHandler;
	(*pBundle).newMessage(msgHandler);

	(*pBundle) << g_componentType;
	(*pBundle) << g_componentID;
	(*pBundle) << type;
	(*pBundle).append(s);
	pChannel->send(pBundle);
}

//-------------------------------------------------------------------------------------
void SpaceViewer::initClient()
{
	MemoryStream s;

	// First issued script ID corresponding script module name, Easy to reduce the amount
	//  of subsequent entity synchronization, the entity only sync id past
	const EntityDef::SCRIPT_MODULES& scriptModules = EntityDef::getScriptModules();
	s << (uint32)scriptModules.size();

	EntityDef::SCRIPT_MODULES::const_iterator moduleIter = scriptModules.begin();
	for (; moduleIter != scriptModules.end(); ++moduleIter)
	{
		s << moduleIter->get()->getUType();
		s << moduleIter->get()->getName();
	}

	sendStream(&s, updateType_);

	// Change to update entity
	updateType_ = 1;

	lastUpdateVersion_ = 0;
}

//-------------------------------------------------------------------------------------
void SpaceViewer::updateClient()
{
	if (spaceID_ == 0)
		return;

	Space* space = Spaces::findSpace(spaceID_);
	if (space == NULL || !space->isGood())
	{
		return;
	}

	// Up to 500 entities per update
	const int MAX_UPDATE_COUNT = 100;
	int updateCount = 0;

	// Gets the difference between this and the last result, and updates the difference into the stream to the client
	// Differences include new entities, and changes in the positions of existing entities
	MemoryStream s;

	Entities<Entity>* pEntities = Cellapp::getSingleton().pEntities();
	Entities<Entity>::ENTITYS_MAP& entitiesMap = pEntities->getEntities();

	// Check the monitored entities first, with a lower priority version number
	if (updateCount < MAX_UPDATE_COUNT)
	{
		std::map< ENTITY_ID, ViewEntity >::iterator viewerIter = viewedEntities.begin();
		for (; viewerIter != viewedEntities.end(); )
		{
			if (updateCount >= MAX_UPDATE_COUNT)
				break;

			ViewEntity& viewEntity = viewerIter->second;
			if (viewEntity.updateVersion > lastUpdateVersion_)
			{
				++viewerIter;
				continue;
			}

			Entities<Entity>::ENTITYS_MAP::iterator iter = entitiesMap.find(viewerIter->first);

			// No entity found, indicating that it has been destroyed or gone to another process
			// If it is in another process, other processes will update it to the client
			if (iter == entitiesMap.end())
			{
				s << viewerIter->first;
				s << false; // True for update, false for destroy

				// Remove it from viewedEntities
				viewedEntities.erase(viewerIter++);
			}
			else
			{
				Entity* pEntity = static_cast<Entity*>(iter->second.get());
				if (pEntity->spaceID() != spaceID_)
				{
					// Remove it from viewedEntities
					viewedEntities.erase(viewerIter++);
					continue;
				}

				/*
				if (pEntity->cellID() != cellID_)
				{
					// Remove it from viewedEntities
					viewedEntities.erase(viewerIter++);
					continue;
				}
				*/

				// There are new entities or already observed entities to check for position changes
				// If there is no change, pass
				if ((viewEntity.position - pEntity->position()).length() <= 0.0004f &&
					(viewEntity.direction.dir - pEntity->direction().dir).length() <= 0.0004f)
				{
					++viewerIter;
					continue;
				}

				viewEntity.entityID = pEntity->id();
				viewEntity.position = pEntity->position();
				viewEntity.direction = pEntity->direction();
				++viewEntity.updateVersion;

				s << viewEntity.entityID;
				s << true; // True for update, false for destroy
				s << pEntity->pScriptModule()->getUType();
				s << viewEntity.position.x << viewEntity.position.y << viewEntity.position.z;
				s << viewEntity.direction.roll() << viewEntity.direction.pitch() << viewEntity.direction.yaw();

				++updateCount;
				++viewerIter;
			}
		}
	}

	// Check for new entities
	if (updateCount < MAX_UPDATE_COUNT)
	{
		Entities<Entity>::ENTITYS_MAP::iterator iter = entitiesMap.begin();

		for (; iter != entitiesMap.end(); ++iter)
		{
			if (updateCount >= MAX_UPDATE_COUNT)
				break;

			Entity* pEntity = static_cast<Entity*>(iter->second.get());
			if (pEntity->spaceID() != spaceID_)
				continue;

			/*
			if (pEntity->cellID() != cellID_)
				continue;
			*/

			std::map< ENTITY_ID, ViewEntity >::iterator findIter = viewedEntities.find(pEntity->id());
			ViewEntity& viewEntity = viewedEntities[pEntity->id()];

			if (findIter != viewedEntities.end())
				continue;

			viewEntity.entityID = pEntity->id();
			viewEntity.position = pEntity->position();
			viewEntity.direction = pEntity->direction();
			viewEntity.updateVersion = lastUpdateVersion_ + 1;

			++updateCount;

			s << viewEntity.entityID;
			s << true; // True for update, false for destroy
			s << pEntity->pScriptModule()->getUType();
			s << viewEntity.position.x << viewEntity.position.y << viewEntity.position.z;
			s << viewEntity.direction.roll() << viewEntity.direction.pitch() << viewEntity.direction.yaw();
		}
	}

	sendStream(&s, updateType_);

	// If all updates are completed, change the version number
	if (updateCount < MAX_UPDATE_COUNT)
		++lastUpdateVersion_;
}

//-------------------------------------------------------------------------------------

}
