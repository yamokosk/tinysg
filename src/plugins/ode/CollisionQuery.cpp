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
 * CollisionQuery.cpp
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#include "CollisionQuery.h"
#include <plugin_framework/Plugin.h>
#include <boost/any.hpp>
#include <boost/foreach.hpp>

#include "Space.h"

const std::string CollisionQuery::Type("ODECollisionQuery");

void* CollisionQuery::create(PF_ObjectParams* params)
{
	CollisionQuery* ptr = new CollisionQuery();
	ptr->services = params->platformServices;
	return ptr;
}

int CollisionQuery::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<CollisionQuery*>(p);

	return 0;
}

CollisionQuery::CollisionQuery()
{

}

CollisionQuery::~CollisionQuery()
{
}

void CollisionQuery::getInfo(QueryInfo* i)
{
	memcpy(i, &info, sizeof(QueryInfo));
}

void CollisionQuery::execute(QueryArguments* args)
{
	BOOST_FOREACH(Property param, args->parameters)
	{
		SceneObjectPair p = boost::any_cast< SceneObjectPair >(param.value());
		Space* s1 = static_cast<Space*>(p.first);
		Space* s2 = static_cast<Space*>(p.second);

		dSpaceCollide2( (dGeomID)s1->getID(), (dGeomID)s2->getID(), (void*)args, CollisionQuery::collisionCallback);
	}
}

void CollisionQuery::collisionCallback(void* ptr, dGeomID o1, dGeomID o2)
{
	// TODO: If the Geom is a TriMesh, I might need to be setting the previous
	// transform with calls to void dGeomTriMeshSetLastTransform( dGeomID g, dMatrix4 last_trans )
	// and dReal* dGeomTriMeshGetLastTransform( dGeomID g )

	dContactGeom dContactPts[NUM_CONTACT_POINTS];

	int numContactPts = dCollide(o1, o2, NUM_CONTACT_POINTS, dContactPts, sizeof(dContactGeom));
	if (numContactPts > 0) {
		QueryArguments* args = static_cast<QueryArguments*>(ptr);

		// Get the associated SceneObject ids from the void pointer stored in the ODE object
		SceneObject* obj1 = static_cast<SceneObject*>( dGeomGetData(o1) );
		SceneObject* obj2 = static_cast<SceneObject*>( dGeomGetData(o2) );

		args->objectsInCollision.push_back( SceneObjectPair(obj1, obj2) );
	}
}


