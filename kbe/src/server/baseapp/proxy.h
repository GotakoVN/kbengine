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


#ifndef KBE_PROXY_H
#define KBE_PROXY_H
	
#include "entity.h"
#include "data_downloads.h"
#include "common/common.h"
#include "helper/debug_helper.h"
#include "network/address.h"
#include "network/message_handler.h"
	
namespace KBEngine{


namespace Network
{
class Channel;
}

class ProxyForwarder;

#define LOG_ON_REJECT  0
#define LOG_ON_ACCEPT  1
#define LOG_ON_WAIT_FOR_DESTROY 2

class Proxy : public Entity
{
	/** Subclassing fills some py operations into derived classes */
	BASE_SCRIPT_HREADER(Proxy, Entity)

public:
	Proxy(ENTITY_ID id, const ScriptDefModule* pScriptModule);
	~Proxy();
	
	INLINE void addr(const Network::Address& address);
	INLINE const Network::Address& addr() const;

	typedef std::vector<Network::Bundle*> Bundles;
	bool pushBundle(Network::Bundle* pBundle);

	/**
		Push a message to the witness client
	*/
	bool sendToClient(const Network::MessageHandler& msgHandler, Network::Bundle* pBundle);
	bool sendToClient(Network::Bundle* pBundle);
	bool sendToClient(bool expectData = true);

	/** 
		Script requests the RTT value of the connection
	*/
	double getRoundTripTime() const;
	DECLARE_PY_GET_MOTHOD(pyGetRoundTripTime);

	/** 
		This is the number of seconds since a packet from the client was last received. 
	*/
	double getTimeSinceHeardFromClient() const;
	DECLARE_PY_GET_MOTHOD(pyGetTimeSinceHeardFromClient);

	/** 
		The script asks if there is a client bound to the proxy
	*/
	bool hasClient() const;
	DECLARE_PY_GET_MOTHOD(pyHasClient);

	/** 
		Script request for client address
	*/
	DECLARE_PY_GET_MOTHOD(pyClientAddr);

	/** 
		Whether the entity is available
	*/
	INLINE bool clientEnabled() const;
	DECLARE_PY_GET_MOTHOD(pyGetClientEnabled);

	/**
		The entity is activated. After the client initializes its corresponding entity, this method is called.
	*/
	void onClientEnabled(void);
	
	/**
		A data download task is completed
	*/
	void onStreamComplete(int16 id, bool success);

	/**
		Login attempt, after normal login fails, call this interface and try again
	*/
	int32 onLogOnAttempt(const char* addr, uint32 port, const char* password);
	
	/**
		Initialize the properties of the client proxy
	*/
	void initClientBasePropertys();
	void initClientCellPropertys();

	/** 
		Detected when the client socket corresponding to this entity is found to be disconnected
	*/
	void onClientDeath(void);
	
	/** Network interface
		Called when the cell associated with this entity is created
	*/
	void onClientGetCell(Network::Channel* pChannel, COMPONENT_ID componentID);

	/**
		Get client type (mobile, win/mac/linux, mini)
	*/
	INLINE COMPONENT_CLIENT_TYPE getClientType() const;
	INLINE void setClientType(COMPONENT_CLIENT_TYPE ctype);
	DECLARE_PY_MOTHOD_ARG0(pyGetClientType);

	/**
		Disconnect the client
	*/
	DECLARE_PY_MOTHOD_ARG0(pyDisconnect);

	/**
		Get Front-End data
	*/
	INLINE const std::string& getLoginDatas();
	INLINE void setLoginDatas(const std::string& datas);
	
	INLINE const std::string& getCreateDatas();
	INLINE void setCreateDatas(const std::string& datas);

	DECLARE_PY_MOTHOD_ARG0(pyGetClientDatas);

	/**
		After each proxy is created, the system generates a UUID that provides the front-end login for identification
	*/
	INLINE uint64 rndUUID() const;
	INLINE void rndUUID(uint64 uid);

	/** 
		Transfer its own associated client to another proxy to associate
	*/
	void giveClientTo(Proxy* proxy);
	void onGiveClientTo(Network::Channel* lpChannel);
	void onGiveClientToFailure();
	DECLARE_PY_MOTHOD_ARG1(pyGiveClientTo, PyObject_ptr);

	/**
		File stream data download
	*/
	static PyObject* __py_pyStreamFileToClient(PyObject* self, PyObject* args);
	int16 streamFileToClient(PyObjectPtr objptr, 
		const std::string& descr = "", int16 id = -1);

	/**
		String stream data download
	*/
	static PyObject* __py_pyStreamStringToClient(PyObject* self, PyObject* args);
	int16 streamStringToClient(PyObjectPtr objptr, 
		const std::string& descr = "", int16 id = -1);

	/**
		Bind witness
	*/
	void onGetWitness();

	/**
		Kick the client out of the server
	*/
	void kick();

	/**
		Get this proxy's client connection object
	*/
	Network::Channel* pChannel();

protected:
	uint64 rndUUID_;
	Network::Address addr_;
	DataDownloads dataDownloads_;

	bool clientEnabled_;

	// Limit the bandwidth per second that the client can use
	int32 bandwidthPerSecond_;

	// Communication encryption key Default blowfish
	std::string encryptionKey;

	ProxyForwarder* pProxyForwarder_;

	COMPONENT_CLIENT_TYPE clientComponentType_;

	// Datas attached to login data (not archived)
	std::string loginDatas_;

	// Datas attached to registration data (permanent archive)
	std::string createDatas_;
};

}


#ifdef CODE_INLINE
#include "proxy.inl"
#endif

#endif // KBE_PROXY_H
