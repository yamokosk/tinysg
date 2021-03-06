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
 * Plane.h
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#ifndef PLANE_H_
#define PLANE_H_

#include "Geometry.h"

#include <linalg/Vector4.h>

//struct PF_ObjectParams;

class Plane : public Geometry
{
public:
	// static plugin interface
	static void* create(PF_ObjectParams *);
	static int destroy(void *);
	static const std::string Type;
	~Plane();

	// SceneObject methods
	virtual void getInfo(ObjectInfo& info) const;
	virtual void notifyMoved( const float* translation, const float* rotation );

protected:
	// Geometry methods
	virtual void initImpl(const ObjectInfo& info);
	virtual Property getPropertyImpl(const std::string& name) const;
	virtual void setPropertyImpl(const Property& p);

private:
	// Setter/getter methods for the ODE Plane object
	void setParams(const Vector4& v);
	Vector4 getParams() const;

	void setNormal(const Vector3& n);
	Vector3 getNormal() const;

	void setPoint(const Vector3& p);
	Vector3 getPoint() const;

	Plane();
};

#endif /* PLANE_H_ */
