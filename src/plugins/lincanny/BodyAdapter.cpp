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
 * BodyAdapter.cpp
 *
 *  Created on: Dec 5, 2008
 *      Author: yamokosk
 */

#include "BodyAdapter.h"

#include <plugin_framework/Plugin.h>
#include <api/Services.h>
#include <linalg/Quaternion.h>
#include <linalg/Matrix3.h>
#include <boost/foreach.hpp>

#include "algorithm/BodyManager.h"

using namespace tinysg;

void* BodyAdapter::create(plugin::PF_ObjectParams* params)
{
	BodyAdapter* ptr = new BodyAdapter();
	ptr->services = params->platformServices;
	return ptr;
}

int BodyAdapter::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<BodyAdapter*>(p);

	return 0;
}

BodyAdapter::BodyAdapter() :
	bodyID(-1)
{
}

BodyAdapter::~BodyAdapter()
{
}

void BodyAdapter::init(const tinysg::ObjectInfo& info)
{
	// Set object name
	name = info.name;

	if ( info.type == "LCBody" )
		bodyType = NonCritical;
	else if ( info.type == "LCCritBody" )
		bodyType = Critical;

	// Set all properties.. should just be mesh filenames
	BOOST_FOREACH(tinysg::Property p, info.parameters)
	{
		setProperty(p);
	}

	if ( bodyType == Critical ) {
		bodyID = BodyManager::getInstance().addCriticalBody( &meshes[0], meshes.size() );
	} else {
		bodyID = BodyManager::getInstance().addBody( &meshes[0], meshes.size() );
	}
}

Property BodyAdapter::getProperty(const std::string& name) const
{
	if ( name == "position" )
	{
		return tinysg::Property(name, getPosition());
	}

	if ( name == "orientation" )
	{
		return tinysg::Property(name, getOrientation());
	}

	LOG_ERROR(services, "getProperty() failed. Property \"" + name + "\" is unknown or unsupported by this object.");
	return Property(name);
}

void BodyAdapter::setProperty(const Property& p)
{
	if ( p.name_str() == "filename")
	{
		std::string filename = boost::any_cast<std::string>(p.const_value());

		BoundingMesh* bm = new BoundingMesh(filename.c_str());

		LOG_MESSAGE(services, "Distance query plugin: Trying to load " + filename);

		if ( !bm->load() )
		{
			LOG_ERROR(services, "Failed to load mesh from \"" + filename + "\".");
			delete bm;
			return;
		} else {
			LOG_MESSAGE(services, "Distance query plugin: loaded mesh from " + filename + "\".");
			meshes.push_back( bm );
		}

		tinysg::Property color_property = p.get_parameter("color");
		if ( !color_property.value().empty() )
		{
			Vector3 color = boost::any_cast<Vector3>( color_property.value() );
			colors.push_back( color );
		}
		else
		{
			Vector3 white(255.0, 255.0, 255.0);
			colors.push_back( white );
		}

		Property alpha_property = p.get_parameter("alpha");
		if ( !alpha_property.value().empty() )
		{
			float alpha = boost::any_cast<float>( alpha_property.value() );
			alphas.push_back( alpha );
		}
		else
		{
			alphas.push_back( float(1.0) );
		}

		return;
	}
}

void BodyAdapter::getInfo(tinysg::ObjectInfo& info) const
{
	info.name = name;

	RigidBody* pBody = NULL;
	if ( bodyType == Critical )
	{
		info.type = "LCCritBody";
		pBody = (BodyManager::getInstance().getCriticalBodyArray())[bodyID];
	}
	else
	{
		info.type = "LCBody";
		pBody = (BodyManager::getInstance().getBodyArray())[bodyID];
	}

	for (int n=0; n < (int)meshes.size(); ++n)
	{
		// Get pointer to mesh
		BoundingMesh* mesh = meshes[n];

		// Report back the filename of the mesh and associated color and alpha
		// values
		tinysg::Property filename_property("filename", std::string(mesh->filename) );
		filename_property.add_parameter( Property("color", Vector3(colors[n])) );
		filename_property.add_parameter( Property("alpha", float(alphas[n])) );
		info.addProperty( filename_property );

		// Report back mesh data
		POLYHEDRON* ppoly = mesh->getPoly();
		TriSurfaceMesh surfmesh;

		for (int nv=0; nv < ppoly->vertex_count; ++nv)
		{
			VERTEX3D transformed_vertex;
			ppoly->vertices[nv]->transform(pBody->transform, transformed_vertex);

			Point3D pt;
			pt.x = transformed_vertex.x;
			pt.y = transformed_vertex.y;
			pt.z = transformed_vertex.z;

			surfmesh.vertices.push_back( pt );
		}

		for (int nf=0; nf < ppoly->face_count; ++nf)
		{
			if ( ppoly->faces[nf]->vertex_count > 3 )
			{
				LOG_ERROR(services, "Mesh is not a tri-mesh!");
			}

			TriFace face;
			face.v1 = ppoly->faces[nf]->vertices[0]->index;
			face.v2 = ppoly->faces[nf]->vertices[1]->index;
			face.v3 = ppoly->faces[nf]->vertices[2]->index;

			surfmesh.faces.push_back( face );
		}

		info.meshes.push_back(surfmesh);
	}
}

void BodyAdapter::notifyMoved( const float* translation, const float* rotation )
{
	Matrix3 R;
	Quaternion q(rotation);
	q.ToRotationMatrix(R);

	double T[16];

	T[0] = (double)R[0][0];
	T[1] = (double)R[1][0];
	T[2] = (double)R[2][0];
	T[3] = 0.0;

	T[4] = (double)R[0][1];
	T[5] = (double)R[1][1];
	T[6] = (double)R[2][1];
	T[7] = 0.0;

	T[8] = (double)R[0][2];
	T[9] = (double)R[1][2];
	T[10] = (double)R[2][2];
	T[11] = 0.0;

	T[12] = (double)translation[0];
	T[13] = (double)translation[1];
	T[14] = (double)translation[2];
	T[15] = 1.0;

	if ( bodyType == Critical )
	{
		BodyManager::getInstance().setCriticalTransform(bodyID, T);
	}
	else
	{
		BodyManager::getInstance().setTransform(bodyID, T);
	}
}

Vector3 BodyAdapter::getPosition() const
{
	return Vector3::ZERO;
}

Quaternion BodyAdapter::getOrientation() const
{
	return Quaternion::IDENTITY;
}
