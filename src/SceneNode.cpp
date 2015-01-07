/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version. The text of the GNU Lesser General
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for
 * more details.
 *
 *************************************************************************/
/*
 * SceneNode.cpp
 *
 *  Created on: Dec 2, 2008
 *      Author: yamokosk
 */

#include "SceneNode.h"
#include "SceneGraph.h"
#include "Command.h"

#include "NodeUtils.h"

#include <sstream>
#include <algorithm>

namespace tinysg
{

#if defined( TSG_HAVE_LOG4CXX )
log4cxx::LoggerPtr SceneNode::logger( log4cxx::Logger::getLogger("TinySG.SceneNode") );
#endif

unsigned long SceneNode::NextGeneratedNameExt(0);

std::string SceneNode::generateName()
{
	std::stringstream ss;
	ss << "Unnamed" << NextGeneratedNameExt++;
	return ss.str();
}

SceneNode::SceneNode(const std::string name) :
	graph(NULL),
	parent(NULL),
	mpPoseGenerator(NULL),
	position( Vector3::ZERO ),
	orientation( Quaternion::IDENTITY ),
	derivedPosition( Vector3::ZERO ),
	derivedOrientation( Quaternion::IDENTITY ),
	areDerivedCoordinatesValid(true),
	id(name)
{

}

SceneNode::~SceneNode()
{
	/*if (mpPoseGenerator != NULL)
	{
		mpPoseGenerator->detachNode(this);
	}*/
	removeAllChildren();
}

SceneNode* SceneNode::createChild()
{
	return createChild(generateName());
}

SceneNode* SceneNode::createChild(const std::string& name)
{
	// TODO: Remove getInstance() calls. To remove this one, need to store
	// pointer to graph who owns this node.
	//SceneGraph& sg = SceneGraph::getInstance();
	SceneNode* child = graph->createNode(name);
	TSG_LOG_DEBUG( "Node " << getName() << ": Created child " << child->getName() );

	addChild(child);
	return child;
}

void SceneNode::addChild(SceneNode* child)
{
	if ( child->hasParent() )
	{
		TSG_LOG_WARN( "Node " << getName() << ": Can't add child " << child->getName() << ". Already has a parent." );
		return;
	}

	if ( children.find( child->getName() ) != children.end())
	{
		TSG_LOG_WARN( "Node " << getName() << ": Can't add child " << child->getName() << ". Child with that name already exists!" );
		return;
	}

	child->setParent(this);
	children[child->getName()] = child;
	TSG_LOG_DEBUG( "Node " << getName() << ": Added child " << child->getName() );
}

/*void SceneNode::notifyChild(SceneNode* child)
{
	// Private method. Assumes child already knows we are its parent. Its
	// just being kind and letting us know. We will still make sure though
	// that we aren't already tracking this node as a child.
	if ( children.find(child->getName()) != children.end() )
	{
		// error
		return;
	}

	children[child->getName()] = child;
}*/

//! Drops the specified child from this node.
SceneNode* SceneNode::removeChild (SceneNode *child)
{
	if (child)
		removeChild( child->getName() );
	return child;
}

//! Drops the named child from this node.
SceneNode* SceneNode::removeChild (const std::string &name)
{
	ChildrenMap::iterator i = children.find(name);

	if (i == children.end())
	{
		TSG_LOG_WARN( "Node " << getName() << ": Can't remove child " << name << " because thats 'no baby of mine'." );
		return NULL;
	}

	TSG_LOG_DEBUG( "Node " << getName() << ": Removing child " << name << "." );

	SceneNode* ret = i->second;
	children.erase(i);
	ret->setParent(NULL);

	return ret;
}

void SceneNode::removeAllChildren()
{
	std::pair<std::string, SceneNode*> pair;
	BOOST_FOREACH(pair, children)
	{
		SceneNode* child = static_cast<SceneNode*>(pair.second);
		child->setParent(NULL);
	}
	children.clear();
}

//! Gets a pointer to a named child node.
SceneNode* SceneNode::getChild (const std::string& name) const
{
	ChildrenMap::const_iterator i = children.find(name);

	if (i == children.end())
	{
		//SML_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Child node named " + name + " does not exist.");
		return NULL;
	}
	return i->second;
}

SceneNode::ChildIterator SceneNode::getChildren()
{
	ChildIterator iter(children.begin(), children.end());
	return iter;
}

void SceneNode::setParent(SceneNode* n)
{
	if ( n != NULL )
	{
		TSG_LOG_DEBUG( "Node " << getName() << ": Setting parent to " << n->getName() );
		parent = n;
	}
	else
	{
		TSG_LOG_DEBUG( "Node " << getName() << ": Parent was set to null. I have been orphaned!" );
		parent = NULL;
	}
}

void SceneNode::updatePose( const float* translation, const float* rotation )
{
	if ( translation != NULL ) setPosition( Vector3(translation) );
	if ( rotation != NULL ) setOrientation( Quaternion(rotation) );
}

void SceneNode::setPosition( const Vector3& p )
{
	TSG_LOG_DEBUG( "Node " << getName() << ": Setting position: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" );
	position = p;
	invalidate();
}

void SceneNode::setOrientation( const Quaternion& q )
{
	TSG_LOG_DEBUG( "Node " << getName() << ": Setting orientation: (" << q[0] << ", " << q[1] << ", " << q[2] << ", " << q[3] << ")" );
	orientation = q;
	invalidate();
}

const Vector3& SceneNode::getPosition(TransformSpace relativeTo) const
{
	switch ( relativeTo )
	{
	case TS_LOCAL:
		return Vector3::ZERO;
	case TS_PARENT:
		return position;
	case TS_WORLD:
		return derivedPosition;
	}
	return position;
}

const Quaternion& SceneNode::getOrientation(TransformSpace relativeTo) const
{
	switch ( relativeTo )
	{
	case TS_LOCAL:
		return Quaternion::IDENTITY;
	case TS_PARENT:
		return orientation;
	case TS_WORLD:
		return derivedOrientation;
	}
	return orientation;
}

void SceneNode::invalidate()
{
	// Lock this object down.
	//boost::mutex::scoped_lock lock(mWriteMutex);

	areDerivedCoordinatesValid = false;
}

void SceneNode::__deprecated_update()
{
	TSG_LOG_DEBUG( "__deprecated_update() entered for " << getName() );

	if ( !areDerivedCoordinatesValid )
	{
		if ( hasParent()  )
		{
			TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose invalid. Recalculating from parent." );

			// Get parent orientation
			Quaternion q_parent = getParent()->getOrientation(TS_WORLD);
			Vector3 p_parent = getParent()->getPosition(TS_WORLD);

			// Update derived orientation
			derivedOrientation = q_parent * orientation;

			// Update derived position
			derivedPosition = q_parent * position + p_parent;
		}
		else
		{
			TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose invalid. Recalculating from self (no parent)." );

			// Root node, no parent
			derivedOrientation = orientation;
			derivedPosition = position;
		}

		areDerivedCoordinatesValid = true;
		TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose now valid." );

		std::stringstream matrixmsg;
		Matrix4 T = get_transform_matrix(this,TS_WORLD);
		matrixmsg << T[0][0] << ", " << T[0][1] << ", " << T[0][2] << ", " << T[0][3] << std::endl
				  << T[1][0] << ", " << T[1][1] << ", " << T[1][2] << ", " << T[1][3] << std::endl
				  << T[2][0] << ", " << T[2][1] << ", " << T[2][2] << ", " << T[2][3] << std::endl
				  << T[3][0] << ", " << T[3][1] << ", " << T[3][2] << ", " << T[3][3] << std::endl;
		TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose t-matrix: \n" << matrixmsg.str() );

		TSG_LOG_DEBUG( "Node " << getName() << ": Notifying " << attachedObjects.size() << " objects." );
		SceneObject* object=NULL;
		BOOST_FOREACH(object, attachedObjects)
		{
			object->notifyMoved(derivedPosition.ptr(), derivedOrientation.ptr());
		}
		TSG_LOG_DEBUG( "Node " << getName() << ": All objects notified." );

		TSG_LOG_DEBUG( "Node " << getName() << ": Invalidating pose of " << children.size() << " children." );
		std::pair<std::string, SceneNode*> pair;
		BOOST_FOREACH(pair, children)
		{
			SceneNode* child = static_cast<SceneNode*>(pair.second);
			child->invalidate();
		}
		TSG_LOG_DEBUG( "Node " << getName() << ": All child poses invalidated." );
	}

	TSG_LOG_DEBUG( "Node " << getName() << ": Calling update() for " << children.size() << " children." );
	std::pair<std::string, SceneNode*> pair;
	BOOST_FOREACH(pair, children)
	{
		SceneNode* child = static_cast<SceneNode*>(pair.second);
		child->__deprecated_update();
	}
	TSG_LOG_DEBUG( "Node " << getName() << ": Update complete." );
}

void SceneNode::update(boost::threadpool::pool& tp, bool forceUpdate)
{
	TSG_LOG_DEBUG( "update() entered for " << getName() );

	bool scheduleAnUpdate = (!areDerivedCoordinatesValid || forceUpdate);

	if ( scheduleAnUpdate )
	{
		// Schedule self for update
		TSG_LOG_DEBUG( "Node " << getName() << ": Scheduling self for update." );
		boost::threadpool::schedule(tp, boost::bind(&SceneNode::doUpdate, this));
	}

	TSG_LOG_DEBUG( "Node " << getName() << ": Calling update() of " << children.size() << " children." );
	std::pair<std::string, SceneNode*> pair;
	BOOST_FOREACH(pair, children)
	{
		SceneNode* child = static_cast<SceneNode*>(pair.second);
		child->update(tp, scheduleAnUpdate);
	}
	TSG_LOG_DEBUG( "Node " << getName() << ": All child poses invalidated." );
}

void SceneNode::doUpdate()
{
	// Lock this object down.
	//boost::mutex::scoped_lock lock(mWriteMutex);

	TSG_LOG_DEBUG( "Node " << getName() << ": Doing my update." );
	if ( hasParent()  )
	{
		TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose invalid. Recalculating from parent." );

		// Get parent orientation
		Quaternion q_parent = getParent()->getOrientation(TS_WORLD);
		Vector3 p_parent = getParent()->getPosition(TS_WORLD);

		// Update derived orientation
		derivedOrientation = q_parent * orientation;

		// Update derived position
		derivedPosition = q_parent * position + p_parent;
	}
	else
	{
		TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose invalid. Recalculating from self (no parent)." );

		// Root node, no parent
		derivedOrientation = orientation;
		derivedPosition = position;
	}

	areDerivedCoordinatesValid = true;
	TSG_LOG_DEBUG( "Node " << getName() << ": Derived pose now valid." );

	TSG_LOG_DEBUG( "Node " << getName() << ": Notifying " << attachedObjects.size() << " objects." );
	SceneObject* object=NULL;
	BOOST_FOREACH(object, attachedObjects)
	{
		// TODO: Threadpool this?
		object->notifyMoved(derivedPosition.ptr(), derivedOrientation.ptr());
	}
	TSG_LOG_DEBUG( "Node " << getName() << ": All objects notified." );
}

void SceneNode::attach(SceneObject* obj)
{
	// Add obj to the node's list of attached objects
	attachedObjects.push_back(obj);

	// Then notify the obj that it needs to update its spatial coordinates
	obj->notifyMoved(derivedPosition.ptr(), derivedOrientation.ptr());
}

SceneObjectIterator SceneNode::getAttachedObjects()
{
	SceneObjectIterator iter(attachedObjects.begin(), attachedObjects.end());
	return iter;
}

unsigned int SceneNode::getNumAttachedObjects() const
{
	return (unsigned int)attachedObjects.size();
}

void SceneNode::registerPoseGenerator(PoseGenerator* pGenerator)
{
	if ( mpPoseGenerator != NULL )
	{
		mpPoseGenerator->detachNode(this);
	}

	pGenerator->attachNode(this);
	mpPoseGenerator = pGenerator;
}

void SceneNode::accept(Visitor& visitor)
{
	visitor.visit(this);
}

}
