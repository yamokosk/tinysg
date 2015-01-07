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
 * Geometry.cpp
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#include <plugin_framework/Plugin.h>
#include <api/Services.h>

#include "Geometry.h"
#include "Space.h"

Geometry::Geometry() :
	services(NULL),
	space(NULL),
	odeobj(NULL)
{

}

Geometry::~Geometry()
{
	if (odeobj != NULL) dGeomDestroy (odeobj);
}

void Geometry::init(const ObjectInfo& info)
{
	try {
		this->initImpl(info);
	} catch (const std::string& errmsg) {
		LOG_ERROR(services, "init() failed. Reason: " + errmsg);
	} catch (...) {
		LOG_ERROR(services, "init() failed. Unknown reason.");
	}
}

Property Geometry::getProperty(const std::string& name) const
{
	if (odeobj == NULL) {
		LOG_ERROR(services, "getProperty() failed. ODE object has not been initialized!");
		return Property(name);
	}

	try {
		return getPropertyImpl(name);
	} catch (const std::string& errmsg) {
		LOG_ERROR(services, "getProperty() failed. Reason: " + errmsg);
	} catch (...) {
		LOG_ERROR(services, "setProperty() failed. Unknown reason.");
	}

	// Only get here if there was a problem
	return Property(name);
}

void Geometry::setProperty(const Property& p)
{
	if (odeobj == NULL) {
		LOG_ERROR(services, "setProperty() failed. ODE object has not been initialized!");
		return;
	}

	try {
		setPropertyImpl(p);
	} catch (const boost::bad_any_cast& e) {
		LOG_ERROR(services, "setProperty() failed. Unexpected data type encountered for property \"" + p.name_str() + "\".");
	} catch (const std::string& errmsg) {
		LOG_ERROR(services, "setProperty() failed. Reason: " + errmsg);
	} catch (...) {
		LOG_ERROR(services, "setProperty() failed. Unknown reason.");
	}
}

void Geometry::getInfo(ObjectInfo& info) const
{
	info.name = name;
	if (space != NULL)
		info.addProperty( getProperty("space") );
	//info.addProperty( getProperty("category_bits") );
	//info.addProperty( getProperty("collide_bits") );
}

Property Geometry::getPropertyImpl(const std::string& name) const
{
	if ( name == "space" ) {
		return Property(name, getSpace());
	}

	if ( name == "class" ) {
		return Property(name, getClass());
	}

	if ( name == "category_bits" ) {
		return Property(name, getCategoryBits());
	}

	if ( name == "collide_bits") {
		return Property(name, getCollideBits());
	}

	if ( name == "enable" ) {
		return Property(name, getEnable());
	}

	if ( name == "position" ) {
		return Property(name, getPosition());
	}

	if ( name == "orientation" ) {
		return Property(name, getOrientation());
	}

	// Unknown/unsupported property
	throw std::string("Property \"" + name + "\" is unknown or unsupported by this object.");
}

void Geometry::setPropertyImpl(const Property& p)
{
	if ( p.name_str() == "enable" ) {
		setEnable( boost::any_cast<int>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "category_bits" ) {
		setCategoryBits( boost::any_cast<unsigned long>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "collide_bits" ) {
		setCollideBits( boost::any_cast<unsigned long>(p.const_value()) );
		return;
	}

	if ( p.name_str() == "space" ) {
		setSpace( boost::any_cast<SceneObject*>(p.const_value()) );
		return;
	}

	// Unknown/unsupported property
	throw std::string("Property \"" + p.name_str() + "\" is unknown or can't be 'set' by this object.");
}

/*void Geometry::notifyMoved( const Real* translation, const Real* rotation )
{
	dGeomSetPosition(odeobj, (dReal)translation[0], (dReal)translation[1], (dReal)translation[2]);

	dQuaternion q = {0};
	for(unsigned int n=0; n < 4; ++n) q[n] = (dReal)rotation[n];
	dGeomSetQuaternion(odeobj, q);
}*/

/*
 * Setter/getter method implementations
 */
void Geometry::setSpace(SceneObject* obj)
{
	// If we are already attached to another space object, tell it to remove us.
	if ( space != NULL )
		space->removeGeometry(odeobj);

	space = dynamic_cast<Space*>(obj);
	space->addGeometry(odeobj);
}

SceneObject* Geometry::getSpace() const
{
	return space;
}

// Get ODE object class
int Geometry::getClass() const
{
	return dGeomGetClass(odeobj);
}

// Get/set category bits
void Geometry::setCategoryBits(unsigned long bits)
{
	dGeomSetCategoryBits(odeobj, bits);
}

unsigned long Geometry::getCategoryBits() const
{
	return dGeomGetCategoryBits(odeobj);
}

// Get/set collide bits
void Geometry::setCollideBits(unsigned long bits)
{
	dGeomSetCollideBits(odeobj, bits);
}

unsigned long Geometry::getCollideBits() const
{
	return dGeomGetCollideBits(odeobj);
}

// Get/set enabled status
void Geometry::setEnable(int flag)
{
	if ( dGeomIsEnabled(odeobj) != flag )
	{
		if (flag == 0)
			dGeomDisable(odeobj);
		else
			dGeomEnable(odeobj);
	}
}

int Geometry::getEnable() const
{
	return dGeomIsEnabled(odeobj);
}

// Get position in world coordinates (as ODE sees it)
Vector3 Geometry::getPosition() const
{
	const dReal* ptr = dGeomGetPosition(odeobj);
	return Vector3((Real)ptr[0], (Real)ptr[1], (Real)ptr[2]);
}

// Get orientation in world coordinates (as ODE sees it)
Quaternion Geometry::getOrientation() const
{
	dQuaternion dq; dGeomGetQuaternion(odeobj, dq);
	return Quaternion((Real)dq[0], (Real)dq[1], (Real)dq[2], (Real)dq[3]);
}
