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

#include "sceneobjects.h"
#include "matrix.h"

using namespace sceneml;

// --------------------------------------------------------------
//	SceneObjectBase
// --------------------------------------------------------------
/** Basic constructor */
SceneObjectBase::SceneObjectBase() :
	name_("_NODATA_"),
	validWorldPose_(false),
	proxObj_(NULL)
{
	//dTSetIdentity(tmatrix_);
}

/** Returns the objects local position vector */
void SceneObjectBase::getLocalPosition(dVector3 pos) const
{
	const dReal *tmatrix = transform_->getTMatrix();
	for (int n=0; n<3; ++n) pos[n] = tmatrix[(n*4)+3];
}

/** Returns the objects local rotation matrix */
void SceneObjectBase::getLocalRotation(dMatrix3 rot) const
{
	const dReal *tmatrix = transform_->getTMatrix();
	dRFromT(rot, tmatrix);
}

void SceneObjectBase::getLocalTransformation(dMatrix4 t) const
{	
	dAASSERT(t);
	const dReal *tmatrix = transform_->getTMatrix();
	memcpy(t, tmatrix, sizeof(dMatrix4));
}
	
/** Compute the local transformation.
  * Computes the prox transformation matrix by stepping through the objects
  * local transformation steps. */
void SceneObjectBase::computeLocalTransform()
{
	transform_->compute();
}

void SceneObjectBase::invalidate()
{
	validWorldPose_ = false;
}

// --------------------------------------------------------------
//	Geom
// --------------------------------------------------------------
//! Basic constructor
Geom::Geom(dGeomID g, dGeomID t) :
	SceneObjectBase(),
	geomID_(g),
	transID_(t)
{
	for (int n=0; n<3; ++n) rgb_[n] = 1.0;
	dGeomID gid = NULL;
	if (t != NULL) gid = t;
	else gid = g;
	dGeomSetCategoryBits(gid, 1);
	dGeomSetCollideBits(gid, 1);
}

//! Basic destructor
Geom::~Geom() 
{
	//if (mesh_ != NULL) delete mesh_;
	
	//dGeomDestroy(id());
	//if (tid()) dGeomDestroy(tid());
}

//! Returns the objects position in world coordinates (accordind to ODE)
void Geom::getGlobalPosition(dVector3 pos) const
{
	if (getType() != dPlaneClass) {
		if (tid() == NULL) {
			// Transform ID does not exist so just return the real geoms position
			memcpy(pos, dGeomGetPosition(id()), sizeof(dVector3));
		} else {
			// Need to do a little math...
			//	P_wcs_g = R_wcs_t * P_t_tg + P_wcs_t;
			memcpy(pos, dGeomGetPosition( tid() ), sizeof(dVector3));
			dMULTIPLYADD0_331(pos, dGeomGetRotation(tid()), dGeomGetPosition(id()));
		}
	} else memset(pos, 0, sizeof(dVector3));
}

//! Returns the objects rotation matrix in world coordinates (according to ODE)
void Geom::getGlobalRotation(dMatrix3 rot) const
{
	if (getType() != dPlaneClass) {
		if (tid() == NULL) {
			// Transform ID does not exist so just return the real geoms position
			memcpy(rot, dGeomGetRotation(id()), sizeof(dMatrix3));
		} else {
			// Need to do a little math...
			//	R_wcs_g = R_wcs_t * R_t_g;
			dMULTIPLY0_333(rot, dGeomGetRotation(tid()), dGeomGetRotation(id()) );
		}
	} else dRSetIdentity(rot);
}

//! Returns the objects transformation matrix in world coordinates (according to ODE)
void Geom::getGlobalTransformation(dMatrix4 T) const
{
	if (getType() != dPlaneClass) {
		dMatrix3 rot; dVector3 pos;
		getGlobalRotation(rot);
		getGlobalPosition(pos);
		dTFromRAndPos(T, rot, pos);
	} else dTSetIdentity(T);
}

/** Sets the objects position in world coordinates. */
void Geom::setGlobalPosition(const dVector3 pos)
{
	if (getType() != dPlaneClass) {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		//dGeomSetPosition(gid, pos[0], pos[1], pos[2]);
	}
}

/** Sets the objects position in world coordinates. */
void Geom::setGlobalRotation(const dMatrix3 rot)
{
	if (getType() != dPlaneClass) {
		dGeomID gid = NULL;
		if (tid() != NULL) gid = tid();
		else gid = id();
		//dGeomSetRotation(gid, rot);
	}
}

/** Gets the ODE body associated with this geom */
dBodyID Geom::getBody() const
{
	if (getType() != dPlaneClass) {
		if (tid() != NULL)
			return dGeomGetBody(tid());
		else
			return dGeomGetBody(id());
	} else return NULL;
}

int Geom::getType() const
{
	return dGeomGetClass(geomID_);
}

/** Instructs the geom to re-validate its world pose. */
void Geom::validate()
{
	if ( getPoseValidity() || getType() == dPlaneClass ) {
		// My world pose has not been invalidated by anyone, therefore there 
		// is nothing to do and I can just return.
		return;
	} else {
		// Someone has invalidated my local transformation, therefore I need
		// to compute a new one, then update my world transformation by asking
		// my prox body for his.
		computeLocalTransform(); // Should only ever have to do this once!
		/*Body* prox = (Body*)getProxObj();
		
		if (prox != NULL ) {
			prox->validate(); // Make sure his is also valid (essentially sets up recursion)
			
			dVector3 P_prox_obj, P_wcs_prox, P_wcs_obj;
			dMatrix3 R_prox_obj, R_wcs_prox, R_wcs_obj;
			
			// Get some data..
			prox->getGlobalPosition(P_wcs_prox);
			prox->getGlobalRotation(R_wcs_prox);
			this->getLocalPosition(P_prox_obj);
			this->getLocalRotation(R_prox_obj);
			
			// .. and do some calculations
			memcpy(P_wcs_obj, P_wcs_prox, sizeof(dVector3));
			dMULTIPLYADD0_331(P_wcs_obj, R_wcs_prox, P_prox_obj);
			dMULTIPLY0_333(R_wcs_obj, R_wcs_prox, R_prox_obj);
			
			// Inform ODE of the Body changes
			this->setGlobalPosition(P_wcs_obj);
			this->setGlobalRotation(R_wcs_obj);
		}*/
		validWorldPose_ = true; // Don't forget to set this!
	}
}

