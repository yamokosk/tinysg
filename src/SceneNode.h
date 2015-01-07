/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "config.h"

#include <string>
#include <vector>
#include <memory>

#include <api/ObjectModel.h>

#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>
using namespace obrsp::linalg;

#include "threadpool.hpp"
#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>

#include <Visitor.h>

namespace tinysg
{

// Forward declaration
class SceneGraph;

enum TransformSpace
{
	/// Transform is relative to the local space
	TS_LOCAL,
	/// Transform is relative to the space of the parent node
	TS_PARENT,
	/// Transform is relative to world space
	TS_WORLD
};

class SceneNode : public Node
{
	friend class SceneGraph;

#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

	static unsigned long NextGeneratedNameExt;
public:
	static std::string generateName();

	typedef std::map<std::string, SceneNode*> ChildrenMap;
	typedef MapIterator<ChildrenMap> ChildIterator;

	typedef std::vector<SceneObject*> SceneObjectVector;
	typedef VectorIterator<SceneObjectVector> SceneObjectIterator;

	SceneNode(const std::string name);
	~SceneNode();

	// From Node
	void updatePose( const float* translation, const float* rotation );

	SceneNode* createChild();
	SceneNode* createChild(const std::string& name);
	void addChild(SceneNode* n);
	SceneNode* removeChild(SceneNode* child);
	SceneNode* removeChild(const std::string& name);
	void removeAllChildren();
	SceneNode* getChild(const std::string& name) const;
	ChildIterator getChildren();

	// Accessors
	SceneNode* getParent() const {return parent;}
	bool hasParent() const {return (parent != NULL);}

	const std::string& getName() const {return id;}

	void setPosition( const Vector3& p );
	void setOrientation( const obrsp::linalg::Quaternion& q );
	const Vector3& getPosition(TransformSpace space=TS_PARENT) const;
	const obrsp::linalg::Quaternion& getOrientation(TransformSpace space=TS_PARENT) const;

	void invalidate();
	void __deprecated_update();
	void update(boost::threadpool::pool& tp, bool forceUpdate = false);
	void doUpdate();

	// Object functions
	void attach(SceneObject* obj);
	SceneObjectIterator getAttachedObjects();
	unsigned int getNumAttachedObjects() const;

	// Generator functions
	void registerPoseGenerator(PoseGenerator*);
	const PoseGenerator* getPoseGenerator() {return mpPoseGenerator;}

	// Visitor functions
	virtual void accept(Visitor* v);

protected:
	void setParent(SceneNode* n);
	void setGraph(SceneGraph* g) {graph = g;};

private:
	//void notifyChild(SceneNode* child);
	SceneGraph* graph;
	SceneNode* parent;
	ChildrenMap children;
	SceneObjectVector attachedObjects;
	PoseGenerator* mpPoseGenerator;

	Vector3 position;
	obrsp::linalg::Quaternion orientation;
	mutable Vector3 derivedPosition;
	mutable obrsp::linalg::Quaternion derivedOrientation;

	mutable bool areDerivedCoordinatesValid;

	std::string id;

