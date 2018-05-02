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


#ifndef KBE_LOGINAPP_H
#define KBE_LOGINAPP_H
	
// common include	
#include "server/kbemain.h"
#include "server/serverapp.h"
#include "server/idallocate.h"
#include "server/serverconfig.h"
#include "server/pendingLoginmgr.h"
#include "server/python_app.h"
#include "common/timer.h"
#include "network/endpoint.h"
	
namespace KBEngine{

class HTTPCBHandler;
class TelnetServer;

class Loginapp :	public PythonApp, 
					public Singleton<Loginapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_TICK = TIMEOUT_PYTHONAPP_MAX + 1
	};

	Loginapp(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Loginapp();
	
	bool run();
	
	virtual void onChannelDeregister(Network::Channel * pChannel);

	virtual void handleTimeout(TimerHandle handle, void * arg);
	void handleMainTick();

	/* Initialization related functions */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();
	void onInstallPyModules();
	
	virtual void onShutdownBegin();
	virtual void onShutdownEnd();

	virtual void onHello(Network::Channel* pChannel, 
		const std::string& verInfo, 
		const std::string& scriptVerInfo, 
		const std::string& encryptedKey);

	/** Network interface
		A client informs the app that it is active.
	*/
	void onClientActiveTick(Network::Channel* pChannel);

	/** Network interface
		Create account
	*/
	bool _createAccount(Network::Channel* pChannel, std::string& accountName, 
		std::string& password, std::string& datas, ACCOUNT_TYPE type = ACCOUNT_TYPE_NORMAL);
	void reqCreateAccount(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Create account w/ email
	*/
	void reqCreateMailAccount(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Create account callback
	*/
	void onReqCreateAccountResult(Network::Channel* pChannel, MemoryStream& s);
	void onReqCreateMailAccountResult(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Reset account password application (forgot password?)
	*/
	void reqAccountResetPassword(Network::Channel* pChannel, std::string& accountName);
	void onReqAccountResetPasswordCB(Network::Channel* pChannel, std::string& accountName, std::string& email,
		SERVER_ERROR_CODE failedcode, std::string& code);

	/** Network interface
		Dbmgr account activation returns
	*/
	void onAccountActivated(Network::Channel* pChannel, std::string& code, bool success);

	/** Network interface
		Dbmgr account binding email back
	*/
	void onAccountBindedEmail(Network::Channel* pChannel, std::string& code, bool success);

	/** Network interface
		Dbmgr account reset password return
	*/
	void onAccountResetPassword(Network::Channel* pChannel, std::string& code, bool success);

	/** Network interface
	Baseapp request binding e-mail (return to need to find loginapp address)
	*/
	void onReqAccountBindEmailAllocCallbackLoginapp(Network::Channel* pChannel, COMPONENT_ID reqBaseappID, ENTITY_ID entityID, std::string& accountName, std::string& email,
		SERVER_ERROR_CODE failedcode, std::string& code);

	/** Network interface
		User login to server
		clientType[COMPONENT_CLIENT_TYPE]: Client type(mobile， web， pcexe)
		clientData[str]: Front end with data (can be arbitrary, such as the phone model, browser type, etc.)
		accountName[str]: Username
		password[str]: password
	*/
	void login(Network::Channel* pChannel, MemoryStream& s);

	/*
		Login failed
		failedcode: 失败返回码 NETWORK_ERR_SRV_NO_READY: The server is not ready
									NETWORK_ERR_SRV_OVERLOAD: The server is overloaded
									NETWORK_ERR_NAME_PASSWORD: incorrect username or password
	*/
	void _loginFailed(Network::Channel* pChannel, std::string& loginName, 
		SERVER_ERROR_CODE failedcode, std::string& datas, bool force = false);
	
	/** Network interface
		Login account test results returned by dbmgr
	*/
	void onLoginAccountQueryResultFromDbmgr(Network::Channel* pChannel, MemoryStream& s);

	/** Network interface
		Login account test results returned by dbmgr
	*/
	void onLoginAccountQueryBaseappAddrFromBaseappmgr(Network::Channel* pChannel, std::string& loginName, 
		std::string& accountName, std::string& addr, uint16 port);


	/** Network interface
		Dbmgr sends initial information
		startGlobalOrder: The global startup sequence includes a variety of different components
		startGroupOrder: The start order in the group, such as the first few start in all baseapps.
	*/
	void onDbmgrInitCompleted(Network::Channel* pChannel, COMPONENT_ORDER startGlobalOrder, 
		COMPONENT_ORDER startGroupOrder, const std::string& digest);

	/** Network interface
		Client protocol export
	*/
	void importClientMessages(Network::Channel* pChannel);

	/** Network interface
		Error code description export
	*/
	void importServerErrorsDescr(Network::Channel* pChannel);

	// Engine version does not match
	virtual void onVersionNotMatch(Network::Channel* pChannel);

	// Engine script layer version does not match
	virtual void onScriptVersionNotMatch(Network::Channel* pChannel);

	/** Network interface
		Baseapp synchronizes its own initialization information
		startGlobalOrder: The global startup sequence includes a variety of different components
		startGroupOrder: The start order in the group, such as the first few start in all baseapps.
	*/
	void onBaseappInitProgress(Network::Channel* pChannel, float progress);

protected:
	TimerHandle							mainProcessTimer_;

	// Logs a request for a registered account that has not yet logged in
	PendingLoginMgr						pendingCreateMgr_;

	// Logs accounts that have logged in to the server but have not been processed yet
	PendingLoginMgr						pendingLoginMgr_;

	std::string							digest_;

	HTTPCBHandler*						pHttpCBHandler;

	float								initProgress_;
	
	TelnetServer*						pTelnetServer_;
};

}

#endif // KBE_LOGINAPP_H
