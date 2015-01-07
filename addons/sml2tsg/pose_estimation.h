#ifndef POSE_ESTIMATION_H
#define POSE_ESTIMATION_H

#include <ode/ode.h>

//#include "config.h"

#include <iostream>

#define WANT_STREAM                  // include.h will get stream fns
#define WANT_MATH                    // include.h will get math fns
                                     // newmatap.h will get include.h

#include <newmat/newmatap.h>                // need matrix applications

#include <newmat/newmatio.h>                // need matrix output routines

#ifdef use_namespace
using namespace NEWMAT;              // access NEWMAT namespace
#endif

namespace sceneml {

//class SCENEML_API PoseEstimatorBase
class PoseEstimatorBase
{
public:
	virtual void estimate(const dReal *X1, const dReal *X2, int nMarkers) = 0;
	//virtual void estimate(const Matrix& lCoord, const Matrix& gCoord) = 0;
	void getPose(dMatrix4 T);
	//ReturnMatrix getPose();

protected:
	PoseEstimatorBase();
	virtual ~PoseEstimatorBase() {};

	dMatrix4 tmatrix_;
};

//class SCENEML_API SVDEstimator : public PoseEstimatorBase
class SVDEstimator : public PoseEstimatorBase
{
public:
	SVDEstimator() : PoseEstimatorBase() {};
	virtual ~SVDEstimator() {};

	void estimate(const dReal *X1, const dReal *X2, int nMarkers);
	//void estimate(const Matrix& lCoord, const Matrix& gCoord);
protected:
};

};

#endif
