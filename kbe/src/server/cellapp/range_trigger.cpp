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

#include "range_trigger.h"
#include "coordinate_system.h"
#include "entity_coordinate_node.h"
#include "range_trigger_node.h"

#ifndef CODE_INLINE
#include "range_trigger.inl"
#endif

namespace KBEngine{	

//-------------------------------------------------------------------------------------
RangeTrigger::RangeTrigger(CoordinateNode* origin, float xz, float y):
range_xz_(fabs(xz)),
range_y_(fabs(y)),
origin_(origin),
positiveBoundary_(NULL),
negativeBoundary_(NULL),
removing_(false)
{
}

//-------------------------------------------------------------------------------------
RangeTrigger::~RangeTrigger()
{
	uninstall();
}

//-------------------------------------------------------------------------------------
bool RangeTrigger::reinstall(CoordinateNode* pCoordinateNode)
{
	uninstall();
	origin_ = pCoordinateNode;
	return install();
}

//-------------------------------------------------------------------------------------
bool RangeTrigger::install()
{
	if(positiveBoundary_ == NULL)
		positiveBoundary_ = new RangeTriggerNode(this, 0, 0, true);
	else
		positiveBoundary_->range(0.0f, 0.0f);

	if(negativeBoundary_ == NULL)
		negativeBoundary_ = new RangeTriggerNode(this, 0, 0, false);
	else
		negativeBoundary_->range(0.0f, 0.0f);

	positiveBoundary_->addFlags(COORDINATE_NODE_FLAG_INSTALLING);
	negativeBoundary_->addFlags(COORDINATE_NODE_FLAG_INSTALLING);

	origin_->pCoordinateSystem()->insert(positiveBoundary_);
	origin_->pCoordinateSystem()->insert(negativeBoundary_);
	
	/*
	Note: It must be first installed negativeBoundary_ and then install positiveBoundary_.
	 If the order is reversed, it will cause a View BUG. For example, an entity that enters View
	 is destroyed when an entity enterView is triggered.
	At this point, the entity was destroyed without triggering the exit View event, and the
	 destruction of the entity referenced in the View list of other entities without triggering 
	 the View event is an invalid pointer.

	The reason is as follows:
	Since it is always installed first in positiveBoundary_, and the boundary causes another entity to enter
	 the View during installation, then he may be destroyed in the process, and the other boundary negativeBoundary_ 
	 is not installed yet.
	When the node is deleted, the node's xx will be set to -FLT_MAX, leaving it to the negativeBoundary_ direction, so
	 the positiveBoundary_ can not check this boundary will not trigger the View exit event.
	*/
	negativeBoundary_->old_xx(-FLT_MAX);
	negativeBoundary_->old_yy(-FLT_MAX);
	negativeBoundary_->old_zz(-FLT_MAX);
	negativeBoundary_->range(-range_xz_, -range_y_);
	negativeBoundary_->old_range(-range_xz_, -range_y_);
	negativeBoundary_->update();
	negativeBoundary_->removeFlags(COORDINATE_NODE_FLAG_INSTALLING);

	// Update may cause the entity to destroy indirectly causing it to be reset, at which time the installation should fail
	if (!negativeBoundary_)
		return false;

	positiveBoundary_->old_xx(FLT_MAX);
	positiveBoundary_->old_yy(FLT_MAX);
	positiveBoundary_->old_zz(FLT_MAX);

	positiveBoundary_->range(range_xz_, range_y_);
	positiveBoundary_->old_range(range_xz_, range_y_);
	positiveBoundary_->update();
	positiveBoundary_->removeFlags(COORDINATE_NODE_FLAG_INSTALLING);

	return positiveBoundary_ != NULL;
}

//-------------------------------------------------------------------------------------
bool RangeTrigger::uninstall()
{
	if (removing_)
		return false;

	removing_ = true;
	if(positiveBoundary_ && positiveBoundary_->pCoordinateSystem())
	{
		positiveBoundary_->pCoordinateSystem()->remove(positiveBoundary_);
		positiveBoundary_->onTriggerUninstall();
	}

	if(negativeBoundary_ && negativeBoundary_->pCoordinateSystem())
	{
		negativeBoundary_->pCoordinateSystem()->remove(negativeBoundary_);
		negativeBoundary_->onTriggerUninstall();
	}
	
	// Node is not released here, the release of nodes is done in CoordinateSystem
	positiveBoundary_ = NULL;
	negativeBoundary_ = NULL;
	removing_ = false;
	return true;
}

//-------------------------------------------------------------------------------------
void RangeTrigger::onNodePassX(RangeTriggerNode* pRangeTriggerNode, CoordinateNode* pNode, bool isfront)
{
	if(pNode == origin())
		return;

	bool wasInZ = pRangeTriggerNode->wasInZRange(pNode);
	bool isInZ = pRangeTriggerNode->isInZRange(pNode);

	// If there is a change in the Z-axis condition, the Z-axis is evaluated again,
	//  and the priority is zyx so that only one enter or leave can be guaranteed.
	if(wasInZ != isInZ)
		return;

	bool wasIn = false;
	bool isIn = false;

	// It is necessary to check other axes at the same time. If the x axis of the node is within the range, 
	//  other axes are theoretically within the range.
	if(CoordinateSystem::hasY)
	{
		bool wasInY = pRangeTriggerNode->wasInYRange(pNode);
		bool isInY = pRangeTriggerNode->isInYRange(pNode);

		if(wasInY != isInY)
			return;

		wasIn = pRangeTriggerNode->wasInXRange(pNode) && wasInY && wasInZ;
		isIn = pRangeTriggerNode->isInXRange(pNode) && isInY && isInZ;
	}
	else
	{
		wasIn = pRangeTriggerNode->wasInXRange(pNode) && wasInZ;
		isIn = pRangeTriggerNode->isInXRange(pNode) && isInZ;
	}

	// If the situation does not change, ignore
	if(wasIn == isIn)
		return;

	if(isIn)
	{
		this->onEnter(pNode);
	}
	else
	{
		this->onLeave(pNode);
	}
}

//-------------------------------------------------------------------------------------
void RangeTrigger::onNodePassY(RangeTriggerNode* pRangeTriggerNode, CoordinateNode* pNode, bool isfront)
{
	if(pNode == origin() || !CoordinateSystem::hasY)
		return;

	bool wasInZ = pRangeTriggerNode->wasInZRange(pNode);
	bool isInZ = pRangeTriggerNode->isInZRange(pNode);

	// If there is a change in the Z-axis condition, the Z-axis is evaluated again,
	//  and the priority is zyx so that only one enter or leave can be guaranteed.
	if(wasInZ != isInZ)
		return;

	bool wasInY = pRangeTriggerNode->wasInYRange(pNode);
	bool isInY = pRangeTriggerNode->isInYRange(pNode);

	if(wasInY == isInY)
		return;

	// It is necessary to check other axes at the same time. If the x axis of the node is within the range, 
	//  other axes are theoretically within the range.
	bool wasIn = pRangeTriggerNode->wasInXRange(pNode) && wasInY && wasInZ;
	bool isIn = pRangeTriggerNode->isInXRange(pNode) && isInY && isInZ;

	if(wasIn == isIn)
		return;

	if(isIn)
	{
		this->onEnter(pNode);
	}
	else
	{
		this->onLeave(pNode);
	}
}

//-------------------------------------------------------------------------------------
void RangeTrigger::onNodePassZ(RangeTriggerNode* pRangeTriggerNode, CoordinateNode* pNode, bool isfront)
{
	if(pNode == origin())
		return;

	if(CoordinateSystem::hasY)
	{
		bool wasInZ = pRangeTriggerNode->wasInZRange(pNode);
		bool isInZ = pRangeTriggerNode->isInZRange(pNode);

		if(wasInZ == isInZ)
			return;

		bool wasIn = pRangeTriggerNode->wasInXRange(pNode) && 
			pRangeTriggerNode->wasInYRange(pNode) && 
			wasInZ;

		bool isIn = pRangeTriggerNode->isInXRange(pNode) && 
			pRangeTriggerNode->isInYRange(pNode) && 
			isInZ;

		if(wasIn == isIn)
			return;

		if(isIn)
		{
			this->onEnter(pNode);
		}
		else
		{
			this->onLeave(pNode);
		}
	}
	else
	{
		bool wasInZ = pRangeTriggerNode->wasInZRange(pNode);
		bool isInZ = pRangeTriggerNode->isInZRange(pNode);

		if(wasInZ == isInZ)
			return;

		bool wasIn = pRangeTriggerNode->wasInXRange(pNode) && wasInZ;
		bool isIn = pRangeTriggerNode->isInXRange(pNode) && isInZ;

		if(wasIn == isIn)
			return;

		if(isIn)
		{
			this->onEnter(pNode);
		}
		else
		{
			this->onLeave(pNode);
		}
	}
}

//-------------------------------------------------------------------------------------
void RangeTrigger::update(float xz, float y)
{
	float old_range_xz_ = range_xz_;
	float old_range_y_ = range_y_;

	range(xz, y);

	if (positiveBoundary_)
	{
		positiveBoundary_->range(range_xz_, range_y_);
		positiveBoundary_->old_range(old_range_xz_, old_range_y_);
		positiveBoundary_->update();
	}

	if (negativeBoundary_)
	{
		negativeBoundary_->range(-range_xz_, -range_y_);
		negativeBoundary_->old_range(-old_range_xz_, -old_range_y_);
		negativeBoundary_->update();
	}
}

//-------------------------------------------------------------------------------------
}
