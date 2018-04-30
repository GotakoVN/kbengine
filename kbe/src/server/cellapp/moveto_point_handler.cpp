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

#include "cellapp.h"
#include "entity.h"
#include "moveto_point_handler.h"	
#include "move_controller.h"	

namespace KBEngine{	


//-------------------------------------------------------------------------------------
MoveToPointHandler::MoveToPointHandler(KBEShared_ptr<Controller>& pController, int layer, const Position3D& destPos, 
											 float velocity, float distance, bool faceMovement, 
											bool moveVertically, PyObject* userarg):
destPos_(destPos),
velocity_(velocity),
faceMovement_(faceMovement),
moveVertically_(moveVertically),
pyuserarg_(userarg),
distance_(distance),
pController_(pController),
layer_(layer),
isDestroyed_(false)
{
	updatableName = "MoveToPointHandler";

	Py_INCREF(userarg);

	//std::static_pointer_cast<MoveController>(pController)->pMoveToPointHandler(this);
	static_cast<MoveController*>(pController.get())->pMoveToPointHandler(this);
	Cellapp::getSingleton().addUpdatable(this);
}

//-------------------------------------------------------------------------------------
MoveToPointHandler::MoveToPointHandler():
destPos_(),
velocity_(0.f),
faceMovement_(false),
moveVertically_(false),
pyuserarg_(NULL),
distance_(0.f),
layer_(0),
isDestroyed_(false)
{
	updatableName = "MoveToPointHandler";

	Cellapp::getSingleton().addUpdatable(this);
}

//-------------------------------------------------------------------------------------
MoveToPointHandler::~MoveToPointHandler()
{
	if(pyuserarg_ != NULL)
	{
		Py_DECREF(pyuserarg_);
	}

	//DEBUG_MSG(fmt::format("MoveToPointHandler::~MoveToPointHandler(): {}\n", (void*)this));
}

//-------------------------------------------------------------------------------------
void MoveToPointHandler::addToStream(KBEngine::MemoryStream& s)
{
	// uint8 utype = type();

	s << /*utype <<*/ destPos_.x << destPos_.y << destPos_.z << velocity_ << faceMovement_ << moveVertically_ <<
		distance_ << layer_;

	s.appendBlob(script::Pickler::pickle(pyuserarg_));
}

//-------------------------------------------------------------------------------------
void MoveToPointHandler::createFromStream(KBEngine::MemoryStream& s)
{
	s >> /*utype <<*/ destPos_.x >> destPos_.y >> destPos_.z >> velocity_ >> faceMovement_ >> moveVertically_ >>
		distance_ >> layer_;

	std::string val = "";
	s.readBlob(val);

	pyuserarg_ = script::Pickler::unpickle(val);
}

//-------------------------------------------------------------------------------------
bool MoveToPointHandler::requestMoveOver(const Position3D& oldPos)
{
	if(pController_)
	{
		if(pController_->pEntity())
			pController_->pEntity()->onMoveOver(pController_->id(), layer_, oldPos, pyuserarg_);

		// Calling cancelController(id) in onMoveOver causes the MoveController to destruct and causes pController_ to be NULL
		pController_->destroy();
	}

	return true;
}

//-------------------------------------------------------------------------------------
bool MoveToPointHandler::update()
{
	if (isDestroyed_)
	{
		delete this;
		return false;
	}
	
	Entity* pEntity = pController_->pEntity();
	Py_INCREF(pEntity);

	const Position3D& dstPos = destPos();
	Position3D currpos = pEntity->position();
	Position3D currpos_backup = currpos;
	Direction3D direction = pEntity->direction();

	Vector3 movement = dstPos - currpos;
	if (!moveVertically_) movement.y = 0.f;
	
	bool ret = true;
	float dist_len = KBEVec3Length(&movement);

	if (dist_len < velocity_ + distance_)
	{
		float y = currpos.y;

		if (distance_ > 0.0f)
		{
			// Unit vector
			KBEVec3Normalize(&movement, &movement); 
				
			if(dist_len > distance_)
			{
				movement *= distance_;
				currpos = dstPos - movement;
			}
		}
		else
		{
			currpos = dstPos;
		}

		if (!moveVertically_)
			currpos.y = y;

		ret = false;
	}
	else
	{
		// Unit vector
		KBEVec3Normalize(&movement, &movement); 

		// Move Location
		movement *= velocity_;
		currpos += movement;
	}
	
	// Should change the orientation?
	if (faceMovement_)
	{
		if (movement.x != 0.f || movement.z != 0.f)
			direction.yaw(movement.yaw());

		if (movement.y != 0.f)
			direction.pitch(movement.pitch());
	}
	
	// Set the new location and orientation of the entity
	if(!isDestroyed_)
		pEntity->setPositionAndDirection(currpos, direction);

	// Non-navigate cannot be sure to be on the ground
	if(!isDestroyed_)
		pEntity->isOnGround(isOnGround());

	// Notify script
	if(!isDestroyed_)
		pEntity->onMove(pController_->id(), layer_, currpos_backup, pyuserarg_);

	// If it was stopped in the onMove process, or reached the destination, it will be destroyed and returned false
	if (isDestroyed_ || 
		(!ret && requestMoveOver(currpos_backup)))
	{
		Py_DECREF(pEntity);
		delete this;
		return false;
	}

	Py_DECREF(pEntity);
	return true;
}

//-------------------------------------------------------------------------------------
}

