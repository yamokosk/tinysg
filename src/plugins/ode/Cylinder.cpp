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
 * Cylinder.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#include "Cylinder.h"
#include <plugin_framework/Plugin.h>
#include <boost/foreach.hpp>

const std::string Cylinder::Type("ODECylinder");

void* Cylinder::create(PF_ObjectParams* params)
{
	Cylinder* ptr = new Cylinder();
	ptr->services = params->platformServices;
	return ptr;
}

int Cylinder::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<Cylinder*>(p);

	return 0;
}

Cylinder::Cylinder() :
	Geometry()
{

}

Cylinder::~Cylinder()
{

}

void Cylinder::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("length") );
	info.addProperty( getProperty("radius") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);
}

void Cylinder::notifyMoved( const Real* translation, const Real* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In Cylinder::notifyMoved()" << std::endl;
#endif
	dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

	dQuaternion q = {0};
	for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
	dGeomSetQuaternion(odeobj, q);
}

void Cylinder::initImpl(const ObjectInfo& info)
{
	// Set object name
	name = info.name;

	odeobj = dCreateCylinder (NULL, 1.0, 1.0);
	dGeomSetData (odeobj, this);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property Cylinder::getPropertyImpl(const std::string& name) const
{
	// To get here, it must be a geom specific property
	if ( name == "length" ) {
		return Property(name, getLength());
	}

	if ( name == "radius" ) {
		return Property(name, getRadius());
	}

	// Have parent class deal with generic ODE properties
	return Geometry::getPropertyImpl(name);
}

void Cylinder::setPropertyImpl(const Property& p)
{
	// To get here, it must be a geom specific property
	if ( p.name_str() == "length" ) {
		setLength( boost::any_cast<Real>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "radius" ) {
		setRadius( boost::any_cast<Real>(p.const_value()) );
		return;
	}

	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

void Cylinder::setLength(Real length)
{
	dReal unchanged, oldval;
	dGeomCylinderGetParams(odeobj, &unchanged, &oldval);
	dGeomCylinderSetParams(odeobj, unchanged, (dReal)length);
}

Real Cylinder::getLength() const
{
	dReal length, radius;
	dGeomCylinderGetParams(odeobj, &radius, &length);
	return Real(length);
}

void Cylinder::setRadius(Real radius)
{
	dReal unchanged, oldval;
	dGeomCylinderGetParams(odeobj, &oldval, &unchanged);
	dGeomCylinderSetParams(odeobj, (dReal)radius, unchanged);
}

Real Cylinder::getRadius() const
{
	dReal length, radius;
	dGeomCylinderGetParams(odeobj, &radius, &length);
	return Real(radius);
}


