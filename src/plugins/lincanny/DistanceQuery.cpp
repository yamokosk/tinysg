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
 * DistanceQuery.cpp
 *
 *  Created on: Dec 5, 2008
 *      Author: yamokosk
 */

#include "DistanceQuery.h"
#include <api/Services.h>
#include <boost/any.hpp>

#include "algorithm/BodyManager.h"

#include <linalg/Vector3.h>

using namespace obrsp::linalg;
using namespace tinysg;

const std::string DistanceQuery::Type("LCDistanceQuery");

void* DistanceQuery::create(plugin::PF_ObjectParams* params)
{
	DistanceQuery* ptr = new DistanceQuery();
	ptr->services = params->platformServices;
	return ptr;
}

int DistanceQuery::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<DistanceQuery*>(p);

	return 0;
}

DistanceQuery::DistanceQuery()
{

}

DistanceQuery::~DistanceQuery()
{
}

void DistanceQuery::init()
{
	BodyManager::getInstance().init();
}

void DistanceQuery::getInfo(tinysg::QueryInfo* i)
{
}

void DistanceQuery::execute(tinysg::QueryArguments* args)
{
	LOG_MESSAGE(services, "In DistanceQuery::execute()");

	BodyManager& bm = BodyManager::getInstance();

	if ( !bm.computeDistances() )
	{
		LOG_ERROR(services, "Distance query failed for unknown reason.");
		return;
	}

	// Get distance data
	VERTEX3D** critPoints = bm.getCriticalPointArray();
	VERTEX3D** regPoints = bm.getPointArray();
	double** distances = bm.getDistancesArray();
	VERTEX3D pnt;

	std::stringstream ss;
	ss << "Num crit bodies: " << bm.getCriticalBodyCount() << ", Num reg bodies: " << bm.getBodyCount() << std::endl;

	LOG_MESSAGE(services, ss.str());
	for (int ii = 0; ii < bm.getCriticalBodyCount(); ++ii)
	{
		for (int jj = 0; jj < bm.getBodyCount(); ++jj)
		{
			pnt = critPoints[ ii ][ jj ];
			//std::cout << "c: " << critPoints[ ii ][ jj ].x << ", " << critPoints[ ii ][ jj ].y << ", " << critPoints[ ii ][ jj ].z << std::endl;
			//std::cout << "c: " << pnt.x << ", " << pnt.y << ", " << pnt.z << std::endl;
			Vector3 p( (Real)pnt.x, (Real)pnt.y, (Real)pnt.z );

			pnt = regPoints[ ii ][ jj ];
			//std::cout << "r: " << regPoints[ ii ][ jj ].x << ", " << regPoints[ ii ][ jj ].y << ", " << regPoints[ ii ][ jj ].z << std::endl;
			//std::cout << "r: " << pnt.x << ", " << pnt.y << ", " << pnt.z << std::endl;
			Vector3 q( (Real)pnt.x, (Real)pnt.y, (Real)pnt.z );

			float dist = (float)distances[ii][jj];

			args->critpnt.push_back( Point3D(p[0], p[1], p[2]) );
			args->regpnt.push_back( Point3D(q[0], q[1], q[2]) );
			args->distanceMap.push_back(dist);
		}
	}
	LOG_MESSAGE(services, "Leaving DistanceQuery::execute()");
}
