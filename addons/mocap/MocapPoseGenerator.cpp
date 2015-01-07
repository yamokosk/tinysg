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

#include "MocapPoseGenerator.h"
#include <boost/foreach.hpp>

using namespace log4cxx;

LoggerPtr MocapPoseGenerator::logger(Logger::getLogger("MocapPoseGenerator"));

MocapPoseGenerator::MocapPoseGenerator() :
	mPosition(0.,0.,0.),
	mOrientation(0.,1.,0.,0.)
{

}

void
MocapPoseGenerator::updatePose()
{
	updatePoseImpl();
	notifyMoved();
}

void
MocapPoseGenerator::attachNode(Node* node)
{
	mRegisteredNodes.push_back(node);
	LOG4CXX_DEBUG(logger, " Added a node. Now have " << mRegisteredNodes.size() << " attached nodes.");
}

void
MocapPoseGenerator::detachNode(Node* node)
{
	NodeVector::iterator itr;
	for (itr = mRegisteredNodes.begin(); itr != mRegisteredNodes.end(); ++itr)
	{
		if ( node == *itr )
		{
			mRegisteredNodes.erase( itr );
			LOG4CXX_DEBUG(logger, " Detached a node. Now have " << mRegisteredNodes.size() << " attached nodes.");
			break;
		}
	}
}

void
MocapPoseGenerator::notifyMoved()
{
	for (unsigned int n=0; n < mRegisteredNodes.size(); ++n)
	{
		LOG4CXX_DEBUG(logger, " Notifying node " << n << " of an update.");
		mRegisteredNodes[n]->updatePose(mPosition.ptr(), mOrientation.ptr());
		LOG4CXX_DEBUG(logger, " Node " << n << " notified.");
	}
}

void
MocapPoseGenerator::addMarker( const Marker& marker)
{
	if ( mIdMarkerMap.find(marker.id) != mIdMarkerMap.end() )
	{
		LOG4CXX_WARN(logger, " Marker " << marker.name << " already exists in this generator. Ignoring addMarker() request.");
		return;
	}

	mIdMarkerMap[marker.id] = marker;
}

bool
MocapPoseGenerator::hasMarkerById( unsigned int id ) const
{
	if ( mIdMarkerMap.find(id) != mIdMarkerMap.end() )
	{
		return true;
	} else
	{
		return false;
	}
}

void
MocapPoseGenerator::updateMarkerWorldCoordinates(unsigned int id, const Vector3& coord)
{
	LOG4CXX_WARN(logger, "Instructed to update marker " << id << ".");
}


