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

#ifndef _SCENE_OBJ_H_FILE_
#define _SCENE_OBJ_H_FILE_

#include <ode/ode.h>

#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

//#include "config.h"
#include "transform.h"
#include "Primitive.h"

#include <boost/shared_ptr.hpp>

namespace sceneml {

//class SCENEML_API Geom;
//class SCENEML_API Body;

class Geom;
class Body;

typedef boost::shared_ptr<Geom> GeomPtr;
typedef boost::shared_ptr<Body> BodyPtr;

typedef std::vector< Body* > BodyList_t;
typedef std::vector< Geom* > GeomList_t;

typedef std::list< BodyPtr > BodyPtrList_t;
typedef std::list< GeomPtr > GeomPtrList_t;

typedef std::map<std::string, Body*> StringBodyMap_t;
typedef std::map<std::string, Geom*> StringGeomMap_t;

//! Base class for movable scene objects.
/** @ingroup xode 
 *  Base class for movalbe scene objects.
 */
//class SCENEML_API SceneObjectBase
class SceneObjectBase
{
	friend class Scene;
public:
	//! Basic destructor
	/** This class nor any of its derivatives should ever own any pointers its handed.
	  * Therefore clean up should not be an issue for this class. */
	virtual ~SceneObjectBase() {};
	//! Returns the objects position in world coordinates
	virtual void getGlobalPosition(dVector3 pos) const = 0;
	//! Returns the objects rotation matrix in world coordinates
	virtual void getGlobalRotation(dMatrix3 rot) const = 0;
	//! Returns the objects transformation matrix in world coordinates
	virtual void getGlobalTransformation(dMatrix4 T) const = 0;
	//! Set the objects position in world coordinates
	virtual void setGlobalPosition(const dVector3 pos) = 0;
	//! Set the objects rotation in world coordinates
	virtual void setGlobalRotation(const dMatrix3 rot) = 0;
	//! Notify this object that its world pose is invalid
	virtual void invalidate() = 0;
	//! Instruct object to re-validate its world pose
	virtual void validate() = 0;
	//! Returns the objects local position vector
	void getLocalPosition(dVector3 pos) const;
	//! Returns the objects local rotation matrix
	void getLocalRotation(dMatrix3 rot) const;
	//! Returns the objects local t-matrix
	void getLocalTransformation(dMatrix4 t) const;
	//! Adds a transform object.
	/** Notice that order which objects are added is extremely important. */
	//void addTransform(Transform *t) {transformList_.push_back(t);};
	void setCompositeTransform(CompositeTransformPtr t) {transform_ = t;};
	//! Get a pointer to the proximal object
	const SceneObjectBase* getProxObj() {return proxObj_;}
	//! Set the proximal object
	void setProxObj(SceneObjectBase* obj) {proxObj_ = obj;}
	//! Get wether the object's world pose is valid
	bool getPoseValidity() const {return validWorldPose_;}
	//! Get obj name
	std::string getName() const {return name_;}
protected:
	SceneObjectBase();
	virtual void computeLocalTransform();

	//! List of transform objects from the proximal object
	//TransformList_t transformList_;
	CompositeTransformPtr transform_;
	//! Objects name
	std::string name_;
	//! Hint to whether this object's world pose is valid
	bool validWorldPose_;
	//! Pointer to proximal object
	SceneObjectBase* proxObj_;
};

#define GEOM_COLLIDE_OFF	0
#define GEOM_COLLIDE_ON		1

//! Geometry class
/** @ingroup xode 
 *  The geometry class is used for any scene object has some non-zero volume. Notice that this class is not
 *  intended for general use as its constructor is protected. Object creation is provided by the Scene class.
 */
//class SCENEML_API Geom : public SceneObjectBase
class Geom : public SceneObjectBase
{
	friend class Scene;
public:
	virtual ~Geom();
	
