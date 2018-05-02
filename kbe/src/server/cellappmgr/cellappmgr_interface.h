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
	#undef KBE_CELLAPPMGR_INTERFACE_H
#endif


#ifndef KBE_CELLAPPMGR_INTERFACE_H
#define KBE_CELLAPPMGR_INTERFACE_H

// common include	
#if defined(CELLAPPMGR)
#include "cellappmgr.h"
#endif
#include "cellappmgr_interface_macros.h"
#include "network/interface_defs.h"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	CELLAPPMGR all message interfaces are defined here
*/
NETWORK_INTERFACE_DECLARE_BEGIN(CellappmgrInterface)
	// An app registers its own interface address to this app
	CELLAPPMGR_MESSAGE_DECLARE_ARGS11(onRegisterNewApp,						NETWORK_VARIABLE_MESSAGE,
									int32,									uid, 
									std::string,							username,
									COMPONENT_TYPE,							componentType,
									COMPONENT_ID,							componentID, 
									COMPONENT_ORDER,						globalorderID,
									COMPONENT_ORDER,						grouporderID,
									uint32,									intaddr, 
									uint16,									intport,
									uint32,									extaddr, 
									uint16,									extport,
									std::string,							extaddrEx)

	// An app actively requests a look.
	CELLAPPMGR_MESSAGE_DECLARE_ARGS0(lookApp,								NETWORK_FIXED_MESSAGE)

	// An app requests to see the app load status.
	CELLAPPMGR_MESSAGE_DECLARE_ARGS0(queryLoad,								NETWORK_FIXED_MESSAGE)

	// An app informs this app that it is active.
	CELLAPPMGR_MESSAGE_DECLARE_ARGS2(onAppActiveTick,						NETWORK_FIXED_MESSAGE,
									COMPONENT_TYPE,							componentType, 
									COMPONENT_ID,							componentID)

	// The baseEntity requests to create cell entity in a new space.
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(reqCreateCellEntityInNewSpace,		NETWORK_VARIABLE_MESSAGE)

	// The baseEntity requests restore in space.
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(reqRestoreSpaceInCell,				NETWORK_VARIABLE_MESSAGE)

	// A message is forwarded by another app that wants to forward a message to an app through this one.
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(forwardMessage,						NETWORK_VARIABLE_MESSAGE)

	// Request to shut down the server
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(reqCloseServer,						NETWORK_VARIABLE_MESSAGE)

	// Request for watcher data
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(queryWatcher,							NETWORK_VARIABLE_MESSAGE)

	// Update cellapp information.
	CELLAPPMGR_MESSAGE_DECLARE_ARGS4(updateCellapp,							NETWORK_FIXED_MESSAGE,
									COMPONENT_ID,							componentID,
									ENTITY_ID,								numEntities,
									float,									load,
									uint32,									flags)

	// Start profile
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(startProfile,							NETWORK_VARIABLE_MESSAGE)

	// Request to kill the current app
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(reqKillServer,						NETWORK_VARIABLE_MESSAGE)

	// Cellapp synchronizes its own initialization information
	CELLAPPMGR_MESSAGE_DECLARE_ARGS4(onCellappInitProgress,					NETWORK_FIXED_MESSAGE,
									COMPONENT_ID,							cid,
									float,									progress,
									COMPONENT_ORDER,						componentGlobalOrder,
									COMPONENT_ORDER,						componentGroupOrder)

	// Query all relevant process load information
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(queryAppsLoads,						NETWORK_VARIABLE_MESSAGE)

	// Query all relevant process space information
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(querySpaces,							NETWORK_VARIABLE_MESSAGE)

	// Update the relevant process space information. Note: this spaceData is not the spaceData described in the API documentation
	// refers to some information about a space
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(updateSpaceData,						NETWORK_VARIABLE_MESSAGE)

	// Tool requests to change the space viewer (including add and delete functions)
	CELLAPPMGR_MESSAGE_DECLARE_STREAM(setSpaceViewer,						NETWORK_VARIABLE_MESSAGE)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
