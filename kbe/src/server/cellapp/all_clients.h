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


#ifndef KBE_ALL_CLIENTS_H
#define KBE_ALL_CLIENTS_H

#include "common/common.h"
#include "pyscript/scriptobject.h"
#include "entitydef/common.h"
#include "network/address.h"

//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <map>	
#include <vector>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#include <time.h> 
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

namespace Network
{
class Channel;
class Bundle;
}

class AllClients;
class ScriptDefModule;
class PropertyDescription;

class AllClientsComponent : public script::ScriptObject
{
	/** Subclassing Fill some py operations into derived classes */
	INSTANCE_SCRIPT_HREADER(AllClientsComponent, ScriptObject)
public:
	AllClientsComponent(PropertyDescription* pComponentPropertyDescription, AllClients* pAllClients);

	~AllClientsComponent();

	/**
	Script request get property or method
	*/
	PyObject* onScriptGetAttribute(PyObject* attr);

	/**
	Get the description of the object
	*/
	PyObject* tp_repr();
	PyObject* tp_str();

	void c_str(char* s, size_t size);

	ScriptDefModule* pComponentScriptDefModule();

protected:
	AllClients* pAllClients_;
	PropertyDescription* pComponentPropertyDescription_;
};

class AllClients : public script::ScriptObject
{
	/** Subclassing Fill some py operations into derived classes */
	INSTANCE_SCRIPT_HREADER(AllClients, ScriptObject)
public:
	AllClients(const ScriptDefModule* pScriptModule, 
		ENTITY_ID eid, 
		bool otherClients);
	
	~AllClients();
	
	/** 
		Script request get property or method
	*/
	PyObject* onScriptGetAttribute(PyObject* attr);						
			
	/** 
		Get the description of the object
	*/
	PyObject* tp_repr();
	PyObject* tp_str();
	
	void c_str(char* s, size_t size);
	
	/** 
		Get EntityId 
	*/
	ENTITY_ID id() const{ return id_; }
	void setID(int id){ id_ = id; }
	DECLARE_PY_GET_METHOD(pyGetID);

	void setScriptModule(const ScriptDefModule*	pScriptModule){ 
		pScriptModule_ = pScriptModule; 
	}

	bool isOtherClients() const {
		return otherClients_;
	}

protected:
	const ScriptDefModule*					pScriptModule_;			//The script module object used by this entity

	ENTITY_ID								id_;					// entityID

	bool									otherClients_;			// Is it just another client, not including yourself
};

}

#endif // KBE_ALL_CLIENTS_H
