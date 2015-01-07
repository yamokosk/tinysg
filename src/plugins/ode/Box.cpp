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
 * Box.cpp
 *
 *  Created on: Dec 1, 2008
 *      Author: yamokosk
 */

#include "Box.h"
#include <plugin_framework/Plugin.h>
#include <api/Services.h>
#include <boost/foreach.hpp>

const std::string Box::Type("ODEBox");

void* Box::create(PF_ObjectParams* params)
{
	Box* ptr = new Box();
	ptr->services = params->platformServices;
	return ptr;
}

int Box::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<Box*>(p);

	return 0;
}

Box::Box() : Geometry()
{

}

Box::~Box()
{

}

void Box::getInfo( ObjectInfo& info ) const
{
	info.type = Type;
	info.addProperty( getProperty("length") );
	info.addProperty( getProperty("width") );
	info.addProperty( getProperty("height") );

	// Have parent class deal with generic ODE properties
	Geometry::getInfo(info);
}

void Box::notifyMoved( const float* translation, const float* rotation )
{
#ifdef BUG_dGeomSetPosition
	std::cout << "In Box::notifyMoved()" << std::endl;
	std::cout << "p:";
	for(unsigned int n=0; n < 3; ++n) std::cout << " " << translation[n];
#endif
	dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

#ifdef BUG_dGeomSetPosition
	std::cout << ", q:";
	for(unsigned int n=0; n < 4; ++n) std::cout << " " << rotation[n];
	std::cout << std::endl;
#endif

	dQuaternion q = {0};
	for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
	dGeomSetQuaternion(odeobj, q);

#ifdef BUG_dGeomSetPosition
	std::cout << "Leaving Box::notifyMoved()" << std::endl;
#endif
}

void Box::initImpl(const ObjectInfo& info)
{
	// Set object name
	name = info.name;

	// Create the ODE Box object
	odeobj = dCreateBox (NULL, 1.0, 1.0, 1.0);
	// Save a pointer back to this adapter class inside the ODE object
	dGeomSetData(odeobj, this);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property Box::getPropertyImpl(const std::string& name) const
{
	// To get here, it must be a geom specific property
	if ( name == "lengths" ) {
		return Property(name, getLengths());
	}

	if ( name == "length" ) {
		return Property(name, getLength());
	}

	if ( name == "width" ) {
		return Property(name, getWidth());
	}

	if ( name == "height" ) {
		return Property(name, getHeight());
	}

	// Have parent class deal with generic ODE properties
	return Geometry::getPropertyImpl(name);
}

void Box::setPropertyImpl(const Property& p)
{
	// To get here, it must be a geom specific property
	if ( p.name_str() == "lengths" ) {
		setLengths( boost::any_cast<Vector3>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "length" ) {
		setLength( boost::any_cast<float>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "width" ) {
		setWidth( boost::any_cast<float>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "height" ) {
		setHeight( boost::any_cast<float>(p.const_value()) );
		return;
	}

	// Have parent class deal with generic ODE properties
	Geometry::setPropertyImpl(p);
}

void Box::setLength(float val)
{
	Vector3 lwh = getLengths();
	lwh[0] = val;
	setLengths(lwh);
}

void Box::setWidth(float val)
{
	Vector3 lwh = getLengths();
	lwh[1] = val;
	setLengths(lwh);
}

void Box::setHeight(float val)
{
	Vector3 lwh = getLengths();
	lwh[2] = val;
	setLengths(lwh);
}

float Box::getLength() const
{
	Vector3 lwh = getLengths();
	return lwh[0];
}

float Box::getWidth() const
{
	Vector3 lwh = getLengths();
	return lwh[1];
}

float Box::getHeight() const
{
	Vector3 lwh = getLengths();
	return lwh[2];
}

void Box::setLengths(const Vector3& lengths)
{
	dGeomBoxSetLengths(odeobj, (dReal)lengths[0], (dReal)lengths[1], (dReal)lengths[2]);
}

Vector3 Box::getLengths() const
{
	dVector3 dLengths;
	dGeomBoxGetLengths(odeobj, dLengths);

	return Vector3( (Real)dLengths[0], (Real)dLengths[1], (Real)dLengths[2] );
}
