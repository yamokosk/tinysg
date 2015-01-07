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
 * DistanceQuery.h
 *
 *  Created on: Dec 5, 2008
 *      Author: yamokosk
 */

#ifndef MOCAPPOSEGENERATOR_H_
#define MOCAPPOSEGENERATOR_H_

// Logging
#include <log4cxx/logger.h>

#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>
#include <vector>
#include <map>
#include <api/ObjectModel.h>

#include "Marker.h"

class MocapPoseGenerator : public PoseGenerator
{
	static log4cxx::LoggerPtr logger;

	typedef std::vector<Node*> NodeVector;
	typedef std::map<unsigned int, Marker> IdMarkerMap;

public:
	MocapPoseGenerator();
	virtual ~MocapPoseGenerator() {};

	void updatePose();

	// Inherited from PoseGenerator
	void attachNode(Node* node);
	void detachNode(Node* node);
	void notifyMoved();

	// Marker management
	void addMarker( const Marker& );
	unsigned int numberOfMarkers() const {return (unsigned int)mIdMarkerMap.size();}
	bool hasMarkerById( unsigned int ) const;
	void updateMarkerWorldCoordinates(unsigned int id, const Vector3& coord);

protected:
	virtual void updatePoseImpl() {};

	Vector3 mPosition;
	Quaternion mOrientation;

private:
	NodeVector mRegisteredNodes;
	IdMarkerMap mIdMarkerMap;
};

#endif /* DISTANCEQUERY_H_ */
