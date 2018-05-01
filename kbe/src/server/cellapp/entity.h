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

#ifndef KBE_ENTITY_H
#define KBE_ENTITY_H
	
#include "profile.h"
#include "common/timer.h"
#include "common/common.h"
#include "common/smartpointer.h"
#include "helper/debug_helper.h"
#include "entitydef/entity_call.h"
#include "entitydef/entity_component.h"
#include "pyscript/math.h"
#include "pyscript/scriptobject.h"
#include "entitydef/datatypes.h"	
#include "entitydef/entitydef.h"	
#include "entitydef/scriptdef_module.h"
#include "entitydef/entity_macro.h"	
#include "server/script_timers.h"	
	
namespace KBEngine{

class Chunk;
class Entity;
class EntityCall;
class EntityComponent;
class Cellapp;
class Witness;
class AllClients;
class CoordinateSystem;
class EntityCoordinateNode;
class Controller;
class Controllers;
class Space;
class VolatileInfo;

namespace Network
{
class Channel;
class Bundle;
}

typedef SmartPointer<Entity> EntityPtr;
typedef std::vector<EntityPtr> SPACE_ENTITIES;

class Entity : public script::ScriptObject
{
	/** Subclassing Fill some py operations into derived classes */
	BASE_SCRIPT_HREADER(Entity, ScriptObject)	
	ENTITY_HEADER(Entity)

public:
	Entity(ENTITY_ID id, const ScriptDefModule* pScriptModule);
	~Entity();
	
	/** 
		This entity is destroyed
	*/
	void onDestroy(bool callScript);
	
	/**
		Destroy space
	*/
	DECLARE_PY_MOTHOD_ARG0(pyDestroySpace);
	void destroySpace(void);

	/** 
		Trigger when the current entity's space is about to be destroyed
	*/
	void onSpaceGone();
	
	/** 
		Determine if it is a realEntity
	*/
	INLINE bool isReal(void) const;

	/** 
		Determine if it has a ghost entity
	*/
	INLINE bool hasGhost(void) const;

	/** 
		Determine if it is a realEntity
	*/
	INLINE COMPONENT_ID realCell(void) const;
	INLINE void realCell(COMPONENT_ID cellID);

	/** 
		To determine whether there is ghostentity 
	*/
	INLINE COMPONENT_ID ghostCell(void) const;
	INLINE void ghostCell(COMPONENT_ID cellID);

	/** 
		Defined attribute data was changed
	*/
	void onDefDataChanged(EntityComponent* pEntityComponent, const PropertyDescription* propertyDescription,
			PyObject* pyData);
	
	/** 
		The entity communication channel
	*/
	INLINE void pChannel(Network::Channel* pchannel);
	INLINE Network::Channel* pChannel(void) const ;

public:
	/** 
		entityCall section
	*/
	INLINE EntityCall* baseEntityCall() const;
	DECLARE_PY_GET_METHOD(pyGetBaseEntityCall);
	INLINE void baseEntityCall(EntityCall* entityCall);
	
	INLINE EntityCall* clientEntityCall() const;
	DECLARE_PY_GET_METHOD(pyGetClientEntityCall);
	INLINE void clientEntityCall(EntityCall* entityCall);

	/**
		all_clients
	*/
	INLINE AllClients* allClients() const;
	DECLARE_PY_GET_METHOD(pyGetAllClients);
	INLINE void allClients(AllClients* clients);

	/**
		other_clients
	*/
	INLINE AllClients* otherClients() const;
	DECLARE_PY_GET_METHOD(pyGetOtherClients);
	INLINE void otherClients(AllClients* clients);

	/**
		Script gets controlledBy property
	*/
	INLINE bool isControlledNotSelfClient() const;
	INLINE EntityCall* controlledBy() const;
	INLINE void controlledBy(EntityCall* baseEntityCall);
	DECLARE_PY_GETSET_METHOD(pyGetControlledBy, pySetControlledBy);
	bool setControlledBy(EntityCall* baseEntityCall);
	void sendControlledByStatusMessage(EntityCall* baseEntityCall, int8 isControlled);

	/** 
		The script gets and sets the position of the entity
	*/
	INLINE Position3D& position();
	INLINE void position(const Position3D& pos);
	DECLARE_PY_GETSET_METHOD(pyGetPosition, pySetPosition);

	/** 
		The script gets and sets the direction of the entity
	*/
	INLINE Direction3D& direction();
	INLINE void direction(const Direction3D& dir);
	DECLARE_PY_GETSET_METHOD(pyGetDirection, pySetDirection);

