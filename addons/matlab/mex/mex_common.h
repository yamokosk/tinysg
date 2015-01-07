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

#ifndef _SCENEML_MEX_COMMON_H_FILE_
#define _SCENEML_MEX_COMMON_H_FILE_

// Required MATLAB includes
#include <mex.h>

// System includes
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>
#include <memory>

#include <TinySG.h>

typedef std::auto_ptr<tinysg::SceneGraph> SceneGraphPtr;

// Convience defines for the LHS and RHS Matlab arguments
#define RHS_ARG_1	prhs[0]
#define RHS_ARG_2   prhs[1]
#define RHS_ARG_3   prhs[2]
#define RHS_ARG_4   prhs[3]
#define RHS_ARG_5   prhs[4]
#define RHS_ARG_6   prhs[5]
#define RHS_ARG_7   prhs[6]
#define RHS_ARG_8   prhs[7]
#define RHS_ARG_9   prhs[8]
#define RHS_ARG_10  prhs[9]

#define LHS_ARG_1   plhs[0]
#define LHS_ARG_2   plhs[1]
#define LHS_ARG_3   plhs[2]
#define LHS_ARG_4   plhs[3]
#define LHS_ARG_5   plhs[4]
#define LHS_ARG_6   plhs[5]
#define LHS_ARG_7   plhs[6]
#define LHS_ARG_8   plhs[7]
#define LHS_ARG_9   plhs[8]

// Useful macros..
#define DUMP_ME(x, fmt) printf("%s:%u: %s=" fmt "\n", __FILE__, __LINE__, #x, x)
#define MAX(x,y) ((x > y) ? x : y)

#endif
