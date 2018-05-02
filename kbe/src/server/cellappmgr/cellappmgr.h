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


#ifndef KBE_CELLAPPMGR_H
#define KBE_CELLAPPMGR_H
	
#include "cellapp.h"
#include "space_viewer.h"
#include "server/kbemain.h"
#include "server/serverapp.h"
#include "server/idallocate.h"
#include "server/serverconfig.h"
#include "server/forward_messagebuffer.h"
#include "common/timer.h"
#include "network/endpoint.h"

namespace KBEngine{


class Cellappmgr :	public ServerApp, 
					public Singleton<Cellappmgr>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK = TIMEOUT_SERVERAPP_MAX + 1
	};
	
	Cellappmgr(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Cellappmgr();
	
	bool run();
	
	virtual void onChannelDeregister(Network::Channel * pChannel);

	void handleTimeout(TimerHandle handle, void * arg);
	void handleGameTick();

	/* Initialize related interfaces */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();

	/** Find the most idle Cellapp */
	COMPONENT_ID findFreeCellapp(void);
	void updateBestCellapp();

	/** Network interface
		baseEntity request created in a new space
	*/
	void reqCreateCellEntityInNewSpace(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		baseEntity request created in a new space
	*/
	void reqRestoreSpaceInCell(Network::Channel* pChannel, MemoryStream& s);
	
	/** Network interface
		A message is forwarded by another app that wants to forward a message to an app through this one.
	*/
	void forwardMessage(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Update cellapp
	*/
	void updateCellapp(Network::Channel* pChannel, COMPONENT_ID componentID, ENTITY_ID numEntities, float load, uint32 flags);

	/** Network interface
		Cellapp synchronizes its initialization information
		startGlobalOrder: The global startup sequence includes a variety of different components
		startGroupOrder: The start order in the group, such as the first few start in all baseapps.
	*/
	void onCellappInitProgress(Network::Channel* pChannel, COMPONENT_ID cid, float progress, 
		COMPONENT_ORDER componentGlobalOrder, COMPONENT_ORDER componentGroupOrder);

	bool componentsReady();
	bool componentReady(COMPONENT_ID cid);

	void removeCellapp(COMPONENT_ID cid);
	Cellapp& getCellapp(COMPONENT_ID cid);
	std::map< COMPONENT_ID, Cellapp >& cellapps();

	uint32 numLoadBalancingApp();

	/* With groupOrderID as the sorting benchmark
	   Add a cellapp component id to the list of cellapp_cids_
	*/
	void addCellappComponentID(COMPONENT_ID cid);

	/** Network interface
		Query all relevant process load information
	*/
	void queryAppsLoads(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
	    Query all relevant process space information
	*/
	void querySpaces(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
	    Update the relevant process space information.
	    Note that this spaceData is not the spaceData described in the API documentation.
	    Refers to space information
	*/
	void updateSpaceData(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Tool requests a change in the space viewer (including add and delete functions)
		If it is a viewer that requests an update and the address does not exist on the server,
		it is automatically created. If it is deleted, the delete request is explicitly given.
	*/
	void setSpaceViewer(Network::Channel* pChannel, MemoryStream& s);

protected:
	TimerHandle							gameTimer_;
	ForwardAnywhere_MessageBuffer		forward_anywhere_cellapp_messagebuffer_;
	ForwardComponent_MessageBuffer		forward_cellapp_messagebuffer_;

	COMPONENT_ID						bestCellappID_;

	std::map< COMPONENT_ID, Cellapp >	cellapps_;
	std::vector<COMPONENT_ID>			cellapp_cids_;

	// View space through tools
	SpaceViewers						spaceViewers_;
};

} 

#endif // KBE_CELLAPPMGR_H