	INLINE void isOnGround(bool v);
	INLINE bool isOnGround() const;
	DECLARE_PY_GET_METHOD(pyGetIsOnGround);

	/** 
		Set the entity direction and position
	*/
	void setPositionAndDirection(const Position3D& pos, 
		const Direction3D& dir);
	
	void onPositionChanged();
	void onDirectionChanged();
	
	void onPyPositionChanged();
	void onPyDirectionChanged();
	
	void updateLastPos();

	bool checkMoveForTopSpeed(const Position3D& position);

	/** Network interface
		Client sets new location
	*/
	void setPosition_XZ_int(Network::Channel* pChannel, int32 x, int32 z);

	/** Network interface
		Client sets new location
	*/
	void setPosition_XYZ_int(Network::Channel* pChannel, int32 x, int32 y, int32 z);

	/** Network interface
		Client sets new location
	*/
	void setPosition_XZ_float(Network::Channel* pChannel, float x, float z);

	/** Network interface
		Client sets new location
	*/
	void setPosition_XYZ_float(Network::Channel* pChannel, float x, float y, float z);

	/** Network interface
		Entity teleport
		@cellAppID: The destination of the app to send to
		@targetEntityID： To be sent to the space of this entity
		@sourceBaseAppID: It may be that the base request on some baseapp requests teleport, if it is 0 it is initiated by cellEntity
	*/
	void teleportFromBaseapp(Network::Channel* pChannel, COMPONENT_ID cellAppID, ENTITY_ID targetEntityID, COMPONENT_ID sourceBaseAppID);

	/**
		cell teleport methods
	*/
	DECLARE_PY_MOTHOD_ARG3(pyTeleport, PyObject_ptr, PyObject_ptr, PyObject_ptr);
	void teleport(PyObject_ptr nearbyMBRef, Position3D& pos, Direction3D& dir);
	void teleportLocal(PyObject_ptr nearbyMBRef, Position3D& pos, Direction3D& dir);
	void teleportRefEntity(Entity* entity, Position3D& pos, Direction3D& dir);
	void teleportRefEntityCall(EntityCall* nearbyMBRef, Position3D& pos, Direction3D& dir);
	void onTeleportRefEntityCall(EntityCall* nearbyMBRef, Position3D& pos, Direction3D& dir);

	/**
		Send callbacks related to success and failure
	*/
	void onTeleport();
	void onTeleportFailure();
	void onTeleportSuccess(PyObject* nearbyEntity, SPACE_ID lastSpaceID);
	void onReqTeleportOtherAck(Network::Channel* pChannel, ENTITY_ID nearbyMBRefID, 
		SPACE_ID destSpaceID, COMPONENT_ID componentID);

	/**
		Enter/leave cell callbacks
	*/
	void onEnteredCell();
	void onEnteringCell();
	void onLeavingCell();
	void onLeftCell();
	
	/**
		Leave/enter space callbacks
	*/
	void onEnterSpace(Space* pSpace);
	void onLeaveSpace(Space* pSpace);

	/** 
		When cellapp unexpectedly terminates, baseapp can recover if it finds a suitable cellapp.
		Will call this method
	*/
	void onRestore();

	/**
		Script debugging view
	*/
	void debugView();
	DECLARE_PY_MOTHOD_ARG0(pyDebugView);

	/** 
		The current entity sets its own View radius
	*/
	int32 setViewRadius(float radius, float hyst);
	float getViewRadius(void) const;
	float getViewHystArea(void) const;
	DECLARE_PY_MOTHOD_ARG2(pySetViewRadius, float, float);
	DECLARE_PY_MOTHOD_ARG0(pyGetViewRadius);
	DECLARE_PY_MOTHOD_ARG0(pyGetViewHystArea);

	/** 
		Whether the current entity is real
	*/
	DECLARE_PY_MOTHOD_ARG0(pyIsReal);
	
	/** 
		Send backup data to baseapp
	*/
	void backupCellData();

	/** 
		Before you save to the database
	*/
	void onWriteToDB();

	/** 
		The script gets and sets the position of the entity
	*/
	INLINE int8 layer() const;
	DECLARE_PY_GETSET_METHOD(pyGetLayer, pySetLayer);

	/** 
		Entity move navigation
	*/
	bool canNavigate();
	uint32 navigate(const Position3D& destination, float velocity, float distance,
					float maxMoveDistance, float maxSearchDistance,
					bool faceMovement, int8 layer, PyObject* userData);
	bool navigatePathPoints(std::vector<Position3D>& outPaths, const Position3D& destination, float maxSearchDistance, int8 layer);