/** Compute the local transformation.
  * This function is overloaded from its base class. ODE is keeping track of
  * each objects global position but when we have a transformed geom, we need
  * to tell ODE about changes in the transformation between the regular and
  * transformed geom. */
void Geom::computeLocalTransform()
{
	this->SceneObjectBase::computeLocalTransform();
	
	if (tid() != NULL)
	{
		dMatrix3 rot;
		this->getLocalRotation(rot);
		dGeomSetRotation(id(), rot);
		
		dVector3 pos;
		this->getLocalPosition(pos);
		dGeomSetPosition(id(), pos[0], pos[1], pos[2]);
	}
}

// --------------------------------------------------------------
//	Body
// --------------------------------------------------------------
/** Basic constructor */
Body::Body(dBodyID b) :
	SceneObjectBase(),
	bodyID_(b)
{}

/** Basic destructor */
Body::~Body()
{
	//if (id() != NULL) dBodyDestroy(id());
}

//! Returns the objects position in world coordinates
void Body::getGlobalPosition(dVector3 pos) const
{
	if (id() != NULL) {
		memcpy(pos, dBodyGetPosition(id()), sizeof(dVector3));
	} else {
		memset(pos, 0, sizeof(dVector3));
	}
}

//! Returns the objects rotation matrix in world coordinates
void Body::getGlobalRotation(dMatrix3 rot) const
{
	if (id() != NULL) {
		memcpy(rot, dBodyGetRotation(id()), sizeof(dMatrix3));
	} else {
		dRSetIdentity(rot);
	}
}

void Body::getGlobalTransformation(dMatrix4 T) const
{
	if (id() != NULL) {
		dMatrix3 rot; dVector3 pos;
		getGlobalRotation(rot);
		getGlobalPosition(pos);
		dTFromRAndPos(T, rot, pos);
	} else dTSetIdentity(T);
}

//! Sets the objects position in world coordinates
void Body::setGlobalPosition(const dVector3 pos)
{
	if (id() != NULL) dBodySetPosition(id(), pos[0], pos[1], pos[2]);
}

//! Sets the objects rotation matrix in world coordinates
void Body::setGlobalRotation(const dMatrix3 rot)
{
	if (id() != NULL) dBodySetRotation(id(), rot);
}

/** Notify the body that its world pose is now invalid. A body can have
  * any number of distal children. So this function also notifies all the
  * children that they are invalid as well. */
void Body::invalidate()
{
	BodyList_t::iterator bi = distBodies_.begin();
	for (; bi != distBodies_.end(); ++bi) (*bi)->invalidate();

	GeomList_t::iterator gi = geomList_.begin();
	for (; gi != geomList_.end(); ++gi) (*gi)->invalidate();
	
	validWorldPose_ = false;
}

/** Instructs the body to re-validate its world pose. */
void Body::validate()
{
	// My world pose has not been invalidated by anyone, therefore there 
	// is nothing to do and I can just return.
	if (!getPoseValidity()) {
		// Someone has invalidated my local transformation, therefore I need
		// to compute a new one, then update my world transformation by asking
		// my prox body for his.
//		std::cout << this->name_ << ": Computing local transform" << std::endl;
		computeLocalTransform();
		Body* prox = (Body*)getProxObj();
		
		if (prox != NULL ) {
			prox->validate(); // Make sure his is also valid (essentially sets up recursion)
			dVector3 P_prox_obj, P_wcs_prox, P_wcs_obj;
			dMatrix3 R_prox_obj, R_wcs_prox, R_wcs_obj;
			
			// Get some data..
			prox->getGlobalPosition(P_wcs_prox);
			prox->getGlobalRotation(R_wcs_prox);
			this->getLocalPosition(P_prox_obj);
			this->getLocalRotation(R_prox_obj);
			
			// .. and do some calculations
			memcpy(P_wcs_obj, P_wcs_prox, sizeof(dVector3));
			dMULTIPLYADD0_331(P_wcs_obj, R_wcs_prox, P_prox_obj);
			dMULTIPLY0_333(R_wcs_obj, R_wcs_prox, R_prox_obj);
			
			// Inform ODE of the Body changes
			this->setGlobalPosition(P_wcs_obj);
			this->setGlobalRotation(R_wcs_obj);
		}
		validWorldPose_ = true; // Don't forget to set this!
	}
}

Body* Body::getDistBody(int n)
{
	//BodyList_t::iterator it = distBodies_.begin();
	//for (int i=0; i < n; ++n) it++;
	//return (*it);

	if (n < distBodies_.size()) {
		return distBodies_[n];
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Index exceeded number of dist bodies associated with this body.";
		throw std::runtime_error(msg.str());
	}
}

Geom* Body::getGeom(int n)
{
	//GeomList_t::iterator it = geomList_.begin();
	//for (int i=0; i < n; ++n) it++;
	//return (*it);

	if (n < geomList_.size()) {
		return geomList_[n];
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Index exceeded number of geoms associated with this body.";
		throw std::runtime_error(msg.str());
	}
}
