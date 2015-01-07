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
/*
 * TriangleMesh.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#include "TriangleMesh.h"
#include <plugin_framework/Plugin.h>
#include <mesh/MeshImport.h>
#include <boost/foreach.hpp>
#include <api/Services.h>

const std::string TriangleMesh::Type("ODETriangleMesh");

void* TriangleMesh::create(PF_ObjectParams* params)
{
	TriangleMesh* ptr = new TriangleMesh();
	ptr->services = params->platformServices;
	return ptr;
}

int TriangleMesh::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<TriangleMesh*>(p);

	return 0;
}

TriangleMesh::TriangleMesh() :
	Geometry(),
	mesh_(NULL)
{

}

TriangleMesh::~TriangleMesh()
{

}

void TriangleMesh::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("filename") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);

#ifdef BUG_trimesh_filename
	std::cout << info.name << ": filename - " << filename_ << std::endl;
#endif
}

void TriangleMesh::notifyMoved( const float* translation, const float* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In TriangleMesh::notifyMoved()" << std::endl;
#endif
	if ( mesh_ != NULL )
	{
		dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

		dQuaternion q = {0};
		for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
		dGeomSetQuaternion(odeobj, q);
	}
}

void TriangleMesh::initImpl(const ObjectInfo& info)
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In TriangleMesh::initImpl()" << std::endl;
#endif
	// Set object name
	name = info.name;

	BOOST_FOREACH(Property prop, info.parameters)
	{
		if (prop.name_str() == "filename")
		{
			filename_ = boost::any_cast<std::string>(prop.value());
			break;
		}
	}

	// Load mesh from file
	std::string errmsg;
#ifdef BUG_dGeomSetPosition
	std::cout << "Loading mesh from: " << filename_ << std::endl;
#endif
	mesh_ = createTriMesh(filename_, 1.0, 1.0, 1.0, errmsg);
	if ( mesh_ == NULL )
	{
#ifdef BUG_dGeomSetPosition
		std::cout << "mesh_ was NULL: " << errmsg << std::endl;
#endif
		LOG_ERROR(services, errmsg);
		return;
	}

	// Create ODE trimesh object
	dTriMeshDataID Data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(Data,
								(void*)mesh_->vertexData(), mesh_->vertexStride(), mesh_->numVertices(),
								(void*)mesh_->faceData(), mesh_->numFaces(), mesh_->faceStride());
	odeobj = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);

	// Save a pointer back to this adapter class inside the ODE object
	dGeomSetData(odeobj, this);

	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property TriangleMesh::getPropertyImpl(const std::string& name) const
{
#ifdef BUG_trimesh_filename
	std::cout << "In TriangleMesh::getPropertyImpl(): name = " << name << std::endl;
#endif
	// To get here, it must be a geom specific property
	if ( name == "filename" )
	{
		Property p(name, std::string(filename_));
#ifdef BUG_trimesh_filename
		std::cout << "** " << p.name_str() << ": " << boost::any_cast<std::string>(p.value()) << std::endl;
#endif
		return p;
	}

	// Have parent class deal with generic ODE properties
	return Geometry::getPropertyImpl(name);
}

void TriangleMesh::setPropertyImpl(const Property& p)
{
	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