	DECLARE_PY_MOTHOD_ARG0(pycanNavigate);
	DECLARE_PY_MOTHOD_ARG3(pyNavigatePathPoints, PyObject_ptr, float, int8);
	DECLARE_PY_MOTHOD_ARG8(pyNavigate, PyObject_ptr, float, float, float, float, int8, int8, PyObject_ptr);

	/** 
		Entity gets a random point
	*/
	bool getRandomPoints(std::vector<Position3D>& outPoints, const Position3D& centerPos, float maxRadius, uint32 maxPoints, int8 layer);
	DECLARE_PY_MOTHOD_ARG4(pyGetRandomPoints, PyObject_ptr, float, uint32, int8);

	/** 
		Entity moves to a point
	*/
	uint32 moveToPoint(const Position3D& destination, float velocity, float distance,
			PyObject* userData, bool faceMovement, bool moveVertically);
	
	DECLARE_PY_MOTHOD_ARG6(pyMoveToPoint, PyObject_ptr, float, float, PyObject_ptr, int32, int32);

	/** 
		Move this entity to another
	*/
	uint32 moveToEntity(ENTITY_ID targetID, float velocity, float distance,
			PyObject* userData, bool faceMovement, bool moveVertically);
	
	DECLARE_PY_MOTHOD_ARG6(pyMoveToEntity, int32, float, float, PyObject_ptr, int32, int32);

	/**
		Entity move acceleration
	*/
	float accelerate(const char* type, float acceleration);
	DECLARE_PY_MOTHOD_ARG2(pyAccelerate, const_charptr, float);

	/** 
		The script gets and sets the entity's maximum xz movement speed
	*/
	float topSpeed() const{ return topSpeed_; }
	INLINE void topSpeed(float speed);
	DECLARE_PY_GETSET_METHOD(pyGetTopSpeed, pySetTopSpeed);
	
	/** 
		The script gets and sets the Y axis top-speed of the entity
	*/
	INLINE float topSpeedY() const;
	INLINE void topSpeedY(float speed);
	DECLARE_PY_GETSET_METHOD(pyGetTopSpeedY, pySetTopSpeedY);
	
	/** 
		The script requests to get a certain type of entities within a range
	*/
	static PyObject* __py_pyEntitiesInRange(PyObject* self, PyObject* args);

	/** 
		Script requests to get entities in View-range
	*/
	DECLARE_PY_MOTHOD_ARG0(pyEntitiesInView);

	/**
		Set whether to do automatic backup
	*/
	INLINE int8 shouldAutoBackup() const;
	INLINE void shouldAutoBackup(int8 v);
	DECLARE_PY_GETSET_METHOD(pyGetShouldAutoBackup, pySetShouldAutoBackup);

	/** Network interface
		Method for remotely calling this entity
	*/
	void onRemoteMethodCall(Network::Channel* pChannel, MemoryStream& s);
	void onRemoteCallMethodFromClient(Network::Channel* pChannel, ENTITY_ID srcEntityID, MemoryStream& s);
	void onRemoteMethodCall_(PropertyDescription* pComponentPropertyDescription, 
		MethodDescription* pMethodDescription, ENTITY_ID srcEntityID, MemoryStream& s);

	/**
		Observer
	*/
	INLINE Witness* pWitness() const;
	INLINE void pWitness(Witness* w);

	/** 
		Whether the observer is monitored by any proxy. If the entity does not have a client, this value is valid
	*/
	INLINE bool isWitnessed(void) const;
	DECLARE_PY_GET_METHOD(pyIsWitnessed);

	/** 
		Whether the entity is has a witness (is an observer of other entities)
	*/
	INLINE bool hasWitness(void) const;
	DECLARE_PY_GET_METHOD(pyHasWitness);

	/** 
		Add a Witness who witnessed this entity
	*/
	void addWitnessed(Entity* entity);

	/** 
		Remove a Witness that was observing this entity
	*/
	void delWitnessed(Entity* entity);
	void onDelWitnessed();

	/**
		 Whether the specified entity is one of the Witnesses observing this entity
	*/
	bool entityInWitnessed(ENTITY_ID entityID);

	INLINE const std::list<ENTITY_ID>&	witnesses();
	INLINE size_t witnessesSize() const;

	/** Network interface
		Entity binds a Witness (client)
	*/
	void setWitness(Witness* pWitness);
	void onGetWitnessFromBase(Network::Channel* pChannel);
	void onGetWitness(bool fromBase = false);

