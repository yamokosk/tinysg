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
 * Plane.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#include "Plane.h"
#include <plugin_framework/Plugin.h>
#include <boost/foreach.hpp>

const std::string Plane::Type("ODEPlane");

void* Plane::create(PF_ObjectParams* params)
{
	Plane* ptr = new Plane();
	ptr->services = params->platformServices;
	return ptr;
}

int Plane::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<Plane*>(p);

	return 0;
}

Plane::Plane() :
	Geometry()
{

}

Plane::~Plane()
{
}

void Plane::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("normal") );
	info.addProperty( getProperty("point") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);
}

void Plane::notifyMoved( const Real* translation, const Real* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In Plane::notifyMoved(). Doing nothing." << std::endl;
#endif
}

void Plane::initImpl( const ObjectInfo& info )
{
	// Set object name
	name = info.name;

	odeobj = dCreatePlane (NULL, 0.0, 0.0, 1.0, 1.0);
	dGeomSetData (odeobj, this);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property Plane::getPropertyImpl(const std::string& name) const
{
	// To get here, it must be a geom specific property
	if ( name == "params" ) {
		return Property(name, getParams());
	}

	if ( name == "normal" ) {
		return Property(name, getNormal());
	}

	if ( name == "point" ) {
		return Property(name, getPoint());
	}

	// Have parent class deal with generic ODE properties
	return Geometry::getPropertyImpl(name);
}

void Plane::setPropertyImpl(const Property& p)
{
	// To get here, it must be a geom specific property
	if ( p.name_str() == "params" ) {
		setParams( boost::any_cast<Vector4>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "normal" ) {
		setNormal( boost::any_cast<Vector3>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "point" ) {
		setPoint( boost::any_cast<Vector3>(p.const_value()) );
		return;
	}

	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

void Plane::setParams(const Vector4& v)
{
	dGeomPlaneSetParams(odeobj, (dReal)v[0], (dReal)v[1], (dReal)v[2], (dReal)v[3]);
}

Vector4 Plane::getParams() const
{
	dVector4 params;
	dGeomPlaneGetParams(odeobj, params);
	return Vector4((Real)params[0], (Real)params[1], (Real)params[2], (Real)params[3]);
}

void Plane::setNormal(const Vector3& n)
{
	Vector4 params = getParams();
	params[0] = n[0];
	params[1] = n[1];
	params[2] = n[2];
	setParams(params);
}

Vector3 Plane::getNormal() const
{
	Vector4 params = getParams();
	return Vector3(params[0], params[1], params[2]);
}

void Plane::setPoint(const Vector3& p)
{
	Real d = getNormal().dotProduct(p);
	Vector4 params = getParams();
	params[3] = d;
	setParams(params);
}
Vector3 Plane::getPoint() const
{
	Vector4 params = getParams();
	Vector3 normal = getNormal();
	return Vector3( params[3] * normal.normalisedCopy() );
}
