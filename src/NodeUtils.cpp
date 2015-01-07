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
 * NodeUtils.cpp
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#include "NodeUtils.h"

#include <linalg/Matrix3.h>

namespace tinysg
{

void translate(SceneNode* node, const Vector3 &d, tinysg::TransformSpace relativeTo)
{
	Vector3 translate(0.0,0.0,0.0);

	switch(relativeTo)
	{
	case tinysg::TS_LOCAL:
		// Position is relative to parent so transform downwards
		translate = node->getPosition(TS_PARENT) + (node->getOrientation(TS_PARENT) * d);
		break;
	case tinysg::TS_WORLD:
		// Position is relative to parent so transform upwards
		if ( node->hasParent() )
		{
			translate = node->getPosition(TS_PARENT) + (node->getParent()->getOrientation(TS_WORLD).Inverse() * d);
		}
		else
		{
			translate = node->getPosition(TS_PARENT) + d;
		}
		break;
	case tinysg::TS_PARENT:
		translate = node->getPosition(TS_PARENT) + d;
		break;
	}

	node->setPosition( translate );
}

//! Moves the node along the cartesian axes.
void translate(SceneNode* node, Real x, Real y, Real z, tinysg::TransformSpace relativeTo)
{
	Vector3 v(x, y, z);
	translate(node, v, relativeTo);
}

//! Moves the node along arbitrary axes.
void translate(SceneNode* node, const Matrix3 &axes, const Vector3 &move, tinysg::TransformSpace relativeTo)
{
	Vector3 derived = axes * move;
	translate(node, derived, relativeTo);
}

//! Moves the node along arbitrary axes.
void translate(SceneNode* node, const Matrix3 &axes, Real x, Real y, Real z, tinysg::TransformSpace relativeTo)
{
	Vector3 d(x, y, z);
	translate(node, axes, d, relativeTo);
}

//! Rotate the node around an arbitrary axis.
void rotate(SceneNode* node, const Vector3 &axis, Real angle, tinysg::TransformSpace relativeTo)
{
	Quaternion q(angle, axis);
	rotate(node, q, relativeTo);
}

void rotate(SceneNode* node, Real w, Real a, Real b, Real c, tinysg::TransformSpace relativeTo)
{
	Quaternion q(w,a,b,c);
	rotate(node, q, relativeTo);
}

//! Rotate the node around an aritrary axis using a Quarternion.
void rotate(SceneNode* node, const Quaternion &q, tinysg::TransformSpace relativeTo)
{
	Quaternion qnorm = q, rotate;
	qnorm.normalise();

	switch(relativeTo)
	{
	case tinysg::TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		rotate = node->getOrientation(TS_PARENT) * qnorm;
		break;
	case tinysg::TS_WORLD:
		// Rotations are normally relative to local axes, transform up
		if ( node->hasParent() )
		{
			rotate = node->getOrientation(TS_PARENT) * node->getOrientation(TS_WORLD).Inverse() * qnorm * node->getOrientation(TS_WORLD);
		}
		break;
	case tinysg::TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		rotate = qnorm * node->getOrientation(TS_PARENT);
		break;
	}

	node->setOrientation( rotate );
}

Matrix4 get_transform_matrix(const SceneNode* node, tinysg::TransformSpace relativeTo)
{
	Matrix4 T( node->getOrientation(relativeTo) );
	Vector3 pos( node->getPosition(relativeTo) );
	for (int r=0; r<3; ++r)
	{
		T[r][3] = pos[r];
	}
	return T;
}

} // End namespace
