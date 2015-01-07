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
 * NodeUtils.h
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#ifndef SCENE_NODE_UTILS_H_
#define SCENE_NODE_UTILS_H_

#include "SceneNode.h"

#include <linalg/Matrix4.h>

namespace tinysg
{

//! Moves the node along the cartesian axes.
void translate(SceneNode*, const Vector3 &d, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Moves the node along the cartesian axes.
void translate(SceneNode*, Real x, Real y, Real z, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Moves the node along arbitrary axes.
void translate(SceneNode*, const Matrix3& axes, const Vector3 &move, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Moves the node along arbitrary axes.
void translate(SceneNode*, const Matrix3& axes, Real x, Real y, Real z, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Rotate the node around an arbitrary axis.
void rotate(SceneNode*, const Vector3 &axis, Real angle, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Rotate the node around an aritrary axis using a Quarternion.
void rotate(SceneNode*, const Quaternion &q, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);
//! Rotate the node around an aritrary axis using a Quarternion.
void rotate(SceneNode*, Real w, Real a, Real b, Real c, tinysg::TransformSpace relativeTo=tinysg::TS_LOCAL);

Matrix4 get_transform_matrix(const SceneNode*, tinysg::TransformSpace relativeTo=tinysg::TS_WORLD);

} // End namespace tinysg

#endif
