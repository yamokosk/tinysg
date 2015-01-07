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
 * Geometry.h
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <api/ObjectModel.h>
#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>

using namespace obrsp::linalg;

// ODE library
#include <ode/ode.h>

//#define BUG_dGeomSetPosition
#ifdef BUG_dGeomSetPosition
#	include<iostream>
#endif

//#define BUG_trimesh_filename
#ifdef BUG_trimesh_filename
#	include<iostream>
#endif

//struct PF_PlatformServices;
#include <plugin_framework/Plugin.h>
using namespace obrsp::plugin;

using namespace tinysg;

class Space;

/*
 * Generic ODE geometry wrapper
 */
class Geometry : public SceneObject
{
public:
	Geometry();
	virtual ~Geometry();

	// SceneObject methods
	void init(const ObjectInfo& info);
	Property getProperty(const std::string& name) const;
	void setProperty(const Property& p);
	virtual void getInfo(ObjectInfo& info) const;
	virtual void notifyMoved( const Real* translation, const Real* rotation ) = 0;

	const PF_PlatformServices* services;

protected:
	virtual void initImpl(const ObjectInfo& info) = 0; // Must be implemented by derived classes
	virtual Property getPropertyImpl(const std::string& name) const;
	virtual void setPropertyImpl(const Property& p);

	// Get/set ODE space which this geom belongs to
	void setSpace(SceneObject*);
	SceneObject* getSpace() const;

	// Get ODE object class
	int getClass() const;

	// Get/set category bits
	void setCategoryBits(unsigned long bits);
	unsigned long getCategoryBits() const;

	// Get/set collide bits
	void setCollideBits(unsigned long bits);
	unsigned long getCollideBits() const;

	// Get/set enabled status
	void setEnable(int);
	int getEnable() const;

	// Get position in world coordinates (as ODE sees it)
	Vector3 getPosition() const;

	// Get orientation in world coordinates (as ODE sees it)
	Quaternion getOrientation() const;

	Space* space;
	dGeomID odeobj;
	std::string name;
};

#endif /* GEOMETRY_H_ */
