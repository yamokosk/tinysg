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
 * ObjectModel.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#ifndef OBJECTMODEL_H_
#define OBJECTMODEL_H_

#include <string>
#include <list>
#include <vector>
#include <map>

#include "Property.h"
#include "Iterator.h"
#include "Visitor.h"

namespace tinysg {

/*
 * Scene node interface
 */
struct Node
{
	virtual ~Node() {};
	virtual void updatePose( const float* translation, const float* rotation ) = 0;
};

struct Point3D
{
	Point3D() : x(0.0), y(0.0), z(0.0) {};
	Point3D(float a, float b, float c) : x(a), y(b), z(c) {};
	float x,y,z;
};

struct TriFace
{
	int v1, v2, v3;
};

struct TriSurfaceMesh
{
	std::vector<TriFace> faces;
	std::vector<Point3D> vertices;
};

/*
 * Scene object info
 */
struct ObjectInfo
{
	void addProperty(const Property& p)
	{
		parameters.push_back(p);
	}
	std::string name;
	std::string type;
	PropertyContainer parameters;
	std::vector<TriSurfaceMesh> meshes;
};

/*
 * Scene object interface
 */
struct SceneObject
{
	virtual ~SceneObject() {};
	virtual void init( const ObjectInfo& info ) = 0;
	virtual Property getProperty(const std::string& p) const = 0;
	virtual void setProperty(const Property& p) = 0;
	virtual void getInfo( ObjectInfo& info ) const = 0;
	virtual void notifyMoved( const float* translation, const float* rotation ) = 0;
};

typedef std::vector<SceneObject*> SceneObjectContainer;
typedef VectorIterator<SceneObjectContainer> SceneObjectIterator;

/*
 * interface
 */
struct PoseGenerator
{
	virtual ~PoseGenerator() {};
	virtual void attachNode(Node* node) = 0;
	virtual void detachNode(Node* node) = 0;
	virtual void notifyMoved() = 0;
};

/*
 * Used to store the result of a scene query.
 */
struct QueryInfo
{
	std::string type;
};

typedef std::pair<SceneObject*, SceneObject*> SceneObjectPair;

struct QueryArguments
{
	virtual ~QueryArguments() {};
	void resetResults()
	{
		objectsInCollision.clear();
		distanceMap.clear();
	}

	PropertyContainer parameters;
	std::vector<SceneObjectPair> objectsInCollision;
	std::vector<float> distanceMap;
	std::vector<Point3D> critpnt;
	std::vector<Point3D> regpnt;
};

/*
 * Query interface
 */
struct Query
{
	virtual ~Query() {};
	virtual void init() {};
	virtual void getInfo(QueryInfo* info) = 0;
	virtual void execute(QueryArguments* args) = 0;
};

}  // namespace tinysg

#endif /* OBJECTMODEL_H_ */
