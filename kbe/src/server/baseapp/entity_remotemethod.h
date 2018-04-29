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

#ifndef KBE_BASE_REMOTE_METHOD_H
#define KBE_BASE_REMOTE_METHOD_H


#include "helper/debug_helper.h"
#include "common/common.h"	
#include "entitydef/remote_entity_method.h"

namespace KBEngine{

class EntityRemoteMethod : public RemoteEntityMethod
{
	/** Subclasses populate a derived class with some py operations */
	INSTANCE_SCRIPT_HREADER(EntityRemoteMethod, RemoteEntityMethod)	
public:
	EntityRemoteMethod(MethodDescription* methodDescription, 
						EntityCallAbstract* entityCall);

	~EntityRemoteMethod();

	static PyObject* tp_call(PyObject* self, 
			PyObject* args, PyObject* kwds);
private:

};

}
#endif
