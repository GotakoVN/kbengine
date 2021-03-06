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

#ifndef KBE_UPDATABLE_H
#define KBE_UPDATABLE_H

// common include
#include "helper/debug_helper.h"
#include "common/common.h"

// #define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32	
#else
// linux include
#endif

namespace KBEngine{

/*
	Used to describe an object that will always be updated. Each tick will be called by the app.
	Updatable to update the state, need to implement different Updatable to complete different update features.
*/
class Updatable
{
public:
	Updatable();
	~Updatable();

	virtual bool update() = 0;

	virtual uint8 updatePriority() const {
		return 0;
	}

	std::string c_str() { return updatableName; }

	// Its own position in the Updatables container
	int removeIdx;

	std::string updatableName;
};

}
#endif
