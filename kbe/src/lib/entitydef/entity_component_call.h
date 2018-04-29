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


#ifndef KBE_ENTITY_COMPONENT_CELL_BASE_CLIENT__CALL_H
#define KBE_ENTITY_COMPONENT_CELL_BASE_CLIENT__CALL_H
	
#include "common/common.h"
#include "pyscript/scriptobject.h"
#include "entitydef/entitycallabstract.h"
	
#ifdef KBE_SERVER
#include "server/components.h"
#endif

	
namespace KBEngine{

namespace Network
{
class Channel;
}

class EntityCall;
class ScriptDefModule;
class RemoteEntityMethod;
class MethodDescription;
class PropertyDescription;

class EntityComponentCall : public EntityCallAbstract
{
	/** 子类化 将一些py操作填充进派生类 */
	INSTANCE_SCRIPT_HREADER(EntityComponentCall, EntityCallAbstract)
public:
	typedef std::tr1::function<RemoteEntityMethod* (MethodDescription* pMethodDescription, EntityComponentCall* pEntityCall)> EntityCallCallHookFunc;
	typedef std::tr1::function<Network::Channel* (EntityComponentCall&)> FindChannelFunc;

	EntityComponentCall(EntityCall* pEntityCall, PropertyDescription* pComponentPropertyDescription);

	~EntityComponentCall();

	virtual ENTITYCALL_CLASS entitycallClass() const {
		return ENTITYCALL_CLASS_ENTITY_COMPONENT;
	}

	/** 
		脚本请求获取属性或者方法 
	*/
	PyObject* onScriptGetAttribute(PyObject* attr);						

	/** 
		获得对象的描述 
	*/
	PyObject* tp_repr();
	PyObject* tp_str();
	
	void c_str(char* s, size_t size);

	/** 
		脚本被安装时被调用 
	*/
	static void onInstallScript(PyObject* mod);

	virtual RemoteEntityMethod* createRemoteMethod(MethodDescription* pMethodDescription);

	/**
		unpickle方法
	*/
	static PyObject* __unpickle__(PyObject* self, PyObject* args);

	ScriptDefModule* pComponentScriptDefModule();

	virtual void newCall(Network::Bundle& bundle);

protected:
	EntityCall*								pEntityCall_;
	PropertyDescription*					pComponentPropertyDescription_;
};

}
#endif // KBE_ENTITY_COMPONENT_CELL_BASE_CLIENT__CALL_H
