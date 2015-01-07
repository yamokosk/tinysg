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
 * Space.cpp
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#include "Space.h"
#include <plugin_framework/Plugin.h>
#include <api/Services.h>

Space::Space()
{

}

Space::~Space()
{
	if (odeobj != NULL) dSpaceDestroy (odeobj);
}

void Space::init( const ObjectInfo& info )
{
	try {
		this->initImpl(info);
	} catch (const std::string& errmsg) {
		LOG_ERROR(services, "init() failed. Reason: " + errmsg);
	} catch (...) {
		LOG_ERROR(services, "init() failed. Unknown reason.");
	}
}

Property Space::getProperty(const std::string& name) const
{
	if (odeobj == NULL) {
		LOG_ERROR(services, "getProperty() failed. ODE space has not been initialized!");
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

void Space::setProperty(const Property& p)
{
	if (odeobj == NULL) {
		LOG_ERROR(services, "setProperty() failed. ODE space has not been initialized!");
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

Property Space::getPropertyImpl( const std::string& name ) const
{
	// Unknown/unsupported property
	throw std::string("Property \"" + name + "\" is unknown or unsupported by this object.");
}

void Space::setPropertyImpl( const Property& prop )
{
	// Unknown/unsupported property
	throw std::string("Property \"" + prop.name_str() + "\" is unknown or unsupported by this object.");
}
