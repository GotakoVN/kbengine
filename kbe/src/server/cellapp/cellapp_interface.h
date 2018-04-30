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
	#undef KBE_CELLAPP_INTERFACE_H
#endif


#ifndef KBE_CELLAPP_INTERFACE_H
#define KBE_CELLAPP_INTERFACE_H

// common include	
#if defined(CELLAPP)
#include "entity.h"
#include "cellapp.h"
#endif
#include "cellapp_interface_macros.h"
#include "entity_interface_macros.h"
#include "network/interface_defs.h"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	Cellapp all message interfaces are defined here
*/
NETWORK_INTERFACE_DECLARE_BEGIN(CellappInterface)
	// An app registers its own interface address to this app
	CELLAPP_MESSAGE_DECLARE_ARGS11(onRegisterNewApp,								NETWORK_VARIABLE_MESSAGE,
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

	// An app actively requests a look.
	CELLAPP_MESSAGE_DECLARE_ARGS0(lookApp,											NETWORK_FIXED_MESSAGE)

	// An app requests to see the app load status.
	CELLAPP_MESSAGE_DECLARE_ARGS0(queryLoad,										NETWORK_FIXED_MESSAGE)

	// Console executes python statements remotely.
	CELLAPP_MESSAGE_DECLARE_STREAM(onExecScriptCommand,								NETWORK_VARIABLE_MESSAGE)

	// Dbmgr tells the address of other baseapp or cellapp that has been started
	// Current app needs to actively establish a connection with them
	CELLAPP_MESSAGE_DECLARE_ARGS11(onGetEntityAppFromDbmgr,							NETWORK_VARIABLE_MESSAGE,
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

	// An app requesting a callback for an entityID segment
	CELLAPP_MESSAGE_DECLARE_ARGS2(onReqAllocEntityID,								NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										startID,
									ENTITY_ID,										endID)

	// 某app请求获取一个entityID段的回调
	CELLAPP_MESSAGE_DECLARE_ARGS6(onDbmgrInitCompleted,								NETWORK_VARIABLE_MESSAGE,
									GAME_TIME,										gametime, 
									ENTITY_ID,										startID,
									ENTITY_ID,										endID,
									COMPONENT_ORDER,								startGlobalOrder,
									COMPONENT_ORDER,								startGroupOrder,
									std::string,									digest)

	// Global data changes
	CELLAPP_MESSAGE_DECLARE_STREAM(onBroadcastGlobalDataChanged,					NETWORK_VARIABLE_MESSAGE)
	CELLAPP_MESSAGE_DECLARE_STREAM(onBroadcastCellAppDataChanged,					NETWORK_VARIABLE_MESSAGE)

	// The baseEntity request is created in a new space.
	CELLAPP_MESSAGE_DECLARE_STREAM(onCreateCellEntityInNewSpaceFromBaseapp,			NETWORK_VARIABLE_MESSAGE)

	// The baseEntity request is restored in a new space.
	CELLAPP_MESSAGE_DECLARE_STREAM(onRestoreSpaceInCellFromBaseapp,					NETWORK_VARIABLE_MESSAGE)

	// Other app requests for this disaster recovery.
	CELLAPP_MESSAGE_DECLARE_STREAM(requestRestore,									NETWORK_VARIABLE_MESSAGE)
	
	// Baseapp requestS to create a entity on this cellapp.
	CELLAPP_MESSAGE_DECLARE_STREAM(onCreateCellEntityFromBaseapp,					NETWORK_VARIABLE_MESSAGE)

	// Destroy a cellentity.
	CELLAPP_MESSAGE_DECLARE_ARGS1(onDestroyCellEntityFromBaseapp,					NETWORK_FIXED_MESSAGE,
									ENTITY_ID,										eid)

	// AnOTHER app informs this app that it's active.
	CELLAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,									NETWORK_FIXED_MESSAGE,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID)

	// Entity received a remote call request, initiated by a entitycall on an app
	CELLAPP_MESSAGE_DECLARE_STREAM(onEntityCall,									NETWORK_VARIABLE_MESSAGE)

	// The cell method of client accessing entity
	CELLAPP_MESSAGE_DECLARE_STREAM(onRemoteCallMethodFromClient,					NETWORK_VARIABLE_MESSAGE)

	// Client Update data
	CELLAPP_MESSAGE_DECLARE_STREAM(onUpdateDataFromClient,							NETWORK_VARIABLE_MESSAGE)
	CELLAPP_MESSAGE_DECLARE_STREAM(onUpdateDataFromClientForControlledEntity,		NETWORK_VARIABLE_MESSAGE)

	// executeRawDatabaseCommand callback from dbmgr
	CELLAPP_MESSAGE_DECLARE_STREAM(onExecuteRawDatabaseCommandCB,					NETWORK_VARIABLE_MESSAGE)

	// Base requests to get celldata
	CELLAPP_MESSAGE_DECLARE_STREAM(reqBackupEntityCellData,							NETWORK_VARIABLE_MESSAGE)

	// Base requests to get WriteToDB
	CELLAPP_MESSAGE_DECLARE_STREAM(reqWriteToDBFromBaseapp,							NETWORK_VARIABLE_MESSAGE)

	// The client directly sends messages to the cell entity
	CELLAPP_MESSAGE_DECLARE_STREAM(forwardEntityMessageToCellappFromClient,			NETWORK_VARIABLE_MESSAGE)

	// Request to shut down the server
	CELLAPP_MESSAGE_DECLARE_STREAM(reqCloseServer,									NETWORK_VARIABLE_MESSAGE)

	// Request for watcher data
	CELLAPP_MESSAGE_DECLARE_STREAM(queryWatcher,									NETWORK_VARIABLE_MESSAGE)

	// Start profile
	CELLAPP_MESSAGE_DECLARE_STREAM(startProfile,									NETWORK_VARIABLE_MESSAGE)

	// Request to teleport to current cellapp
	CELLAPP_MESSAGE_DECLARE_STREAM(reqTeleportToCellApp,							NETWORK_VARIABLE_MESSAGE)

	// After the entity is teleported to the space on this destination cellapp, it returns the callback to the previous cellapp.
	CELLAPP_MESSAGE_DECLARE_STREAM(reqTeleportToCellAppCB,							NETWORK_VARIABLE_MESSAGE)

	// After telporting to cellapp, the baseapp needs to be set and then the cellapp record flag is cleared.
	// After that, cellapp can continue to use teleport.
	CELLAPP_MESSAGE_DECLARE_STREAM(reqTeleportToCellAppOver,						NETWORK_VARIABLE_MESSAGE)
		
	// Real request to update attributes to ghost
	CELLAPP_MESSAGE_DECLARE_STREAM(onUpdateGhostPropertys,							NETWORK_VARIABLE_MESSAGE)
	
	// Ghost request to call def method real
	CELLAPP_MESSAGE_DECLARE_STREAM(onRemoteRealMethodCall,							NETWORK_VARIABLE_MESSAGE)

	// Real entity requests to update volatile data to ghost
	CELLAPP_MESSAGE_DECLARE_STREAM(onUpdateGhostVolatileData,						NETWORK_VARIABLE_MESSAGE)

	// Request to kill the current app
	CELLAPP_MESSAGE_DECLARE_STREAM(reqKillServer,									NETWORK_VARIABLE_MESSAGE)

	// Tool request to change the space viewer (including add and delete functions)
	CELLAPP_MESSAGE_DECLARE_STREAM(setSpaceViewer,									NETWORK_VARIABLE_MESSAGE)

	//--------------------------------------------Entity----------------------------------------------------------
	// Remote call entity method
	ENTITY_MESSAGE_DECLARE_STREAM(onRemoteMethodCall,								NETWORK_VARIABLE_MESSAGE)

	// Client set new position
	ENTITY_MESSAGE_DECLARE_ARGS2(setPosition_XZ_int,								NETWORK_FIXED_MESSAGE,
									int32,											x, 
									int32,											z)
	
	// Client set new position
	ENTITY_MESSAGE_DECLARE_ARGS3(setPosition_XYZ_int,								NETWORK_FIXED_MESSAGE,
									int32,											x, 
									int32,											y, 
									int32,											z)

	// Client set new position
	ENTITY_MESSAGE_DECLARE_ARGS2(setPosition_XZ_float,								NETWORK_FIXED_MESSAGE,
									float,											x, 
									float,											z)

	// Client set new position
	ENTITY_MESSAGE_DECLARE_ARGS3(setPosition_XYZ_float,								NETWORK_FIXED_MESSAGE,
									float,											x, 
									float,											y, 
									float,											z)

	// The entity is bound to an observer (client)
	ENTITY_MESSAGE_DECLARE_ARGS0(onGetWitnessFromBase,								NETWORK_FIXED_MESSAGE)

	// Entity loses an observer (client)
	ENTITY_MESSAGE_DECLARE_ARGS0(onLoseWitness,										NETWORK_FIXED_MESSAGE)

	// Entity transfer.
	ENTITY_MESSAGE_DECLARE_ARGS3(teleportFromBaseapp,								NETWORK_FIXED_MESSAGE,
									COMPONENT_ID,									cellAppID,
									ENTITY_ID,										targetEntityID,
									COMPONENT_ID,									sourceBaseAppID)
NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
