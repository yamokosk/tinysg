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

#ifndef _SCENEML_COMMON_H_FILE_
#define _SCENEML_COMMON_H_FILE_

//#include "config.h"

// Standard C++ headers
#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>

// Useful macros
#define mBitsOn(x,by) ((x)|=(by))
#define mBitsOff(x,bn) ((x)&= ~(bn))


#endif
