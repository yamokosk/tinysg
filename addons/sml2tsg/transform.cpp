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

#include "transform.h"
#include "pose_estimation.h"
#include "matrix.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
using namespace sceneml;

CoordinateTransform::CoordinateTransform() :
	pos_(3),
	ori_(0,0,0,0)
{ }

SimpleTransform::~SimpleTransform()
{ }

//const dReal* SimpleTransform::compute()
const dReal* SimpleTransform::compute()
{
	//using namespace boost::numeric::ublas;

	//identity_matrix<double> eye(4);
	//matrix<double> tmatrix(4,4);
	dTSetIdentity(tmatrix_);
	
	// Compute transform - using pos and quaternion
	if ( !type_.compare("translation") ) {
		dTFromTrans(tmatrix_, (data_.get())[0], (data_.get())[1], (data_.get())[2]);
		//for (int n=0; n < 3; ++n) tmatrix(n,3) = pos_(n);
		//subrange(tmatrix, 0, 2, 3, 3) = pos_;
	} else if ( !type_.compare("rotation") ) {
		if ( !subtype_.compare("x") ) 		dTFromAxisAndAngle(tmatrix_, REAL(1.0), REAL(0.0), REAL(0.0), (data_.get())[0]); 
		else if ( !subtype_.compare("y") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(1.0), REAL(0.0), (data_.get())[0]); 
		else if ( !subtype_.compare("z") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(0.0), REAL(1.0), (data_.get())[0]);
		else if ( !subtype_.compare("e123") )	dTFromEuler123(tmatrix_, (data_.get())[0], (data_.get())[1], (data_.get())[2]);
		else if ( !subtype_.compare("t123") )	dTFromEuler123(tmatrix_, -(data_.get())[0], -(data_.get())[1], -(data_.get())[2]);
		else throw std::runtime_error("");
	} else {
	
	}
	return tmatrix_;
	/*if ( !type_.compare("translation") ) {
		dTFromTrans(tmatrix_, (data_.get())[0], (data_.get())[1], (data_.get())[2]);
	} else if ( !type_.compare("rotation") ) {
		if ( !subtype_.compare("x") ) 		dTFromAxisAndAngle(tmatrix_, REAL(1.0), REAL(0.0), REAL(0.0), (data_.get())[0]); 
		else if ( !subtype_.compare("y") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(1.0), REAL(0.0), (data_.get())[0]); 
		else if ( !subtype_.compare("z") ) 	dTFromAxisAndAngle(tmatrix_, REAL(0.0), REAL(0.0), REAL(1.0), (data_.get())[0]);
		else if ( !subtype_.compare("e123") )	dTFromEuler123(tmatrix_, (data_.get())[0], (data_.get())[1], (data_.get())[2]);
		else if ( !subtype_.compare("t123") )	dTFromEuler123(tmatrix_, -(data_.get())[0], -(data_.get())[1], -(data_.get())[2]);
		else throw std::runtime_error("");
	} else  {
		throw std::runtime_error("");
	}
	
	return tmatrix_;*/
}


MarkerTransform::~MarkerTransform()
{
	/*for (unsigned int n=0; n < localCoords_.size(); ++n)
	{
		delete [] localCoords_[n];
		delete [] globalCoords_[n];
	}*/
}

const dReal* MarkerTransform::compute()
{
	// Number of local and global coords is ASSUMED to be the same.. no error 
	// checking done here
	int nNumCoords = localCoords_.size();
	
	// Allocate temp storage for markers and fill up with data
	dRealPtr lCoords( new dReal[3 * nNumCoords] );
	dRealPtr gCoords( new dReal[3 * nNumCoords] );

	for (int n=0; n < nNumCoords; ++n)
	{
		//dReal* lCoord = localCoords_[n], gCoord = globalCoords[n];
		
		memcpy( (lCoords.get()+n*3), localCoords_[n].get(), 3*sizeof(dReal) );
		memcpy( (gCoords.get()+n*3), globalCoords_[n].get(), 3*sizeof(dReal) );
	}
	
	// Do estimation and get answer
	SVDEstimator estimator;
	estimator.estimate(gCoords.get(), lCoords.get(), nNumCoords);
	estimator.getPose(tmatrix_);
	
	return tmatrix_;
}


CompositeTransform::~CompositeTransform()
{
	//CoordinateTransformList_t::iterator it = childTransforms_.begin();
	//for (; it != childTransforms_.end(); ++it) delete (*it);
}


const dReal* CompositeTransform::compute() 
{
	dTSetIdentity(tmatrix_);
	CoordinateTransformList_t::iterator it = childTransforms_.begin();
	for (; it != childTransforms_.end(); ++it)
	{
		dMatrix4 Tr;
		dMultiply0(Tr, tmatrix_, (*it)->compute(), 4, 4, 4);
		memcpy(tmatrix_, Tr, sizeof(dMatrix4));
	}
	mNeedsUpdate = false;
	return tmatrix_;
};
