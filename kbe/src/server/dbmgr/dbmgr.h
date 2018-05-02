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

#ifndef KBE_DBMGR_H
#define KBE_DBMGR_H

#include "db_interface/db_threadpool.h"
#include "buffered_dbtasks.h"
#include "server/kbemain.h"
#include "pyscript/script.h"
#include "pyscript/pyobject_pointer.h"
#include "entitydef/entitydef.h"
#include "server/python_app.h"
#include "server/idallocate.h"
#include "server/serverconfig.h"
#include "server/globaldata_client.h"
#include "server/globaldata_server.h"
#include "common/timer.h"
#include "network/endpoint.h"
#include "resmgr/resmgr.h"
#include "thread/threadpool.h"


namespace KBEngine{

class DBInterface;
class TelnetServer;
class InterfacesHandler;
class SyncAppDatasHandler;
class UpdateDBServerLogHandler;

class Dbmgr :	public PythonApp, 
				public Singleton<Dbmgr>
{
public:
	enum TimeOutType
	{
		TIMEOUT_TICK = TIMEOUT_PYTHONAPP_MAX + 1,
		TIMEOUT_CHECK_STATUS
	};
	
	Dbmgr(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Dbmgr();
	
	bool run();
	
	void handleTimeout(TimerHandle handle, void * arg);
	void handleMainTick();
	void handleCheckStatusTick();

	/* Initialize related interfaces */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();
	void onInstallPyModules();
	
	bool initInterfacesHandler();

	bool initDB();

	virtual bool canShutdown();

	virtual void onShutdownBegin();
	virtual void onShutdownEnd();

	/** Get ID server pointer */
	IDServer<ENTITY_ID>& idServer(void){ return idServer_; }

	/** Network interface
		Request to allocate a ENTITY_ID segment
	*/
	void onReqAllocEntityID(Network::Channel* pChannel, COMPONENT_ORDER componentType, COMPONENT_ID componentID);

	/* Network interface
	   Register a newly activated baseapp or cellapp or dbmgr
	   Usually a new app is started and it needs to register itself with certain components.
	*/
	virtual void onRegisterNewApp(Network::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							COMPONENT_TYPE componentType, COMPONENT_ID componentID, COMPONENT_ORDER globalorderID, COMPONENT_ORDER grouporderID,
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport, std::string& extaddrEx);


	/** Network interface
		Dbmgr broadcast global data changes
	*/
	void onGlobalDataClientLogon(Network::Channel* pChannel, COMPONENT_TYPE componentType);
	void onBroadcastGlobalDataChanged(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** Network interface
		Request to create an account
	*/
	void reqCreateAccount(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void onCreateAccountCBFromInterfaces(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Request to erase client request task
	*/
	void eraseClientReq(Network::Channel* pChannel, std::string& logkey);

	/** Network interface
		A new user login, need to check the legality
	*/
	void onAccountLogin(Network::Channel* pChannel, KBEngine::MemoryStream& s);
	void onLoginAccountCBBFromInterfaces(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Baseapp requests query account information
	*/
	void queryAccount(Network::Channel* pChannel, std::string& accountName, std::string& password, bool needCheckPassword,
		COMPONENT_ID componentID, ENTITY_ID entityID, DBID entityDBID, uint32 ip, uint16 port);

	/** Network interface
		Entity automatic loading function
	*/
	void entityAutoLoad(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		The account is online from baseapp
	*/
	void onAccountOnline(Network::Channel* pChannel, std::string& accountName, 
		COMPONENT_ID componentID, ENTITY_ID entityID);

	/** Network interface
		Entity-baseapp is offline
	*/
	void onEntityOffline(Network::Channel* pChannel, DBID dbid, ENTITY_SCRIPT_UID sid, uint16 dbInterfaceIndex);

	/** Network interface
		Perform database queries
	*/
	void executeRawDatabaseCommand(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Archive entity
	*/
	void writeEntity(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Delete the archive data of an entity
	*/
	void removeEntity(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Remove the callback of an entity from the database by dbid
	*/
	void deleteEntityByDBID(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Check if an entity is checked out from the database by dbid
	*/
	void lookUpEntityByDBID(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Request to get all data of entity from db
	*/
	void queryEntity(Network::Channel* pChannel, uint16 dbInterfaceIndex, COMPONENT_ID componentID, int8	queryMode, DBID dbid, 
		std::string& entityType, CALLBACK_ID callbackID, ENTITY_ID entityID);

	/** Network interface
		synchronize entity stream template
	*/
	void syncEntityStreamTemplate(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	virtual bool initializeWatcher();

	/** Network interface
		Request recharge
	*/
	void charge(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
		Recharge callback
	*/
	void onChargeCB(Network::Channel* pChannel, KBEngine::MemoryStream& s);


	/** Network interface
		Account activate callback
	*/
	void accountActivate(Network::Channel* pChannel, std::string& scode);

	/** Network interface
		Account reset password
	*/
	void accountReqResetPassword(Network::Channel* pChannel, std::string& accountName);
	void accountResetPassword(Network::Channel* pChannel, std::string& accountName, 
		std::string& newpassword, std::string& code);

	/** Network interface
		Account binding email
	*/
	void accountReqBindMail(Network::Channel* pChannel, ENTITY_ID entityID, std::string& accountName, 
		std::string& password, std::string& email);
	void accountBindMail(Network::Channel* pChannel, std::string& username, std::string& scode);

	/** Network interface
		Account change password
	*/
	void accountNewPassword(Network::Channel* pChannel, ENTITY_ID entityID, std::string& accountName, 
		std::string& password, std::string& newpassword);
	
	SyncAppDatasHandler* pSyncAppDatasHandler() const { return pSyncAppDatasHandler_; }
	void pSyncAppDatasHandler(SyncAppDatasHandler* p){ pSyncAppDatasHandler_ = p; }

	std::string selectAccountDBInterfaceName(const std::string& name);

	Buffered_DBTasks* findBufferedDBTask(const std::string& dbInterfaceName)
	{
		BUFFERED_DBTASKS_MAP::iterator dbin_iter = bufferedDBTasksMaps_.find(dbInterfaceName);
		if (dbin_iter == bufferedDBTasksMaps_.end())
			return NULL;

		return &dbin_iter->second;
	}

protected:
	TimerHandle											loopCheckTimerHandle_;
	TimerHandle											mainProcessTimer_;

	// entityID allocation server
	IDServer<ENTITY_ID>									idServer_;

	// globalData
	GlobalDataServer*									pGlobalData_;

	// baseAppData
	GlobalDataServer*									pBaseAppData_;

	// cellAppData
	GlobalDataServer*									pCellAppData_;

	typedef KBEUnordered_map<std::string, Buffered_DBTasks> BUFFERED_DBTASKS_MAP;
	BUFFERED_DBTASKS_MAP								bufferedDBTasksMaps_;

	// Statistics
	uint32												numWrittenEntity_;
	uint32												numRemovedEntity_;
	uint32												numQueryEntity_;
	uint32												numExecuteRawDatabaseCommand_;
	uint32												numCreatedAccount_;

	InterfacesHandler*									pInterfacesAccountHandler_;
	InterfacesHandler*									pInterfacesChargeHandler_;

	SyncAppDatasHandler*								pSyncAppDatasHandler_;
	UpdateDBServerLogHandler*							pUpdateDBServerLogHandler_;
	
	TelnetServer*										pTelnetServer_;
};

}

#endif // KBE_DBMGR_H
