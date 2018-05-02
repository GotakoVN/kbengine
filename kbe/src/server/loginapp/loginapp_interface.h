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
	#undef KBE_LOGINAPP_INTERFACE_H
#endif


#ifndef KBE_LOGINAPP_INTERFACE_H
#define KBE_LOGINAPP_INTERFACE_H

// common include	
#if defined(LOGINAPP)
#include "loginapp.h"
#endif
#include "loginapp_interface_macros.h"
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
	LOGINAPP all message interfaces are defined here
*/
NETWORK_INTERFACE_DECLARE_BEGIN(LoginappInterface)
	// Client protocol export.
	LOGINAPP_MESSAGE_EXPOSED(importClientMessages)
	LOGINAPP_MESSAGE_DECLARE_ARGS0(importClientMessages,							NETWORK_FIXED_MESSAGE)

	// Error code description export.
	LOGINAPP_MESSAGE_EXPOSED(importServerErrorsDescr)
	LOGINAPP_MESSAGE_DECLARE_ARGS0(importServerErrorsDescr,							NETWORK_FIXED_MESSAGE)

	// An app actively requests a disconnection.
	LOGINAPP_MESSAGE_DECLARE_ARGS0(reqClose,										NETWORK_FIXED_MESSAGE)

	// An app actively requests a look.
	LOGINAPP_MESSAGE_DECLARE_ARGS0(lookApp,											NETWORK_FIXED_MESSAGE)

	// Another app requests to see this app load status.
	LOGINAPP_MESSAGE_DECLARE_ARGS0(queryLoad,										NETWORK_FIXED_MESSAGE)

	// Hello handshaking.
	NETWORK_MESSAGE_EXPOSED(Loginapp, hello)
	LOGINAPP_MESSAGE_DECLARE_STREAM(hello,											NETWORK_VARIABLE_MESSAGE)

	// Client informs this app that it is active.
	LOGINAPP_MESSAGE_EXPOSED(onClientActiveTick)
	LOGINAPP_MESSAGE_DECLARE_ARGS0(onClientActiveTick,								NETWORK_FIXED_MESSAGE)
	
	// Request to create an account
	LOGINAPP_MESSAGE_EXPOSED(reqCreateAccount)
	LOGINAPP_MESSAGE_DECLARE_STREAM(reqCreateAccount,								NETWORK_VARIABLE_MESSAGE)

	LOGINAPP_MESSAGE_EXPOSED(reqCreateMailAccount)
	LOGINAPP_MESSAGE_DECLARE_STREAM(reqCreateMailAccount,							NETWORK_VARIABLE_MESSAGE)

	// Reset account password application
	LOGINAPP_MESSAGE_EXPOSED(reqAccountResetPassword)
	LOGINAPP_MESSAGE_DECLARE_ARGS1(reqAccountResetPassword,							NETWORK_VARIABLE_MESSAGE,
									std::string,									accountName)

	// Callback for Reset Account Password Request
	LOGINAPP_MESSAGE_DECLARE_ARGS4(onReqAccountResetPasswordCB,						NETWORK_VARIABLE_MESSAGE,
									std::string,									accountName,
									std::string,									email,
									SERVER_ERROR_CODE,								failedcode,
									std::string,									code)
	// User login into server
	LOGINAPP_MESSAGE_EXPOSED(login)
	LOGINAPP_MESSAGE_DECLARE_STREAM(login,											NETWORK_VARIABLE_MESSAGE)

	// An app requests a callback for an entityID segment
	LOGINAPP_MESSAGE_DECLARE_ARGS3(onDbmgrInitCompleted,							NETWORK_VARIABLE_MESSAGE,
									COMPONENT_ORDER,								startGlobalOrder,
									COMPONENT_ORDER,								startGroupOrder,
									std::string,									digest)

	// Another app informs this app that it is active.
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,									NETWORK_FIXED_MESSAGE,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID)

	// Query from dbmgr to user legality result
	LOGINAPP_MESSAGE_DECLARE_STREAM(onLoginAccountQueryResultFromDbmgr,				NETWORK_VARIABLE_MESSAGE)

	// Login address returned by baseappmgr
	LOGINAPP_MESSAGE_DECLARE_ARGS4(onLoginAccountQueryBaseappAddrFromBaseappmgr,	NETWORK_VARIABLE_MESSAGE,
									std::string,									loginName, 
									std::string,									accountName,
									std::string,									addr,
									uint16,											port)

	// Dbmgr request to create an account to return results
	LOGINAPP_MESSAGE_DECLARE_STREAM(onReqCreateAccountResult,						NETWORK_VARIABLE_MESSAGE)
	LOGINAPP_MESSAGE_DECLARE_STREAM(onReqCreateMailAccountResult,					NETWORK_VARIABLE_MESSAGE)

	// Dbmgr account activation returns
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onAccountActivated,								NETWORK_VARIABLE_MESSAGE,
									std::string,									code, 
									bool,											success)
	
	// Dbmgr account binding email back
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onAccountBindedEmail,							NETWORK_VARIABLE_MESSAGE,
									std::string,									code, 
									bool,											success)

	// Dbmgr account reset password return
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onAccountResetPassword,							NETWORK_VARIABLE_MESSAGE,
									std::string,									code, 
									bool,											success)

	// Baseapp requests to bind e-mail (return to need to find loginapp address)
	LOGINAPP_MESSAGE_DECLARE_ARGS6(onReqAccountBindEmailAllocCallbackLoginapp,		NETWORK_VARIABLE_MESSAGE,
									COMPONENT_ID,									reqBaseappID,
									ENTITY_ID,										entityID,
									std::string,									accountName,
									std::string,									email,
									SERVER_ERROR_CODE,								failedcode,
									std::string,									code)

	// Request to shut down the server
	LOGINAPP_MESSAGE_DECLARE_STREAM(reqCloseServer,									NETWORK_VARIABLE_MESSAGE)


	// Request for watcher data
	LOGINAPP_MESSAGE_DECLARE_STREAM(queryWatcher,									NETWORK_VARIABLE_MESSAGE)

	// Baseapp synchronizes its own initialization information
	LOGINAPP_MESSAGE_DECLARE_ARGS1(onBaseappInitProgress,							NETWORK_FIXED_MESSAGE,
									float,											progress)

	// Start profile
	LOGINAPP_MESSAGE_DECLARE_STREAM(startProfile,									NETWORK_VARIABLE_MESSAGE)

	// Request to kill the current app
	LOGINAPP_MESSAGE_DECLARE_STREAM(reqKillServer,									NETWORK_VARIABLE_MESSAGE)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
