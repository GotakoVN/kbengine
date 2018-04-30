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

#ifndef KBE_CELLAPP_H
#define KBE_CELLAPP_H

#include "entity.h"
#include "spaces.h"
#include "cells.h"
#include "space_viewer.h"
#include "updatables.h"
#include "ghost_manager.h"
#include "witnessed_timeout_handler.h"
#include "server/entity_app.h"
#include "server/forward_messagebuffer.h"
	
namespace KBEngine{

class TelnetServer;

class Cellapp:	public EntityApp<Entity>, 
				public Singleton<Cellapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_LOADING_TICK = TIMEOUT_ENTITYAPP_MAX + 1
	};
	
	Cellapp(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Cellapp();

	virtual bool installPyModules();
	virtual void onInstallPyModules();
	virtual bool uninstallPyModules();
	
	bool run();
	
	virtual bool initializeWatcher();

	/**  
		Related processing interface
	*/
	virtual void handleTimeout(TimerHandle handle, void * arg);
	virtual void handleGameTick();

	/**  
		Initialize related interfaces
	*/
	bool initializeBegin();
	bool initializeEnd();
	void finalise();

	virtual bool canShutdown();
	virtual void onShutdown(bool first);

	void destroyObjPool();

	float _getLoad() const { return getLoad(); }
	virtual void onUpdateLoad();

	/**  Network interface
		Dbmgr tells the address of other baseapp or cellapp that has been started
		Current app needs to actively establish a connection with them
	*/
	virtual void onGetEntityAppFromDbmgr(Network::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							COMPONENT_TYPE componentType, COMPONENT_ID componentID, COMPONENT_ORDER globalorderID, COMPONENT_ORDER grouporderID,
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport, std::string& extaddrEx);

	/**  
		Create an entity
	*/
	static PyObject* __py_createEntity(PyObject* self, PyObject* args);

	/** 
		Dbmgr request to execute a database command
	*/
	static PyObject* __py_executeRawDatabaseCommand(PyObject* self, PyObject* args);
	void executeRawDatabaseCommand(const char* datas, uint32 size, PyObject* pycallback, ENTITY_ID eid, const std::string& dbInterfaceName);
	void onExecuteRawDatabaseCommandCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Dbmgr Send initial information
		startID: Initial allocation of ENTITY_ID segment start position
		endID: Initial allocation of ENTITY_ID segment end position
		startGlobalOrder: The global startup sequence includes a variety of different components
		startGroupOrder: The start order within the group, such as the first few start in all baseapps.
		machineGroupOrder: The actual group order in the machine, which provides the 
							bottom layer to use when determining if it is the first cellapp at some point
	*/
	void onDbmgrInitCompleted(Network::Channel* pChannel, GAME_TIME gametime, 
		ENTITY_ID startID, ENTITY_ID endID, COMPONENT_ORDER startGlobalOrder, COMPONENT_ORDER startGroupOrder, 
		const std::string& digest);

	/** Network interface
		Dbmgr broadcast global data changes
	*/
	void onBroadcastCellAppDataChanged(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		baseEntity request created in a new space
	*/
	void onCreateCellEntityInNewSpaceFromBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		baseEntity request created in a new space
	*/
	void onRestoreSpaceInCellFromBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** Network interface
	Tool request to change the space viewer (including add and delete functions)
	If the viewer is requesting an update and the address does not exist on the server,
	 it is automatically created. If it is deleted, the delete request is explicitly given.
	*/
	void setSpaceViewer(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Other APP requests in this disaster recovery
	*/
	void requestRestore(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Baseapp requests to create an entity on this cellapp
	*/
	void onCreateCellEntityFromBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void _onCreateCellEntityFromBaseapp(std::string& entityType, ENTITY_ID createToEntityID, ENTITY_ID entityID, 
		MemoryStream* pCellData, bool hasClient, bool inRescore, COMPONENT_ID componentID, SPACE_ID spaceID);

	/** Network interface
		Destroy a cellEntity
	*/
	void onDestroyCellEntityFromBaseapp(Network::Channel* pChannel, ENTITY_ID eid);

	/** Network interface
		The entity receives a remote call request initiated by an entitycall on an app
	*/
	void onEntityCall(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** Network interface
		The client accesses a cell method, forwarded by the baseapp
	*/
	void onRemoteCallMethodFromClient(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Client update data
	*/
	void onUpdateDataFromClient(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void onUpdateDataFromClientForControlledEntity(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Real entity requests to update attributes to ghost
	*/
	void onUpdateGhostPropertys(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** Network interface
		Ghost request to call def method real entity
	*/
	void onRemoteRealMethodCall(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Real request to update volatile attributes to ghost
	*/
	void onUpdateGhostVolatileData(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Base request to get celldata
	*/
	void reqBackupEntityCellData(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Base requests to WriteToDB
	*/
	void reqWriteToDBFromBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		The client sends a message directly to the cell entity
	*/
	void forwardEntityMessageToCellappFromClient(Network::Channel* pChannel, MemoryStream& s);

	/**
		Get game time
	*/
	static PyObject* __py_gametime(PyObject* self, PyObject* args);

	/**
		Add and remove a updatable object
	*/
	bool addUpdatable(Updatable* pObject);
	bool removeUpdatable(Updatable* pObject);

	/**
		hook entitycallcall
	*/
	RemoteEntityMethod* createEntityCallCallEntityRemoteMethod(MethodDescription* pMethodDescription, EntityCallAbstract* pEntityCall);

	/** Network interface
		Another app requested to view this app
	*/
	virtual void lookApp(Network::Channel* pChannel);

	/**
		Re-import All Scripts
	*/
	static PyObject* __py_reloadScript(PyObject* self, PyObject* args);
	virtual void reloadScript(bool fullReload);
	virtual void onReloadScript(bool fullReload);

	/**
		Gets whether the process is closing
	*/
	static PyObject* __py_isShuttingDown(PyObject* self, PyObject* args);

	/**
		Get internal network address for process
	*/
	static PyObject* __py_address(PyObject* self, PyObject* args);

	WitnessedTimeoutHandler	* pWitnessedTimeoutHandler(){ return pWitnessedTimeoutHandler_; }

	/**
		Network interface
		Another cellapp entity wants to teleport to the space on this cellapp
	*/
	void reqTeleportToCellApp(Network::Channel* pChannel, MemoryStream& s);
	void reqTeleportToCellAppCB(Network::Channel* pChannel, MemoryStream& s);
	void reqTeleportToCellAppOver(Network::Channel* pChannel, MemoryStream& s);

	/**
		Get and set the ghost manager
	*/
	void pGhostManager(GhostManager* v){ pGhostManager_ = v; }
	GhostManager* pGhostManager() const{ return pGhostManager_; }

	ArraySize spaceSize() const { return (ArraySize)Spaces::size(); }

	/** 
		Raycast
	*/
	int raycast(SPACE_ID spaceID, int layer, const Position3D& start, const Position3D& end, std::vector<Position3D>& hitPos);
	static PyObject* __py_raycast(PyObject* self, PyObject* args);

	uint32 flags() const { return flags_; }
	void flags(uint32 v) { flags_ = v; }
	static PyObject* __py_setFlags(PyObject* self, PyObject* args);
	static PyObject* __py_getFlags(PyObject* self, PyObject* args);

protected:
	// cellAppData
	GlobalDataClient*					pCellAppData_;

	ForwardComponent_MessageBuffer		forward_messagebuffer_;

	Updatables							updatables_;

	// All cells
	Cells								cells_;

	TelnetServer*						pTelnetServer_;

	WitnessedTimeoutHandler	*			pWitnessedTimeoutHandler_;

	GhostManager*						pGhostManager_;
	
	// APP flags
	uint32								flags_;

	// View space through tools
	SpaceViewers						spaceViewers_;
};

}

#endif // KBE_CELLAPP_H
