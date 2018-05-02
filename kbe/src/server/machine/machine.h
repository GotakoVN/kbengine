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

#ifndef KBE_MACHINE_H
#define KBE_MACHINE_H
	
// common include	
#include "server/kbemain.h"
#include "server/serverapp.h"
#include "server/idallocate.h"
#include "server/serverconfig.h"
#include "server/components.h"
#include "common/timer.h"
#include "network/endpoint.h"
#include "network/udp_packet_receiver.h"
#include "network/common.h"

//#define NDEBUG
#include <map>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class Machine:	public ServerApp, 
				public Singleton<Machine>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK = TIMEOUT_SERVERAPP_MAX + 1
	};
	
	Machine(Network::EventDispatcher& dispatcher, 
		Network::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Machine();
	
	bool run();
	
	bool findBroadcastInterface();

	/** Network interface
		An app broadcasts its own address
	*/
	void onBroadcastInterface(Network::Channel* pChannel, int32 uid, std::string& username, 
							COMPONENT_TYPE componentType, COMPONENT_ID componentID, COMPONENT_ID componentIDEx, 
							COMPONENT_ORDER globalorderid, COMPONENT_ORDER grouporderid, COMPONENT_GUS gus,
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport, std::string& extaddrEx, uint32 pid,
							float cpu, float mem, uint32 usedmem, int8 state, uint32 machineID, uint64 extradata,
							uint64 extradata1, uint64 extradata2, uint64 extradata3, uint32 backRecvAddr, uint16 backRecvPort);
	
	/** Network interface
		An app looks for another app's address
	*/
	void onFindInterfaceAddr(Network::Channel* pChannel, int32 uid, std::string& username, 
		COMPONENT_TYPE componentType, COMPONENT_ID componentID, COMPONENT_TYPE findComponentType, uint32 finderAddr, uint16 finderRecvPort);

	/** Network interface
		Query all interface information
	*/
	void onQueryAllInterfaceInfos(Network::Channel* pChannel, int32 uid, std::string& username, 
		uint16 finderRecvPort);

	/** Network interface
		Query all machine processes
	*/
	void onQueryMachines(Network::Channel* pChannel, int32 uid, std::string& username,
		uint16 finderRecvPort);

	void handleTimeout(TimerHandle handle, void * arg);

	/* Initialization related interfaces */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();
	bool initNetwork();

	/** Network interface
		Start the server
		@uid: Provide start uid parameters
		@components: Which components to start (may take a distributed startup scheme)
	*/
	void startserver(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Signal processing
	*/
	virtual bool installSignals();
	virtual void onSignalled(int sigNum);

#if KBE_PLATFORM != PLATFORM_WIN32
	/**
	* Start a new process under Linux
	*/
	uint16 startLinuxProcess(int32 uid, COMPONENT_TYPE componentType, uint64 cid, int16 gus, 
		std::string& KBE_ROOT, std::string& KBE_RES_PATH, std::string& KBE_BIN_PATH);
#else
	/**
	* Start a new process under windows
	*/
	DWORD startWindowsProcess(int32 uid, COMPONENT_TYPE componentType, uint64 cid, int16 gus, 
		std::string& KBE_ROOT, std::string& KBE_RES_PATH, std::string& KBE_BIN_PATH);
#endif

	/** Network interface
		Shut down the server
		@uid: Provide start uid parameters
	*/
	void stopserver(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/** Network interface
	Kill the server
	@uid: Provide start uid parameters
	*/
	void killserver(Network::Channel* pChannel, KBEngine::MemoryStream& s);

	/**
		Check whether the components running on the machine are available
	*/
	bool checkComponentUsable(const Components::ComponentInfos* info, bool getdatas, bool autoerase);

protected:
	// UDP broadcast address
	u_int32_t					broadcastAddr_;
	Network::EndPoint			ep_;
	Network::EndPoint			epBroadcast_;

	Network::EndPoint			epLocal_;

	Network::UDPPacketReceiver* pEPPacketReceiver_;
	Network::UDPPacketReceiver* pEBPacketReceiver_;
	Network::UDPPacketReceiver* pEPLocalPacketReceiver_;

	// Native uid
	std::vector<int32>			localuids_;
};

}

#endif // KBE_MACHINE_H