	boost::mutex mWriteMutex;
};


typedef std::auto_ptr<SceneNode> SceneNodePtr;

}
/*
// External includes
#include <string>
#include <set>
#include <queue>
#include <iostream>
#include <log4cxx/logger.h>

// Local includes
#include "Map.h"
#include "Object.h"
#include "MovableObject.h"
#include "PropertyCollection.h"

#include "Math.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace TinySG
{

class SceneGraph;


class SceneNode : public Object
{
	// For logging
	static log4cxx::LoggerPtr logger;

protected:
	typedef MAP<std::string, SceneNode*>		ChildNodeMap;
	typedef ChildNodeMap::iterator		ChildNodeIterator;
	typedef ChildNodeMap::const_iterator	ConstChildNodeIterator;

	typedef MAP<std::string, MovableObject*>	ObjectMap;
	typedef ObjectMap::iterator 		ObjectMapIterator;
	typedef ObjectMap::const_iterator	ObjectMapConstIterator;

public:
	static const uint8_t ParentChangedBit = 0x01;
	static const uint8_t PoseChangedBit = 0x02;
	static const uint8_t ScaleChangedBit = 0x04;
	static const uint8_t BoundsChangedBit = 0x08;
	static const std::string ObjectTypeID;

	enum TransformSpace
	{
		/// Transform is relative to the local space
		TS_LOCAL,
		/// Transform is relative to the space of the parent node
		TS_PARENT,
		/// Transform is relative to world space
		TS_WORLD
	};

	SceneNode();
	//explicit SceneNode(const std::string& name);
	virtual ~SceneNode();

	// From Object
	virtual Object* clone() const;

	void save(PropertyCollection& pc) const;

	// Child management
	//! Adds a (precreated) child scene node to this node.
	void addChild (SceneNode* child);
	//! Reports the number of child nodes under this one.
	unsigned short numChildren (void) const;
	//! Gets a pointer to a child node.
	SceneNode* getChild (unsigned short index) const;
	//! Gets a pointer to a named child node.
	SceneNode* getChild (const std::string& name) const;
	//! Drops the specified child from this node.
	SceneNode* removeChild (unsigned short index);
	//! Drops the specified child from this node.
	SceneNode* removeChild (SceneNode *child);
	//! Drops the named child from this node.
	SceneNode* removeChild (const std::string &name);
	//! Removes all child Nodes attached to this node.
	void removeAllChildren (void);
	//! Gets the parent of this SceneNode.
	SceneNode* getParent(void) const;
	//! Tells us if this node has a parent
	bool hasParent(void) const {return (parent_ != NULL);}
	//! Sets the parent of this node
	void setParent(SceneNode *parent);
	void setParent(const std::string& name);

	// Spatial management
	//! Sets the orientation of this node via a Quaternion.
	void setOrientation (const Quaternion &q);
	//! Sets the orientation of this node via Quaternion parameters.
	void setOrientation (Real w, Real x, Real y, Real z);
	//! Sets the position of the node relative to it's parent.
	void setPosition(const Vector3 &pos);
	//! Sets the position of the node relative to it's parent.
	void setPosition(Real x, Real y, Real z);
	//! Sets the scale of the node relative to it's parent.
	void setScale(const Vector3 &s);
	//! Sets the scale of the node relative to it's parent.
	void setScale(Real x, Real y, Real z);
	//! Returns a Quaternion representing the transform's orientation.
	const Quaternion& getOrientation(void) const {return orientation_;}
	//! Gets the position of the node relative to it's parent.
	const Vector3& getPosition(void) const {return position_;}
	//! Returns the scale associated with this transform
	const Vector3& getScale(void) const {return scale_;}
	//! Returns full 4x4 matrix representation of the transform
	const Matrix4& getFullTransform (void) const;
	//! Returns the parent's derived orientation
	const Quaternion& getParentOrientation() const;
	const Vector3& getParentPosition() const;
	const Vector3& getParentScale() const;
	//! Returns this node's derived orientation/position/scale
	const Quaternion& getDerivedOrientation();
	const Vector3& getDerivedPosition();
	const Vector3& getDerivedScale();

	//! Moves the node along the cartesian axes.
	void translate(const Vector3 &d, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along the cartesian axes.
	void translate(Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along arbitrary axes.
	void translate(const Matrix3& axes, const Vector3 &move, TransformSpace relativeTo=TS_PARENT);
	//! Moves the node along arbitrary axes.
	void translate(const Matrix3& axes, Real x, Real y, Real z, TransformSpace relativeTo=TS_PARENT);
	//! Rotate the node around an arbitrary axis.
	void rotate(const Vector3 &axis, Real angle, TransformSpace relativeTo=TS_LOCAL);
	//! Rotate the node around an aritrary axis using a Quarternion.
	void rotate(const Quaternion &q, TransformSpace relativeTo=TS_LOCAL);

	//! Adds an instance of a scene object to this node.
	void attachObject (MovableObject *obj);
	//! Reports the number of objects attached to this node.
	unsigned short numAttachedObjects (void) const;
	//! Retrieves a pointer to an attached object.
	MovableObject* getAttachedObject (unsigned short index);
	//! Retrieves a pointer to an attached object.
	MovableObject* getAttachedObject (const std::string &name);
	//! Detaches the indexed object from this scene node.
	MovableObject* detachObject (unsigned short index);
	//! Detaches an object by pointer.
	void detachObject (MovableObject *obj);
	//! Detaches the named object from this node and returns a pointer to it.
	MovableObject* detachObject (const std::string &name);
	//! Detaches all objects attached to this node.
	void detachAllObjects (void);

	// Update management
	virtual void update(uint8_t flags=0);
	virtual void notifyUpdate(uint8_t hint=0);

protected:
	void updateFromParent();
	//! Internal method called to update the cached transform
	void updateCachedTransform() const;

	//! Pointer to parent node.
	SceneNode* parent_;
	//! level in the tree
	unsigned int level_;
	//! Stores the orientation of the node relative to it's parent.
	Quaternion orientation_;
	//! Stores the position/translation of the node relative to its parent.
	Vector3 position_;
	//! Stores the scaling factor applied to this node.
	Vector3 scale_;
	//! Stores the orientation of the node relative to it's parent.
	Quaternion derivedOrientation_;
	//! Stores the position/translation of the node relative to its parent.
	Vector3 derivedPosition_;
	//! Stores the scaling factor applied to this node.
	Vector3 derivedScale_;
	//! Cached transform
	mutable Matrix4 cachedTransform_;
	//! Collection of pointers to direct children; hashmap for efficiency.
	ChildNodeMap children_;
	//! Scene objects attached to this node
	ObjectMap sceneObjects_;
	//! Whether or not this node has a valid world transform.
	bool validWorldTransform_;
	mutable bool cachedTransformOutOfDate_;
};

class SceneNodeFactory : public ObjectFactory
{
protected:
	// To be overloaded by specific object factories
	virtual Object* createInstanceImpl(const PropertyCollection* params = 0);

public:
	SceneNodeFactory(const std::string& type) : ObjectFactory(type) {};
	virtual ~SceneNodeFactory() {};

	// To be overloaded by specific object factories
	virtual void destroyInstance(Object* obj);
};

} // namespace TinySG

//ostream& operator << (ostream& os, const TinySG::SceneNode& s);
*/
#endif
