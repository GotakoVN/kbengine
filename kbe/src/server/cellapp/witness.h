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

#ifndef KBE_WITNESS_H
#define KBE_WITNESS_H

// common include
#include "updatable.h"
#include "entityref.h"
#include "helper/debug_helper.h"
#include "common/common.h"
#include "common/objectpool.h"
#include "math/math.h"

// #define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32	
#else
// linux include
#endif

namespace KBEngine{

namespace Network
{
	class Bundle;
	class MessageHandler;
}

class Entity;
class MemoryStream;
class ViewTrigger;
class Space;

/** Observer Information Structure */
struct WitnessInfo
{
	WitnessInfo(const int8& lv, Entity* e, const float& r):
	detailLevel(lv),
	entity(e),
	range(r)
	{
		for(int i=0; i<3; ++i)
			if(lv == i)
				detailLevelLog[i] = true;
			else
				detailLevelLog[i] = false;
	}
	
	int8 detailLevel;							// Current level of detail
	Entity* entity;								// Witnessed entity
	float range;								// Current distance from entity
	bool detailLevelLog[3];						// Indicates which level of detail this entity has entered with, for attribute broadcast optimization
												// When entering a LOD, all that LOD's properties will be updated to the witness,
												//  otherwise, it will only update properties changed recently.
	std::vector<uint32> changeDefDataLogs[3];	// After the entity leaves a level of detail (without leaving the witness),
												//  level of detail changes are recorded here.
};

/**
	This class is used to monitor entity data of interest, such as: view, property update, calling entity methods.
	And transmits it to the observer.
*/
class Witness : public PoolObject, public Updatable
{
public:
	typedef std::list<EntityRef*> VIEW_ENTITIES;
	typedef std::map<ENTITY_ID, EntityRef*> VIEW_ENTITIES_MAP;

	Witness();
	~Witness();
	
	virtual uint8 updatePriority() const {
		return 1;
	}

	void addToStream(KBEngine::MemoryStream& s);
	void createFromStream(KBEngine::MemoryStream& s);

	typedef KBEShared_ptr< SmartPoolObject< Witness > > SmartPoolObjectPtr;
	static SmartPoolObjectPtr createSmartPoolObj();

	static ObjectPool<Witness>& ObjPool();
	static Witness* createPoolObject();
	static void reclaimPoolObject(Witness* obj);
	static void destroyObjPool();
	void onReclaimObject();

	virtual size_t getPoolObjectBytes()
	{
		size_t bytes = sizeof(pEntity_)
		 + sizeof(viewRadius_) + sizeof(viewLagArea_)
		 + sizeof(pViewTrigger_) + sizeof(pViewLagAreaTrigger_) + sizeof(clientViewSize_)
		 + sizeof(lastBasePos_) + (sizeof(EntityRef*) * viewEntities_map_.size());

		return bytes;
	}

	INLINE void pEntity(Entity* pEntity);
	INLINE Entity* pEntity();

	void attach(Entity* pEntity);
	void detach(Entity* pEntity);
	void clear(Entity* pEntity);
	void onAttach(Entity* pEntity);

	void setViewRadius(float radius, float hyst = 5.0f);
	
	INLINE float viewRadius() const;
	INLINE float viewLagArea() const;

	typedef std::vector<Network::Bundle*> Bundles;
	bool pushBundle(Network::Bundle* pBundle);

	/**
		Base position, if there is a vehicle base position may be vehicle etc.
	*/
	INLINE const Position3D& basePos();

	/**
		Base orientation, if there is a vehicle base facing may be vehicle etc
	*/
	INLINE const Direction3D& baseDir();

	bool update();
	
	void onEnterSpace(Space* pSpace);
	void onLeaveSpace(Space* pSpace);

	void onEnterView(ViewTrigger* pViewTrigger, Entity* pEntity);
	void onLeaveView(ViewTrigger* pViewTrigger, Entity* pEntity);
	void _onLeaveView(EntityRef* pEntityRef);

	/**
		Get flags for syncing Volatile data for the entity
	*/
	uint32 getEntityVolatileDataUpdateFlags(Entity* otherEntity);
	

	const Network::MessageHandler& getViewEntityMessageHandler(const Network::MessageHandler& normalMsgHandler, 
											   const Network::MessageHandler& optimizedMsgHandler, ENTITY_ID entityID, int& ialiasID);

	bool entityID2AliasID(ENTITY_ID id, uint8& aliasID);

	/**
		What protocol to use to update the client
	*/
	void addUpdateToStream(Network::Bundle* pForwardBundle, uint32 flags, EntityRef* pEntityRef);

	/**
		Add base location to update package
	*/
	void addBaseDataToStream(Network::Bundle* pSendBundle);

	/**
		Push a message to the witness client
	*/
	bool sendToClient(const Network::MessageHandler& msgHandler, Network::Bundle* pBundle);
	Network::Channel* pChannel();
		
	INLINE VIEW_ENTITIES_MAP& viewEntitiesMap();
	INLINE VIEW_ENTITIES& viewEntities();

	/** Get a reference to viewentity */
	INLINE EntityRef* getViewEntityRef(ENTITY_ID entityID);

	/** Whether the entityID is in the view */
	INLINE bool entityInView(ENTITY_ID entityID);

	INLINE ViewTrigger* pViewTrigger();
	INLINE ViewTrigger* pViewLagAreaTrigger();
	
	void installViewTrigger();
	void uninstallViewTrigger();

	/**
		Reset the entities in View-range to bring them back to their original unsynchronized state
	*/
	void resetViewEntities();

private:
	/**
		If the number of entities in the view is less than 256, only the index position is sent
	*/
	INLINE void _addViewEntityIDToBundle(Network::Bundle* pBundle, EntityRef* pEntityRef);
	
	/**
		The update of the entityRef's aliasID is required when the view is changed when the update is executed
	*/
	void updateEntitiesAliasID();
		
private:
	Entity*									pEntity_;

	// The current entity's view radius
	float									viewRadius_;
	// A lag range for the current entityView
	float									viewLagArea_;

	ViewTrigger*							pViewTrigger_;
	ViewTrigger*							pViewLagAreaTrigger_;

	VIEW_ENTITIES							viewEntities_;
	VIEW_ENTITIES_MAP						viewEntities_map_;

	Position3D								lastBasePos_;
	Direction3D								lastBaseDir_;

	uint16									clientViewSize_;
};

}

#ifdef CODE_INLINE
#include "witness.inl"
#endif
#endif
