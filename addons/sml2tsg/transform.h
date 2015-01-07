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

#ifndef _SCENEML_TRANSFORM_H_FILE_
#define _SCENEML_TRANSFORM_H_FILE_

//#include "config.h"

#include <ode/ode.h>
#include <vector>
#include <string>
#include <list>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/math/quaternion.hpp>

namespace sceneml {

// Forward declarations
class CoordinateTransform;
class SimpleTransform;
class MarkerTransform;
class CompositeTransform;

// Typedefs
typedef boost::shared_ptr<CoordinateTransform> CoordinateTransformPtr;
typedef boost::shared_ptr<CompositeTransform> CompositeTransformPtr;
typedef boost::shared_ptr<SimpleTransform> SimpleTransformPtr;
typedef std::list< CoordinateTransformPtr > CoordinateTransformList_t;
typedef boost::shared_ptr<dReal> dRealPtr;
typedef std::vector< dRealPtr > dRealVector_t;

// Class definitions
class CoordinateTransform
{
public:
	virtual const dReal* compute() = 0;
protected:
	CoordinateTransform();
	
	boost::numeric::ublas::vector<double> pos_;
	boost::math::quaternion<double> ori_;
	dMatrix4 tmatrix_;	
};

class SimpleTransform : public CoordinateTransform
{
public:
	SimpleTransform(const std::string& type, const std::string& subtype) : 
		CoordinateTransform(), 
		type_(type), 
		subtype_(subtype) {}
	virtual ~SimpleTransform();
	void setData(dRealPtr data) {data_ = data;};
	virtual const dReal* compute();
private:
	SimpleTransform() {}; // Private default constructor - type and subtype must be set!
	std::string type_;
	std::string subtype_;
	dRealPtr data_;
};


class MarkerTransform : public CoordinateTransform
{
public:
	virtual ~MarkerTransform();
	void addLocalCoord(dRealPtr lc) 
	{
		localCoords_.push_back(lc);
	}
	void addGlobalCoord(dRealPtr gc) 
	{
		globalCoords_.push_back(gc);
	}
	virtual const dReal* compute();
private:
	dRealVector_t globalCoords_;
	dRealVector_t localCoords_;
};


class CompositeTransform : public CoordinateTransform
{
public:
	CompositeTransform() : CoordinateTransform(), mNeedsUpdate(true) {}
	virtual ~CompositeTransform();
	//! Adds the transform to the composition.
	void add(CoordinateTransformPtr t) 
	{
		childTransforms_.push_back(t);
		mNeedsUpdate = true;
	};
	//! Computes the composite transform
	virtual const dReal* compute();
	//! Get the current transformation matrix - do not compute it
	const dReal* getTMatrix()
	{
		if ( mNeedsUpdate )
		{
			return compute();
		}
		else
		{
			return tmatrix_;
		}
	}
	//! Get number of child transforms
	int size() {return childTransforms_.size();}
	void clear() {childTransforms_.clear();}
private:
	//Collection of child transforms.
	CoordinateTransformList_t childTransforms_;
	bool mNeedsUpdate;
};


};

#endif