	// All the following virtual function from the parent
	void getGlobalPosition(dVector3 pos) const;
	void getGlobalRotation(dMatrix3 rot) const;
	void getGlobalTransformation(dMatrix4 T) const;
	void setGlobalPosition(const dVector3 pos);
	void setGlobalRotation(const dMatrix3 rot);
	/** Notify the geom that its world pose is now invalid. A geom is always
	  * a leaf node in the model tree. Because it has no children, this
	  * call just changes validWorldPose_ to false */
	void invalidate() {validWorldPose_ = false;};
	void validate();
	
	// Methods specific to this class
	//! Get the geom type according to ODE
	int getType() const;
	dBodyID getBody() const;
	/** Returns the ODE pointer associated with this object. */
	dGeomID id() const {return geomID_;}
	/** Returns the transform ODE geom object associated with this object.
	  * Note that it is possible for this pointer to be NULL. */
	dGeomID tid() const {return transID_;}
	void setMesh(PolyhedronPtr p) {mesh_ = p;}
	const POLYHEDRON* getMesh() const {return mesh_.get();}
	void setColor(dReal r, dReal g, dReal b) {rgb_[0] = r; rgb_[0] = g; rgb_[0] = b;}
	void setColor(dVector3 rgb) { for(int n=0; n<3; ++n) rgb_[n] = rgb[n]; }
	const dReal* getColor() const {return rgb_;}
	void setAlpha(const dReal alpha) {alpha_ = alpha;}
	dReal getAlpha() const {return alpha_;}
	void setCollisionCheck( const int check ) { 
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		dGeomSetCategoryBits(gid, check); dGeomSetCollideBits(gid, check); 
	}
	int getCollisionCheck() const {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		return (int)dGeomGetCollideBits(gid);
	}
			
protected:
	Geom(dGeomID g, dGeomID t = NULL);
	void computeLocalTransform();

	//! ODE object pointer
	dGeomID geomID_;
	//! ODE object pointer
	dGeomID transID_;
	//! Geom mesh data
	PolyhedronPtr mesh_; // Owner of this pointer
	//! Geom color
	dVector3 rgb_;
	//! Geom alpha (transperancy)
	dReal alpha_;
};

//! Body class
/** @ingroup xode 
 *  The body class is used to model volumeless objects. Its really provided to model placeable coordinate systems.
 *  Notice that this class is not intended for general use as its constructor is protected. Object creation is provided 
 *  by the Scene class.
 */
//class SCENEML_API Body : public SceneObjectBase
class Body : public SceneObjectBase
{
	friend class Scene;
public:
	virtual ~Body();
	
	// All the following virtual function from the parent
	void getGlobalPosition(dVector3 pos) const;
	void getGlobalRotation(dMatrix3 rot) const;
	void getGlobalTransformation(dMatrix4 T) const;
	void setGlobalPosition(const dVector3 pos);
	void setGlobalRotation(const dMatrix3 rot);
	void invalidate();
	void validate();
	
	// Methods specific to this class
	//! Returns the body ODE pointer associated with this object.
	dBodyID id() const {return bodyID_;}
	//! Associate a distal body with this body
	void addDistBody(Body* body) {distBodies_.push_back(body);}
	//! Associate a geom with this body
	void addGeom(Geom* geom) {geomList_.push_back(geom);}
	/** Gets the number of distal bodies */
	int getNumDistBodies() {return distBodies_.size();}
	/** Gets the number of geoms */
	int getNumGeoms() {return geomList_.size();}
	/** Gets distal body assocaited with index n */
	Body* getDistBody(int n);
	/** Gets geom associated with index n */
	Geom* getGeom(int n);
protected:
	Body(dBodyID);
		
	//! List of distal bodies
	BodyList_t distBodies_;
	//! List of geoms
	GeomList_t geomList_;
	//! ODE object pointer
	dBodyID bodyID_;
};

}; // Namespace

#endif
