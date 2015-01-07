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
 * CappedCylinder.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#include "CappedCylinder.h"
#include <plugin_framework/Plugin.h>
#include <boost/foreach.hpp>

const std::string CappedCylinder::Type("ODECappedCylinder");

void* CappedCylinder::create(PF_ObjectParams* params)
{
	CappedCylinder* ptr = new CappedCylinder();
	ptr->services = params->platformServices;
	return ptr;
}

int CappedCylinder::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<CappedCylinder*>(p);

	return 0;
}

CappedCylinder::CappedCylinder() :
	Geometry()
{

}

CappedCylinder::~CappedCylinder()
{

}

void CappedCylinder::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("length") );
	info.addProperty( getProperty("radius") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);
}

void CappedCylinder::notifyMoved( const float* translation, const float* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In CappedCylinder::notifyMoved()" << std::endl;
#endif
	dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

	dQuaternion q = {0};
	for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
	dGeomSetQuaternion(odeobj, q);
}

void CappedCylinder::initImpl(const ObjectInfo& info)
{
	// Set object name
	name = info.name;

	odeobj = dCreateCCylinder (NULL, 1.0, 1.0);
	dGeomSetData (odeobj, this);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property CappedCylinder::getPropertyImpl(const std::string& name) const
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

void CappedCylinder::setPropertyImpl(const Property& p)
{
	// To get here, it must be a geom specific property
	if ( p.name_str() == "length" ) {
		setLength( boost::any_cast<float>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "radius" ) {
		setRadius( boost::any_cast<float>(p.const_value()) );
		return;
	}

	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

void CappedCylinder::setLength(float length)
{
	dReal unchanged, oldval;
	dGeomCCylinderGetParams(odeobj, &unchanged, &oldval);
	dGeomCCylinderSetParams(odeobj, unchanged, (dReal)length);
}

float CappedCylinder::getLength() const
{
	dReal length, radius;
	dGeomCCylinderGetParams(odeobj, &radius, &length);
	return float(length);
}

void CappedCylinder::setRadius(float radius)
{
	dReal unchanged, oldval;
	dGeomCCylinderGetParams(odeobj, &oldval, &unchanged);
	dGeomCCylinderSetParams(odeobj, (dReal)radius, unchanged);
}

float CappedCylinder::getRadius() const
{
	dReal length, radius;
	dGeomCCylinderGetParams(odeobj, &radius, &length);
	return float(radius);
}


