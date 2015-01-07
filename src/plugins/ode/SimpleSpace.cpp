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
 * SimpleSpace.cpp
 *
 *  Created on: Dec 11, 2008
 *      Author: yamokosk
 */

#include "SimpleSpace.h"
#include <plugin_framework/Plugin.h>
#include <api/Services.h>
#include <boost/foreach.hpp>

const std::string SimpleSpace::Type("ODESimpleSpace");

void* SimpleSpace::create(PF_ObjectParams* params)
{
	SimpleSpace* ptr = new SimpleSpace();
	ptr->services = params->platformServices;
	return ptr;
}

int SimpleSpace::destroy(void *p)
{
	if (!p) return -1;

	delete static_cast<SimpleSpace*>(p);

	return 0;
}

SimpleSpace::SimpleSpace() : Space()
{

}

SimpleSpace::~SimpleSpace()
{

}

void SimpleSpace::getInfo(ObjectInfo& info) const
{
	info.type = Type;
	info.name = name;
}

void SimpleSpace::initImpl(const ObjectInfo& info)
{
	// Set object name
	name = info.name;

	// Create the ODE Box object
	odeobj = dSimpleSpaceCreate (NULL);
	dSpaceSetCleanup (odeobj, 0);
	// If there are any applicable parameters, set them now
	BOOST_FOREACH(Property p, info.parameters)
	{
		setProperty(p);
	}
}

Property SimpleSpace::getPropertyImpl(const std::string& name) const
{
	// Have parent class deal with generic ODE properties
	return Space::getPropertyImpl(name);
}

void SimpleSpace::setPropertyImpl(const Property& p)
{
	// Have parent class deal with generic ODE properties
	Space::setPropertyImpl(p);
}

