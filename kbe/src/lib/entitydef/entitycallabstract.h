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


#ifndef KBE_ENTITYCALL_BASE_H
#define KBE_ENTITYCALL_BASE_H
	
#include "common/common.h"
//#include "network/channel.h"
#include "pyscript/scriptobject.h"
#include "entitydef/common.h"
#include "network/address.h"

#ifdef KBE_SERVER
#include "server/components.h"
#endif

namespace KBEngine{

class ScriptDefModule;
class RemoteEntityMethod;
class MethodDescription;

namespace Network
{
class Channel;
class Bundle;
}

class EntityCallAbstract : public script::ScriptObject
{
	/** 子类化 将一些py操作填充进派生类 */
	INSTANCE_SCRIPT_HREADER(EntityCallAbstract, ScriptObject)
public:
	EntityCallAbstract(PyTypeObject* scriptType, 
		const Network::Address* pAddr, 
		COMPONENT_ID componentID, 
		ENTITY_ID eid, 
		uint16 utype, 
		ENTITYCALL_TYPE type);
	
	virtual ~EntityCallAbstract();

	typedef std::tr1::function<RemoteEntityMethod* (MethodDescription* pMethodDescription, EntityCallAbstract* pEntityCall)> EntityCallCallHookFunc;
	typedef std::tr1::function<Network::Channel* (EntityCallAbstract&)> FindChannelFunc;

	enum ENTITYCALL_CLASS
	{
		ENTITYCALL_CLASS_ENTITY,
		ENTITYCALL_CLASS_ENTITY_COMPONENT,
	};

	/**
		设置entityCall的__findChannelFunc函数地址
	*/
	static void setFindChannelFunc(FindChannelFunc func) {
		__findChannelFunc = func;
	};

	/**
		设置entityCall的__hookCallFunc函数地址
	*/
	static void setEntityCallCallHookFunc(EntityCallCallHookFunc* pFunc) {
		__hookCallFuncPtr = pFunc;
	};

	static void resetCallHooks() {
		__hookCallFuncPtr = NULL;
		__findChannelFunc = FindChannelFunc();
	}

	virtual ENTITYCALL_CLASS entitycallClass() const {
		return ENTITYCALL_CLASS_ENTITY;
	}

	/** 
		获取entityID 
	*/
	INLINE ENTITY_ID id() const;

	INLINE void id(int v);

	DECLARE_PY_GET_METHOD(pyGetID);

	/** 
		获得组件ID 
	*/
	INLINE COMPONENT_ID componentID(void) const;

	/** 
		设置组件的ID 
	*/
	INLINE void componentID(COMPONENT_ID cid);

	/** 
		获得utype 
	*/
	INLINE ENTITY_SCRIPT_UID utype(void) const;

	/** 
		获得type 
	*/
	INLINE ENTITYCALL_TYPE type(void) const;

	/** 
		支持pickler 方法 
	*/
	static PyObject* __py_reduce_ex__(PyObject* self, PyObject* protocol);
	
	virtual Network::Channel* getChannel(void);

	virtual bool sendCall(Network::Bundle* pBundle);

	virtual void newCall_(Network::Bundle& bundle);
	virtual void newCall(Network::Bundle& bundle);

	const Network::Address& addr() const{ return addr_; }
	void addr(const Network::Address& saddr){ addr_ = saddr; }

	INLINE bool isClient() const;
	INLINE bool isCell() const;
	INLINE bool isCellReal() const;
	INLINE bool isCellViaBase() const;
	INLINE bool isBase() const;
	INLINE bool isBaseReal() const;
	INLINE bool isBaseViaCell() const;
	
protected:
	COMPONENT_ID							componentID_;			// 远端机器组件的ID
	Network::Address						addr_;					// 频道地址
	ENTITYCALL_TYPE							type_;					// 该entityCall的类型
	ENTITY_ID								id_;					// entityID
	ENTITY_SCRIPT_UID						utype_;					// entity的utype按照entities.xml中的定义顺序

	static EntityCallCallHookFunc*			__hookCallFuncPtr;
	static FindChannelFunc					__findChannelFunc;
};

}

#ifdef CODE_INLINE
#include "entitycallabstract.inl"
#endif
#endif // KBE_ENTITYCALL_BASE_H
 