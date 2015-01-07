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
 * Sphere.cpp
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#include "Sphere.h"
#include <plugin_framework/Plugin.h>
#include <boost/foreach.hpp>

const std::string Sphere::Type("ODESphere");

void* Sphere::create(PF_ObjectParams* params)
{
	Sphere* ptr = new Sphere();
	ptr->services = params->platformServices;
	return ptr;
}

int Sphere::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<Sphere*>(p);

	return 0;
}

Sphere::Sphere() :
	Geometry()
{

}

Sphere::~Sphere()
{

}

void Sphere::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("radius") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);
}

void Sphere::notifyMoved( const Real* translation, const Real* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In Sphere::notifyMoved()" << std::endl;
#endif
	dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

	dQuaternion q = {0};
	for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
	dGeomSetQuaternion(odeobj, q);
}

void Sphere::initImpl(const ObjectInfo& info)
{
	// Set object name
	name = info.name;

	odeobj = dCreateSphere (NULL, 1.0);
	dGeomSetData (odeobj, this);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property Sphere::getPropertyImpl(const std::string& name) const
{
	if ( name == "radius" ) {
		return Property(name, getRadius());
	}

	// Have parent class deal with generic ODE properties
	return Geometry::getPropertyImpl(name);
}

void Sphere::setPropertyImpl(const Property& p)
{
	if ( p.name_str() == "radius" ) {
		setRadius( boost::any_cast<Real>(p.const_value()) );
		return;
	}

	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

void Sphere::setRadius(Real r)
{
	dGeomSphereSetRadius(odeobj, (dReal)r);
}

Real Sphere::getRadius() const
{
	return (Real)dGeomSphereGetRadius(odeobj);
}
