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


#ifndef KBE_BASEAPP_H
#define KBE_BASEAPP_H
	
// common include	
#include "entity.h"
#include "proxy.h"
#include "profile.h"
#include "server/entity_app.h"
#include "server/pendingLoginmgr.h"
#include "server/forward_messagebuffer.h"
#include "network/endpoint.h"

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

namespace Network{
	class Channel;
	class Bundle;
}

class Proxy;
class Backuper;
class Archiver;
class TelnetServer;
class RestoreEntityHandler;
class InitProgressHandler;

class Baseapp :	public EntityApp<Entity>,
				public Singleton<Baseapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_CHECK_STATUS = TIMEOUT_ENTITYAPP_MAX + 1,
		TIMEOUT_MAX
	};
	
	Baseapp(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Baseapp();
	
	virtual bool installPyModules();
	virtual void onInstallPyModules();
	virtual bool uninstallPyModules();

	bool run();
	
	/** 
		Correlation Processing Interface
	*/
	virtual void handleTimeout(TimerHandle handle, void * arg);
	virtual void handleGameTick();
	void handleCheckStatusTick();
	void handleBackup();
	void handleArchive();

	/** 
		Initialize related interfaces
	*/
	bool initializeBegin();
	bool initializeEnd();
	void finalise();
	
	virtual bool canShutdown();
	virtual void onShutdownBegin();
	virtual void onShutdown(bool first);
	virtual void onShutdownEnd();

	virtual bool initializeWatcher();

	static PyObject* __py_quantumPassedPercent(PyObject* self, PyObject* args);
	float _getLoad() const { return getLoad(); }
	virtual void onUpdateLoad();

	virtual void onChannelDeregister(Network::Channel * pChannel);

	void onCellAppDeath(Network::Channel * pChannel);

	/** Network interface
		Dbmgr tells the address of other baseapp or cellapp that has been started
		Current app needs to actively establish a connection with them
	*/
	virtual void onGetEntityAppFromDbmgr(Network::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							COMPONENT_TYPE componentType, COMPONENT_ID componentID, COMPONENT_ORDER globalorderID, COMPONENT_ORDER grouporderID,
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport, std::string& extaddrEx);
	
	/** Network interface
		A client informs the app that it is active.
	*/
	void onClientActiveTick(Network::Channel* pChannel);

	/** Network interface
		Automatic entity load information returned by the database
	*/
	void onEntityAutoLoadCBFromDBMgr(Network::Channel* pChannel, MemoryStream& s);

	/** 
		on Entity created callback
	*/
	virtual Entity* onCreateEntity(PyObject* pyEntity, ScriptDefModule* sm, ENTITY_ID eid);

	/** 
		Create an Entity
	*/
	static PyObject* __py_createEntity(PyObject* self, PyObject* args);
	static PyObject* __py_createEntityAnywhere(PyObject* self, PyObject* args);
	static PyObject* __py_createEntityRemotely(PyObject* self, PyObject* args);
	static PyObject* __py_createEntityFromDBID(PyObject* self, PyObject* args);
	static PyObject* __py_createEntityAnywhereFromDBID(PyObject* self, PyObject* args);
	static PyObject* __py_createEntityRemotelyFromDBID(PyObject* self, PyObject* args);
	
	/**
		Create a new space
	*/
	void createCellEntityInNewSpace(Entity* pEntity, PyObject* pyCellappIndex);

	/**
		Restore a space
	*/
	void restoreSpaceInCell(Entity* pEntity);

	/** 
		Create a base-Entity on a lower-load baseapp
	*/
	void createEntityAnywhere(const char* entityType, PyObject* params, PyObject* pyCallback);

	/** Received Baseappmgr decision to execute a createEntityAnywhere request on this Baseapp from another Baseapp
		@param entityType	: Entity type, defined in Entities.xml.
		@param strInitData	: After the entity is created, some data that should be initialized for him which needs to be unwrapped using pickle.loads.
		@param componentID	: The component ID of the baseapp requesting to create an entity
	*/
	void onCreateEntityAnywhere(Network::Channel* pChannel, MemoryStream& s);

	/**
	baseapp createEntityAnywhere callbacks
	*/
	void onCreateEntityAnywhereCallback(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void _onCreateEntityAnywhereCallback(Network::Channel* pChannel, CALLBACK_ID callbackID,
		std::string& entityType, ENTITY_ID eid, COMPONENT_ID componentID);

	/**
	Create a base-Entity on a lower-load baseapp
	*/
	void createEntityRemotely(const char* entityType, COMPONENT_ID componentID, PyObject* params, PyObject* pyCallback);

	/** 收Received Baseappmgr decision to execute a createEntityAnywhere request on this Baseapp from another Baseapp
	@param entityType	: Entity type, defined in Entities.xml.
	@param strInitData	: After the entity is created, some data that should be initialized for him which needs to be unwrapped using pickle.loads.
	@param componentID	: The component ID of the baseapp requesting to create an entity
	*/
	void onCreateEntityRemotely(Network::Channel* pChannel, MemoryStream& s);

	/**
	baseapp createEntityAnywhere callbacks
	*/
	void onCreateEntityRemotelyCallback(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void _onCreateEntityRemotelyCallback(Network::Channel* pChannel, CALLBACK_ID callbackID,
		std::string& entityType, ENTITY_ID eid, COMPONENT_ID componentID);

	/** 
		Create an entity by obtaining information from db
	*/
	void createEntityFromDBID(const char* entityType, DBID dbid, PyObject* pyCallback, const std::string& dbInterfaceName);

	/** Network interface
		createEntityFromDBID callback
	*/
	void onCreateEntityFromDBIDCallback(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** 
		Create an entity by obtaining information from db
	*/
	void createEntityAnywhereFromDBID(const char* entityType, DBID dbid, PyObject* pyCallback, const std::string& dbInterfaceName);

	/** Network interface
		createEntityAnywhereFromDBID callback
	*/
	// Component ID callback used to create an entity from a baseappmgr query
	void onGetCreateEntityAnywhereFromDBIDBestBaseappID(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		createEntityAnywhereFromDBID callback
	*/
	// Callback from the database
	void onCreateEntityAnywhereFromDBIDCallback(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	// Request to create an entity on this process
	void createEntityAnywhereFromDBIDOtherBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	// Callback after creation
	void onCreateEntityAnywhereFromDBIDOtherBaseappCallback(Network::Channel* pChannel, COMPONENT_ID createByBaseappID, 
							std::string entityType, ENTITY_ID createdEntityID, CALLBACK_ID callbackID, DBID dbid);
	
	/**
	Create an entity by obtaining information from db
	*/
	void createEntityRemotelyFromDBID(const char* entityType, DBID dbid, COMPONENT_ID createToComponentID, 
		PyObject* pyCallback, const std::string& dbInterfaceName);

	/** Network interface
	createEntityRemotelyFromDBID callback
	*/
	// Callback from the database
	void onCreateEntityRemotelyFromDBIDCallback(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	// Request to create an entity on this process
	void createEntityRemotelyFromDBIDOtherBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	// Callback after creation
	void onCreateEntityRemotelyFromDBIDOtherBaseappCallback(Network::Channel* pChannel, COMPONENT_ID createByBaseappID,
		std::string entityType, ENTITY_ID createdEntityID, CALLBACK_ID callbackID, DBID dbid);

	/** 
		Create a cellEntity on a specified cell for a baseEntity
	*/
	void createCellEntity(EntityCallAbstract* createToCellEntityCall, Entity* pEntity);
	
	/** Network interface
		createCellEntity failed callback
	*/
	void onCreateCellFailure(Network::Channel* pChannel, ENTITY_ID entityID);

	/** Network interface
		createCellEntity cell entity created successfully callback
	*/
	void onEntityGetCell(Network::Channel* pChannel, ENTITY_ID id, COMPONENT_ID componentID, SPACE_ID spaceID);

	/** 
		Notifies the client to create a proxy-corresponding entity 
	*/
	bool createClientProxies(Proxy* pEntity, bool reload = false);

	/** 
		Execute a database command to the DBMGR request
	*/
	static PyObject* __py_executeRawDatabaseCommand(PyObject* self, PyObject* args);
	void executeRawDatabaseCommand(const char* datas, uint32 size, PyObject* pycallback, ENTITY_ID eid, const std::string& dbInterfaceName);
	void onExecuteRawDatabaseCommandCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Dbmgr sends initial information
		startID: Initial allocation of ENTITY_ID segment starting position
		endID: Initial allocation of ENTITY_ID segment end position
		startGlobalOrder: Global boot order includes various components
		startGroupOrder: The order in which the groups are started, such as the first few in all Baseapp.
		machineGroupOrder: The real group order in the machine, providing the underlying layer to at some point determine whether the first Baseapp is used
	*/
	void onDbmgrInitCompleted(Network::Channel* pChannel, 
		GAME_TIME gametime, ENTITY_ID startID, ENTITY_ID endID, COMPONENT_ORDER startGlobalOrder, 
		COMPONENT_ORDER startGroupOrder, const std::string& digest);

	/** Network interface
		Dbmgr broadcast global data changes
	*/
	void onBroadcastBaseAppDataChanged(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Register the account to be logged in. After registering, login to this Baseapp is allowed.
	*/
	void registerPendingLogin(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		New user requests to login to this Baseapp
	*/
	void loginBaseapp(Network::Channel* pChannel, std::string& accountName, std::string& password);

	/**
		Kick a channel
	*/
	void kickChannel(Network::Channel* pChannel, SERVER_ERROR_CODE failedcode);

	/** Network interface
		Re-login to quickly establish a connection with the Baseapp
		(The premise is that before login, the reconnection can be quickly established on the
		 premise that the Entity of the front end has not timed out and been destroyed and the
		 server can reestablish control of the entity.)
	*/
	void reloginBaseapp(Network::Channel* pChannel, std::string& accountName, 
		std::string& password, uint64 key, ENTITY_ID entityID);

	/**
	   Login failed
	   @failedcode:	NETWORK_ERR_SRV_NO_READY: The server is not ready
					NETWORK_ERR_ILLEGAL_LOGIN: Illegal login
					NETWORK_ERR_NAME_PASSWORD: Incorrect username or password
	*/
	void loginBaseappFailed(Network::Channel* pChannel, std::string& accountName, 
		SERVER_ERROR_CODE failedcode, bool relogin = false);

	/** Network interface
		Get account entity information from Dbmgr
	*/
	void onQueryAccountCBFromDbmgr(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/**
		The client itself has entered the world.
	*/
	void onClientEntityEnterWorld(Proxy* pEntity, COMPONENT_ID componentID);

	/** Network interface
		The entity receives a remote call request, initiated by an entityCall on the app.
		(only used internally by the server, the client's entitycall method calls onRemoteCellMethodCallFromClient)
	*/
	void onEntityCall(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** Network interface
		Client calls Entity's Cell method
	*/
	void onRemoteCallCellMethodFromClient(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Client updates data
	*/
	void onUpdateDataFromClient(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void onUpdateDataFromClientForControlledEntity(Network::Channel* pChannel, KBEngine::MemoryStream& s);


	/** Network interface
		cellapp backs up entity cell data
	*/
	void onBackupEntityCellData(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		cellapp writeToDB complete
	*/
	void onCellWriteToDBCompleted(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		cellapp forwards entity message to client
	*/
	void forwardMessageToClientFromCellapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Cellapp forwards the entity message to the cellEntity of a baseEntity
	*/
	void forwardMessageToCellappFromCellapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/**
		Get game time
	*/
	static PyObject* __py_gametime(PyObject* self, PyObject* args);

	/** Network interface
		Write entity to DB callback
	*/
	void onWriteToDBCallback(Network::Channel* pChannel, ENTITY_ID eid, DBID entityDBID, 
		uint16 dbInterfaceIndex, CALLBACK_ID callbackID, bool success);

	/**
		Increase proxies count
	*/
	void incProxiesCount() { ++numProxies_; }

	/**
		Decrease proxies count
	*/
	void decProxiesCount() { --numProxies_; }

	/**
		get proxies count
	*/
	int32 numProxies() const { return numProxies_; }

	/**
		get numClients count
	*/
	int32 numClients() { return this->networkInterface().numExtChannels(); }
	
	/** 
		Request recharge
	*/
	static PyObject* __py_charge(PyObject* self, PyObject* args);
	void charge(std::string chargeID, DBID dbid, const std::string& datas, PyObject* pycallback);
	void onChargeCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/**
		hook entitycallcall
	*/
	RemoteEntityMethod* createEntityCallCallEntityRemoteMethod(MethodDescription* pMethodDescription, EntityCallAbstract* pEntityCall);

	virtual void onHello(Network::Channel* pChannel, 
		const std::string& verInfo, 
		const std::string& scriptVerInfo, 
		const std::string& encryptedKey);

	// Engine version does not match
	virtual void onVersionNotMatch(Network::Channel* pChannel);

	// Engine script layer version does not match
	virtual void onScriptVersionNotMatch(Network::Channel* pChannel);

	/** Network interface
		Request return results from other app (disaster recovery)
	*/
	void onRequestRestoreCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/**
		A cell entity is restored
	*/
	void onRestoreEntitiesOver(RestoreEntityHandler* pRestoreEntityHandler);

	/** Network interface
		Space restored from Baseapp, determines whether the current baseapp has related entities and needs to restore their cells
	*/
	void onRestoreSpaceCellFromOtherBaseapp(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Another app requested to view this app
	*/
	virtual void lookApp(Network::Channel* pChannel);

	/** Network interface
		Client-side protocol export
	*/
	void importClientMessages(Network::Channel* pChannel);

	/** Network interface
		Client EntityDef export
	*/
	void importClientEntityDef(Network::Channel* pChannel);

	/**
		Re-import all scripts
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

	/**
		Delete an entity from the database by dbid
		
		If the entity is not checked out from the database, the callback returns true
		 and it can be deleted. If it is checked out, the callback returns the EntityCall,
		 otherwise the callback returns false
	*/
	static PyObject* __py_deleteEntityByDBID(PyObject* self, PyObject* args);

	/** Network interface
		If the entity is not checked out from the database, the callback returns true
		 and it can be deleted. If it is checked out, the callback returns the EntityCall,
		 otherwise the callback returns false
	*/
	void deleteEntityByDBIDCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/**
		Check if an entity is checked out from the database by dbid
	*/
	static PyObject* __py_lookUpEntityByDBID(PyObject* self, PyObject* args);

	/** Network interface
		The callback returns BASEENTITYCALL if it is checked out, returns true if the entity is not checked out, and any other reason returns false.
	*/
	void lookUpEntityByDBIDCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Request to bind email to account
	*/
	void reqAccountBindEmail(Network::Channel* pChannel, ENTITY_ID entityID, std::string& password, std::string& email);

	/** Network interface
		DBMgr callback, returns result after requesting to bind email
	*/
	void onReqAccountBindEmailCBFromDBMgr(Network::Channel* pChannel, ENTITY_ID entityID, std::string& accountName, std::string& email,
		SERVER_ERROR_CODE failedcode, std::string& code);

	/** Network interface
		Request to bind email callback, baseappmgr returns the address needed to find loginapp
	*/
	void onReqAccountBindEmailCBFromBaseappmgr(Network::Channel* pChannel, ENTITY_ID entityID, std::string& accountName, std::string& email,
		SERVER_ERROR_CODE failedcode, std::string& code, std::string& loginappCBHost, uint16 loginappCBPort);

	/** Network interface
		Request new password
	*/
	void reqAccountNewPassword(Network::Channel* pChannel, ENTITY_ID entityID, std::string& oldpassworld, std::string& newpassword);

	void onReqAccountNewPasswordCB(Network::Channel* pChannel, ENTITY_ID entityID, std::string& accountName,
		SERVER_ERROR_CODE failedcode);

	uint32 flags() const { return flags_; }
	void flags(uint32 v) { flags_ = v; }
	static PyObject* __py_setFlags(PyObject* self, PyObject* args);
	static PyObject* __py_getFlags(PyObject* self, PyObject* args);
	
protected:
	TimerHandle												loopCheckTimerHandle_;

	// globalBases
	GlobalDataClient*										pBaseAppData_;

	// Records accounts that are logged onto the server but not yet processed
	PendingLoginMgr											pendingLoginMgr_;

	ForwardComponent_MessageBuffer							forward_messagebuffer_;

	// Backup archive related
	KBEShared_ptr< Backuper >								pBackuper_;	
	KBEShared_ptr< Archiver >								pArchiver_;	

	int32													numProxies_;

	TelnetServer*											pTelnetServer_;

	std::vector< KBEShared_ptr< RestoreEntityHandler > >	pRestoreEntityHandlers_;

	TimerHandle												pResmgrTimerHandle_;

	InitProgressHandler*									pInitProgressHandler_;
	
	// APP flags
	uint32													flags_;

	// Dynamic import of entitydef protocol for clients
	Network::Bundle*										pBundleImportEntityDefDatas_;
};

}

#endif // KBE_BASEAPP_H
