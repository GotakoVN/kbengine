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


#if defined(DEFINE_IN_INTERFACE)
	#undef KBE_BASEAPP_INTERFACE_H
#endif


#ifndef KBE_BASEAPP_INTERFACE_H
#define KBE_BASEAPP_INTERFACE_H

// common include	
#if defined(BASEAPP)
#include "baseapp.h"
#endif
#include "baseapp_interface_macros.h"
#include "entity_interface_macros.h"
#include "proxy_interface_macros.h"
#include "network/interface_defs.h"
#include "server/server_errors.h"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	All Baseapp message interfaces are defined here
*/
NETWORK_INTERFACE_DECLARE_BEGIN(BaseappInterface)
	// The client requests to import the protocol.
	BASEAPP_MESSAGE_EXPOSED(importClientMessages)
	BASEAPP_MESSAGE_DECLARE_ARGS0(importClientMessages,								NETWORK_FIXED_MESSAGE)

	// Client EntityDef export
	BASEAPP_MESSAGE_EXPOSED(importClientEntityDef)
	BASEAPP_MESSAGE_DECLARE_ARGS0(importClientEntityDef,							NETWORK_FIXED_MESSAGE)

	// An app actively requests disconnection
	BASEAPP_MESSAGE_DECLARE_ARGS0(reqClose,											NETWORK_FIXED_MESSAGE)

	// Another app actively requests a look.
	BASEAPP_MESSAGE_DECLARE_ARGS0(lookApp,											NETWORK_FIXED_MESSAGE)

	// Another app requests to see the app load status.
	BASEAPP_MESSAGE_DECLARE_ARGS0(queryLoad,										NETWORK_FIXED_MESSAGE)

	// Console executes python statements remotely.
	BASEAPP_MESSAGE_DECLARE_STREAM(onExecScriptCommand,								NETWORK_VARIABLE_MESSAGE)

	// App registers its own interface address to this app
	BASEAPP_MESSAGE_DECLARE_ARGS11(onRegisterNewApp,								NETWORK_VARIABLE_MESSAGE,
									int32,											uid, 
									std::string,									username,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID, 
									COMPONENT_ORDER,								globalorderID,
									COMPONENT_ORDER,								grouporderID,
									uint32,											intaddr, 
									uint16,											intport,
									uint32,											extaddr, 
									uint16,											extport,
									std::string,									extaddrEx)

	// Dbmgr tells the address of other baseapp or cellapp that has been started
	// Current app needs to actively establish a connection with them
	BASEAPP_MESSAGE_DECLARE_ARGS11(onGetEntityAppFromDbmgr,							NETWORK_VARIABLE_MESSAGE,
									int32,											uid, 
									std::string,									username,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID, 
									COMPONENT_ORDER,								globalorderID,
									COMPONENT_ORDER,								grouporderID,
									uint32,											intaddr, 
									uint16,											intport,
									uint32,											extaddr, 
									uint16,											extport,
									std::string,									extaddrEx)

	// An app requests to get a callback for a EntityId segment
	BASEAPP_MESSAGE_DECLARE_ARGS2(onReqAllocEntityID,								NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										startID,
									ENTITY_ID,										endID)


	// An app requests to get a callback for a EntityId segment
	BASEAPP_MESSAGE_DECLARE_ARGS6(onDbmgrInitCompleted,								NETWORK_VARIABLE_MESSAGE,
									GAME_TIME,										gametime, 
									ENTITY_ID,										startID,
									ENTITY_ID,										endID,
									COMPONENT_ORDER,								startGlobalOrder,
									COMPONENT_ORDER,								startGroupOrder,
									std::string,									digest)

	// hello handshaking
	BASEAPP_MESSAGE_EXPOSED(hello)
	BASEAPP_MESSAGE_DECLARE_STREAM(hello,											NETWORK_VARIABLE_MESSAGE)

	// global data changes
	BASEAPP_MESSAGE_DECLARE_STREAM(onBroadcastGlobalDataChanged,					NETWORK_VARIABLE_MESSAGE)
	BASEAPP_MESSAGE_DECLARE_STREAM(onBroadcastBaseAppDataChanged,					NETWORK_VARIABLE_MESSAGE)

	// Another app informs this app that it's active.
	BASEAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,									NETWORK_FIXED_MESSAGE,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID)

	// Client informs this app that it's active.
	BASEAPP_MESSAGE_EXPOSED(onClientActiveTick)
	BASEAPP_MESSAGE_DECLARE_ARGS0(onClientActiveTick,								NETWORK_FIXED_MESSAGE)

	// Received Baseappmgr decision to execute a request for a createEntityAnywhere on this Baseapp
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityAnywhere,							NETWORK_VARIABLE_MESSAGE)

	// After createEntityAnywhere succeeds, it recalls the message to the entity of the Baseapp that initiated createEntityAnywhere.
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityAnywhereCallback,					NETWORK_FIXED_MESSAGE)

	// After the createentityremotely succeeds, it recalls the message to the entity of the Baseapp that initiated the layer createentityremotely.
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityRemotely,							NETWORK_FIXED_MESSAGE)

	// After the createentityremotely succeeds, it recalls the message to the entity of the Baseapp that initiated the layer createentityremotely.
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityRemotelyCallback,					 NETWORK_FIXED_MESSAGE)

	// The createCellEntity cell entity creates a successful callback.
	BASEAPP_MESSAGE_DECLARE_ARGS3(onEntityGetCell,									NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										id,
									COMPONENT_ID,									componentID,
									SPACE_ID,										spaceID)

	// The createCellEntity creates a failed callback.
	BASEAPP_MESSAGE_DECLARE_ARGS1(onCreateCellFailure,								NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										entityID)

	// Loginapp registers itself with an account to be logged in, forwarded by baseappmgr.
	BASEAPP_MESSAGE_DECLARE_STREAM(registerPendingLogin,							NETWORK_VARIABLE_MESSAGE)

	// Automatic entity load information for queries in the database returns 
	BASEAPP_MESSAGE_DECLARE_STREAM(onEntityAutoLoadCBFromDBMgr,						NETWORK_VARIABLE_MESSAGE)

	// Front-end Request login to the baseapp.
	BASEAPP_MESSAGE_EXPOSED(loginBaseapp)
	BASEAPP_MESSAGE_DECLARE_ARGS2(loginBaseapp,										NETWORK_VARIABLE_MESSAGE,
									std::string,									accountName,
									std::string,									password)

	// Front-end requests log back on to the Baseapp.
	BASEAPP_MESSAGE_EXPOSED(reloginBaseapp)
	BASEAPP_MESSAGE_DECLARE_ARGS4(reloginBaseapp,									NETWORK_VARIABLE_MESSAGE,
									std::string,									accountName,
									std::string,									password,
									uint64,											key,
									ENTITY_ID,										entityID)

	// Get account entity information from Dbmgr
	BASEAPP_MESSAGE_DECLARE_STREAM(onQueryAccountCBFromDbmgr,						NETWORK_VARIABLE_MESSAGE)

	// Entity received a remote call request, initiated by a entitycall on an app
	BASEAPP_MESSAGE_DECLARE_STREAM(onEntityCall,									NETWORK_VARIABLE_MESSAGE)
	
	// The client calls a method on the cell entity
	BASEAPP_MESSAGE_EXPOSED(onRemoteCallCellMethodFromClient)
	BASEAPP_MESSAGE_DECLARE_STREAM(onRemoteCallCellMethodFromClient,				NETWORK_VARIABLE_MESSAGE)

	// Client Update data
	BASEAPP_MESSAGE_EXPOSED(onUpdateDataFromClient)
	BASEAPP_MESSAGE_DECLARE_STREAM(onUpdateDataFromClient,							NETWORK_VARIABLE_MESSAGE)
	BASEAPP_MESSAGE_EXPOSED(onUpdateDataFromClientForControlledEntity)
	BASEAPP_MESSAGE_DECLARE_STREAM(onUpdateDataFromClientForControlledEntity,		NETWORK_VARIABLE_MESSAGE)

	// Executerawdatabasecommand from Dbmgr's callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onExecuteRawDatabaseCommandCB,					NETWORK_VARIABLE_MESSAGE)

	// Cellapp Backup Entity cell data
	BASEAPP_MESSAGE_DECLARE_STREAM(onBackupEntityCellData,							NETWORK_VARIABLE_MESSAGE)

	// Cellapp Writetodb Complete
	BASEAPP_MESSAGE_DECLARE_STREAM(onCellWriteToDBCompleted,						NETWORK_VARIABLE_MESSAGE)

	// Cellapp forward entity message to client
	BASEAPP_MESSAGE_DECLARE_STREAM(forwardMessageToClientFromCellapp,				NETWORK_VARIABLE_MESSAGE)

	// Cellapp forwards entity messages to the cellentity of a baseentity 
	BASEAPP_MESSAGE_DECLARE_STREAM(forwardMessageToCellappFromCellapp,				NETWORK_VARIABLE_MESSAGE)

	// Request to shut down the server
	BASEAPP_MESSAGE_DECLARE_STREAM(reqCloseServer,									NETWORK_VARIABLE_MESSAGE)

	// Write entity to db callback.
	BASEAPP_MESSAGE_DECLARE_ARGS5(onWriteToDBCallback,								NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										eid,
									DBID,											entityDBID,
									uint16,											dbInterfaceIndex,
									CALLBACK_ID,									callbackID,
									bool,											success)

	// createEntityFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityFromDBIDCallback,					NETWORK_FIXED_MESSAGE)

	// createEntityAnywhereFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onGetCreateEntityAnywhereFromDBIDBestBaseappID,	NETWORK_FIXED_MESSAGE)

	// createEntityAnywhereFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityAnywhereFromDBIDCallback,			NETWORK_FIXED_MESSAGE)

	// createEntityAnywhereFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(createEntityAnywhereFromDBIDOtherBaseapp,		NETWORK_FIXED_MESSAGE)

	// createEntityAnywhereFromDBID callback
	BASEAPP_MESSAGE_DECLARE_ARGS5(onCreateEntityAnywhereFromDBIDOtherBaseappCallback,NETWORK_VARIABLE_MESSAGE,
									COMPONENT_ID,									createByBaseappID,
									std::string,									entityType,
									ENTITY_ID,										createdEntityID,
									CALLBACK_ID,									callbackID,
									DBID,											dbid)

	// createEntityRemotelyFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateEntityRemotelyFromDBIDCallback,			NETWORK_FIXED_MESSAGE)

	// createEntityRemotelyFromDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(createEntityRemotelyFromDBIDOtherBaseapp,		NETWORK_FIXED_MESSAGE)

	// createEntityRemotelyFromDBID callback
	BASEAPP_MESSAGE_DECLARE_ARGS5(onCreateEntityRemotelyFromDBIDOtherBaseappCallback,NETWORK_VARIABLE_MESSAGE,
									COMPONENT_ID,									createByBaseappID,
									std::string,									entityType,
									ENTITY_ID,										createdEntityID,
									CALLBACK_ID,									callbackID,
									DBID,											dbid)

	// Request for watcher data
	BASEAPP_MESSAGE_DECLARE_STREAM(queryWatcher,									NETWORK_VARIABLE_MESSAGE)

	// Recharge callback
	BASEAPP_MESSAGE_DECLARE_STREAM(onChargeCB,										NETWORK_VARIABLE_MESSAGE)

	// Start profile
	BASEAPP_MESSAGE_DECLARE_STREAM(startProfile,									NETWORK_VARIABLE_MESSAGE)

	// Request to delete entity from database
	BASEAPP_MESSAGE_DECLARE_STREAM(deleteEntityByDBIDCB,							NETWORK_VARIABLE_MESSAGE)
	
	// lookUpEntityByDBID callback
	BASEAPP_MESSAGE_DECLARE_STREAM(lookUpEntityByDBIDCB,							NETWORK_VARIABLE_MESSAGE)

	// Space restored from Baseapp, determines whether the current baseapp has related entities and needs to restore their cells
	BASEAPP_MESSAGE_DECLARE_STREAM(onRestoreSpaceCellFromOtherBaseapp,				NETWORK_VARIABLE_MESSAGE)

	// Request to return results from other APP (disaster recovery).
	BASEAPP_MESSAGE_DECLARE_STREAM(onRequestRestoreCB,								NETWORK_VARIABLE_MESSAGE)

	// Request binding email
	BASEAPP_MESSAGE_EXPOSED(reqAccountBindEmail)
	BASEAPP_MESSAGE_DECLARE_ARGS3(reqAccountBindEmail,								NETWORK_VARIABLE_MESSAGE,
									ENTITY_ID,										entityID,
									std::string,									password,
									std::string,									email)

	// Request callback for binding email request
	BASEAPP_MESSAGE_DECLARE_ARGS5(onReqAccountBindEmailCBFromDBMgr,					NETWORK_VARIABLE_MESSAGE,
									ENTITY_ID,										entityID,
									std::string,									accountName,
									std::string,									email,
									SERVER_ERROR_CODE,								failedcode,
									std::string,									code)

	// Baseapp request binding e-mail (return addressed needed to find loginapp)
	BASEAPP_MESSAGE_DECLARE_ARGS7(onReqAccountBindEmailCBFromBaseappmgr,			NETWORK_VARIABLE_MESSAGE,
									ENTITY_ID,										entityID,
									std::string,									accountName,
									std::string,									email,
									SERVER_ERROR_CODE,								failedcode,
									std::string,									code,
									std::string,									loginappCBHost,
									uint16,											loginappCBPort)

	// Request to change password
	BASEAPP_MESSAGE_EXPOSED(reqAccountNewPassword)
	BASEAPP_MESSAGE_DECLARE_ARGS3(reqAccountNewPassword,							NETWORK_VARIABLE_MESSAGE,
									ENTITY_ID,										entityID,
									std::string,									oldpassword,
									std::string,									newpassword)

	// Request to change password callback
	BASEAPP_MESSAGE_DECLARE_ARGS3(onReqAccountNewPasswordCB,						NETWORK_VARIABLE_MESSAGE,
									ENTITY_ID,										entityID,
									std::string,									accountName,
									SERVER_ERROR_CODE,								failedcode)

	// Request to kill the current app
	BASEAPP_MESSAGE_DECLARE_STREAM(reqKillServer,									NETWORK_VARIABLE_MESSAGE)

	//--------------------------------------------Entity----------------------------------------------------------
	// Remote call entity method
	ENTITY_MESSAGE_EXPOSED(onRemoteMethodCall)
	ENTITY_MESSAGE_DECLARE_STREAM(onRemoteMethodCall,								NETWORK_VARIABLE_MESSAGE)

	// Cellapp informs that the cell part of the entity is destroyed or lost
	ENTITY_MESSAGE_DECLARE_STREAM(onLoseCell,										NETWORK_VARIABLE_MESSAGE)

	// The client directly sends messages to the cell entity
	ENTITY_MESSAGE_EXPOSED(forwardEntityMessageToCellappFromClient)
	ENTITY_MESSAGE_DECLARE_STREAM(forwardEntityMessageToCellappFromClient,			NETWORK_VARIABLE_MESSAGE)
	
	// An entity requests to teleport to the space of another entity
	ENTITY_MESSAGE_DECLARE_ARGS3(reqTeleportOther,									NETWORK_FIXED_MESSAGE,
								ENTITY_ID,											reqTeleportEntityID,
								COMPONENT_ID,										reqTeleportEntityAppID,
								COMPONENT_ID,										reqTeleportEntityBaseAppID)

	// Callback result of an entity request teleport
	ENTITY_MESSAGE_DECLARE_ARGS2(onTeleportCB,										NETWORK_FIXED_MESSAGE,
								SPACE_ID,											spaceID,
								bool,												fromCellTeleport)

	// 某个entity请求teleport后的回调结果
	ENTITY_MESSAGE_DECLARE_ARGS1(onGetDBID,											NETWORK_FIXED_MESSAGE,
								DBID,												dbid)

	// Entity request to migrate to space procedure on another cellapp begins
	ENTITY_MESSAGE_DECLARE_ARGS2(onMigrationCellappStart,							NETWORK_FIXED_MESSAGE,
								COMPONENT_ID,										sourceCellAppID,
								COMPONENT_ID,										targetCellAppID)
		
	// Entity request to migrate to space procedure on another cellapp ends
	ENTITY_MESSAGE_DECLARE_ARGS2(onMigrationCellappEnd,								NETWORK_FIXED_MESSAGE,
								COMPONENT_ID,										sourceCellAppID,
								COMPONENT_ID,										targetCellAppID)

	//--------------------------------------------Proxy---------------------------------------------------------
	/**
		Remote call entity method
	*/
	//PROXY_MESSAGE_EXPOSED(onClientGetCell)
	//PROXY_MESSAGE_DECLARE_ARGS0(onClientGetCell,									NETWORK_FIXED_MESSAGE)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
