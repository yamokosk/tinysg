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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

//#include "config.h"
#include "common.h"

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<float> VERTEX3D;
typedef boost::shared_ptr<unsigned int> INDEX;
 
//struct SCENEML_API POLYHEDRON {
struct POLYHEDRON {
	long vertex_count, index_count;
	short vertex_stride;
	VERTEX3D vertices;
	short index_stride;
	INDEX indices;
	std::string filename;
	POLYHEDRON();
	~POLYHEDRON();
};

typedef boost::shared_ptr<POLYHEDRON> PolyhedronPtr;

#endif
