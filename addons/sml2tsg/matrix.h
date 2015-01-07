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

#ifndef _SCEMEML_MATRIX_H_FILE_
#define _SCEMEML_MATRIX_H_FILE_

#include <ode/ode.h>

//#include "config.h"

namespace sceneml {

#define dMULTIPLYOPV_333(A,op,B,C) \
do { \
  (A)[0] op B[0]*C[0]; \
  (A)[1] op B[0]*C[1]; \
  (A)[2] op B[0]*C[2]; \
  (A)[4] op B[1]*C[0]; \
  (A)[5] op B[1]*C[1]; \
  (A)[6] op B[1]*C[2]; \
  (A)[8] op B[2]*C[0]; \
  (A)[9] op B[2]*C[1]; \
  (A)[10] op B[2]*C[2]; \
} while(0)

/*void SCENEML_API dTSetIdentity(dMatrix4 T);
void SCENEML_API dTFromTrans(dMatrix4 T, dReal x, dReal y, dReal z);
void SCENEML_API dTFromAxisAndAngle (dMatrix4 T, dReal ax, dReal ay, dReal az, dReal angle);
void SCENEML_API dTfromQ (dMatrix4 T, const dQuaternion q);
void SCENEML_API dTFromEuler123(dReal* T, dReal x, dReal y, dReal z);
void SCENEML_API dTFromRAndPos(dMatrix4 T, const dMatrix3 rot, const dVector3 pos);
void SCENEML_API dRFromT(dMatrix3 R, const dMatrix4 T);
dReal SCENEML_API dDeterminant3(const dMatrix3 R);
void SCENEML_API dSVD(dMatrix4 U, dMatrix4 V, dVector4 D, const dMatrix4 C);
void SCENEML_API dTPrint(dMatrix4 T);
void SCENEML_API dRPrint(dMatrix3 T);
void SCENEML_API dVPrint(dVector3 V);*/
void dTSetIdentity(dMatrix4 T);
void dTFromTrans(dMatrix4 T, dReal x, dReal y, dReal z);
void dTFromAxisAndAngle (dMatrix4 T, dReal ax, dReal ay, dReal az, dReal angle);
void dTfromQ (dMatrix4 T, const dQuaternion q);
void dTFromEuler123(dReal* T, dReal x, dReal y, dReal z);
void dTFromRAndPos(dMatrix4 T, const dMatrix3 rot, const dVector3 pos);
void dRFromT(dMatrix3 R, const dMatrix4 T);
dReal dDeterminant3(const dMatrix3 R);
void dSVD(dMatrix4 U, dMatrix4 V, dVector4 D, const dMatrix4 C);
void dTPrint(dMatrix4 T);
void dRPrint(dMatrix3 T);
void dVPrint(dVector3 V);

};

#endif