	/** Network interface
		Entity lost an Witness (client)
	*/
	void onLoseWitness(Network::Channel* pChannel);

	/** 
		Client Updates data
	*/
	void onUpdateDataFromClient(KBEngine::MemoryStream& s);

	/** 
		Add a range Trigger  
	*/
	uint32 addProximity(float range_xz, float range_y, int32 userarg);
	DECLARE_PY_MOTHOD_ARG3(pyAddProximity, float, float, int32);

	/** 
		Methods for calling client entities 
	*/
	DECLARE_PY_MOTHOD_ARG1(pyClientEntity, ENTITY_ID);

	/** 
		Restore all range triggers 
		This is what happens when you teleport.
	*/
	void restoreProximitys();

	/** 
		Delete a controller 
	*/
	void cancelController(uint32 id);
	static PyObject* __py_pyCancelController(PyObject* self, PyObject* args);

	/** 
		An entity enters the range of one of this entity's proximity triggers
	*/
	void onEnterTrap(Entity* entity, float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		An entity leaves the range of one of this entity's proximity triggers
	*/
	void onLeaveTrap(Entity* entity, float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		When the entity jumps to a new space, leaving the proximity trigger event will callback this function
	*/
	void onLeaveTrapID(ENTITY_ID entityID, 
							float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		An entity enters the View range
	*/
	void onEnteredView(Entity* entity);

	/** 
		Stop any move
	*/
	bool stopMove();

	/** 
		One frame of the move of the entity is complete
	*/
	void onMove(uint32 controllerId, int layer, const Position3D& oldPos, PyObject* userarg);

	/** 
		Entity's move is complete
	*/
	void onMoveOver(uint32 controllerId, int layer, const Position3D& oldPos, PyObject* userarg);

	/** 
		Entity move failed
	*/
	void onMoveFailure(uint32 controllerId, PyObject* userarg);

	/**
		Entity turning towards
	*/
	uint32 addYawRotator(float yaw, float velocity,
		PyObject* userData);

	DECLARE_PY_MOTHOD_ARG3(pyAddYawRotator, float, float, PyObject_ptr);
	
	/**
		Entity turn complete
	*/
	void onTurn(uint32 controllerId, PyObject* userarg);
	
	/**
		Get its position in the entities of space
	*/
	INLINE SPACE_ENTITIES::size_type spaceEntityIdx() const;
	INLINE void spaceEntityIdx(SPACE_ENTITIES::size_type idx);

	/**
		Get the entity coordinate node
	*/
	INLINE EntityCoordinateNode* pEntityCoordinateNode() const;
	INLINE void pEntityCoordinateNode(EntityCoordinateNode* pNode);

	/**
		Install/uninstall coordinate node
	*/
	void installCoordinateNodes(CoordinateSystem* pCoordinateSystem);
	void uninstallCoordinateNodes(CoordinateSystem* pCoordinateSystem);
	void onCoordinateNodesDestroy(EntityCoordinateNode* pEntityCoordinateNode);

	/**
		Get the last time that the pos/dir was changed
	*/
	INLINE GAME_TIME posChangedTime() const;
	INLINE GAME_TIME dirChangedTime() const;

	/** 
		Real entity requests to update properties to ghost
	*/
	void onUpdateGhostPropertys(KBEngine::MemoryStream& s);
	
	/** 
		Ghost requests to call def method real
	*/
	void onRemoteRealMethodCall(KBEngine::MemoryStream& s);

	/** 
		Real entity requests to update volatile properties to ghost
	*/
	void onUpdateGhostVolatileData(KBEngine::MemoryStream& s);

	/** 
		Changes entity to a ghost entity, must be real
	*/
	void changeToGhost(COMPONENT_ID realCell, KBEngine::MemoryStream& s);

	/** 
		Change ghost entity to a real entity, must be ghost
	*/
	void changeToReal(COMPONENT_ID ghostCell, KBEngine::MemoryStream& s);

	void addToStream(KBEngine::MemoryStream& s);
	void createFromStream(KBEngine::MemoryStream& s);

	void addTimersToStream(KBEngine::MemoryStream& s);
	void createTimersFromStream(KBEngine::MemoryStream& s);

	void addControllersToStream(KBEngine::MemoryStream& s);
	void createControllersFromStream(KBEngine::MemoryStream& s);

	void addWitnessToStream(KBEngine::MemoryStream& s);
	void createWitnessFromStream(KBEngine::MemoryStream& s);

	void addMovementHandlerToStream(KBEngine::MemoryStream& s);
	void createMovementHandlerFromStream(KBEngine::MemoryStream& s);
	
	void addEventsToStream(KBEngine::MemoryStream& s);
	void createEventsFromStream(KBEngine::MemoryStream& s);

	/** 
		Get Entity Controller Manager
	*/
	INLINE Controllers*	pControllers() const;

	/** 
		Set the entity persistence data is dirty, if data is dirty it will be automatically archived
	*/
	INLINE void setDirty(bool dirty = true);
	INLINE bool isDirty() const;
	
	/**
		VolatileInfo section
	*/
	INLINE VolatileInfo* pCustomVolatileinfo(void);
	DECLARE_PY_GETSET_METHOD(pyGetVolatileinfo, pySetVolatileinfo);

	/**
		Call the entity's callback function, which may be cached
	*/
	bool bufferOrExeCallback(const char * funcName, PyObject * funcArgs, bool notFoundIsOK = true);
	static void bufferCallback(bool enable);

private:
	/** 
		Send teleport results to base section
	*/
	void _sendBaseTeleportResult(ENTITY_ID sourceEntityID, COMPONENT_ID sourceBaseAppID, 
		SPACE_ID spaceID, SPACE_ID lastSpaceID, bool fromCellTeleport);

private:
	struct BufferedScriptCall
	{
		EntityPtr		entityPtr;
		PyObject *		pyCallable;
		PyObject *		pyFuncArgs; // Can be NULL, NULL means no parameters
		const char*		funcName;
	};

	typedef std::list<BufferedScriptCall*>					BufferedScriptCallArray;
	static BufferedScriptCallArray							_scriptCallbacksBuffer;
	static int32											_scriptCallbacksBufferNum;
	static int32											_scriptCallbacksBufferCount;

protected:
	// The entityCall of the entity part of this entity
	EntityCall*												clientEntityCall_;

	// The entityCall of the baseapp part of this entity
	EntityCall*												baseEntityCall_;

	/** The entity's coordinates and orientation are controlled by the current client
	    NULL means no client is in control (ie system control),
	    Otherwise, it points to the baseEntityCall_ of the proxy object that controls the entity.
		If the player himself controls, Entity.controlledBy = self.base
	*/
	EntityCall *											controlledBy_;

	// If an entity is ghost, then the entity will have its real entity cell id
	COMPONENT_ID											realCell_;

	// If an entity is real then the entity may have a ghost cell id
	COMPONENT_ID											ghostCell_;	

	// The current position of the entity
	Position3D												lastpos_;
	Position3D												position_;
	script::ScriptVector3*									pPyPosition_;

	// The current direction of the entity
	Direction3D												direction_;	
	script::ScriptVector3*									pPyDirection_;

	// Last time the entity pos/dir has changed
	// This property can be used for such as: Deciding whether to synchronize the entity at a chosen interval
	GAME_TIME												posChangedTime_;
	GAME_TIME												dirChangedTime_;

	// Whether on the ground
	bool													isOnGround_;

	// Entity x, z axis maximum movement speed
	float													topSpeed_;

	// Entity y axis maximum speed
	float													topSpeedY_;

	// Its own position in the entities list of its space
	SPACE_ENTITIES::size_type								spaceEntityIdx_;

	// Is it observed by any Witnesses
	std::list<ENTITY_ID>									witnesses_;
	size_t													witnesses_count_;

	// Witness object
	Witness*												pWitness_;

	AllClients*												allClients_;
	AllClients*												otherClients_;

	// Entity node
	EntityCoordinateNode*									pEntityCoordinateNode_;	

	// Controller Manager
	Controllers*											pControllers_;
	KBEShared_ptr<Controller>								pMoveController_;
	KBEShared_ptr<Controller>								pTurnController_;
	
	script::ScriptVector3::PYVector3ChangedCallback			pyPositionChangedCallback_;
	script::ScriptVector3::PYVector3ChangedCallback			pyDirectionChangedCallback_;
	
	// The entity layer can be expressed arbitrarily.
	// Tile-based games can be represented by layers such as land, sea and air. 3D can also have various layers.
	// You can search by layer in script
	int8													layer_;
	
	// Whether the data that needs to be persisted. If is dirty needs to be re-persisted, otherwise it already is persistent
	bool													isDirty_;

	// If the user has set up Volatileinfo, Volatileinfo is created here, otherwise it is NULL.
	// Use Volatileinfo of ScriptDefModule
	VolatileInfo*											pCustomVolatileinfo_;
};

}

#ifdef CODE_INLINE
#include "entity.inl"
#endif
#endif // KBE_ENTITY_H
