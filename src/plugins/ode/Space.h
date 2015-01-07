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
 * Space.h
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#ifndef SPACE_H_
#define SPACE_H_

#include <api/ObjectModel.h>
#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>
using namespace obrsp::linalg;

// ODE library
#include <ode/ode.h>

//struct PF_PlatformServices;
#include <plugin_framework/Plugin.h>
using namespace obrsp::plugin;
using namespace tinysg;

class Space : public SceneObject
{
public:
 	Space();
	virtual ~Space();

	void removeGeometry(dGeomID id) {dSpaceRemove(odeobj, id);}
	void addGeometry(dGeomID id) {dSpaceAdd(odeobj, id);}
	dSpaceID getID() {return odeobj;}

	// Inherited from SceneObject
	void init( const ObjectInfo& info );
	Property getProperty(const std::string& name) const;
	void setProperty(const Property& p);
	virtual void notifyMoved( const Real* translation, const Real* rotation ) = 0;

	const PF_PlatformServices* services;

protected:
	virtual void initImpl( const ObjectInfo& info ) = 0;
	virtual Property getPropertyImpl( const std::string& name ) const;
	virtual void setPropertyImpl( const Property& prop );

	dSpaceID odeobj;
	std::string name;
};

#endif /* SPACE_H_ */
