/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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
 * ODE_Geom.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#ifndef ODE_GEOM_H_
#define ODE_GEOM_H_

#include "Geometry.h"

//struct PF_ObjectParams;

class Sphere : public Geometry
{
public:
	// static plugin interface
	static void* create(PF_ObjectParams *);
	static int destroy(void *);
	static const std::string Type;

	~Sphere();

	// SceneObject methods
	virtual void getInfo(ObjectInfo& info) const;
	virtual void notifyMoved( const float* translation, const float* rotation );

protected:
	// Geometry methods
	virtual void initImpl(const ObjectInfo& info);
	virtual Property getPropertyImpl(const std::string& name) const;
	virtual void setPropertyImpl(const Property& p);

private:
	// Setter/getter methods for the ODE Sphere object
	void setRadius(Real r);
	Real getRadius() const;

	Sphere();
};

#endif /* ODE_GEOM_H_ */
