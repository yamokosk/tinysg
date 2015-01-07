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
 * CollisionQuery.h
 *
 *  Created on: Dec 4, 2008
 *      Author: yamokosk
 */

#ifndef COLLISIONQUERY_H_
#define COLLISIONQUERY_H_

#define NUM_CONTACT_POINTS 1

#include <api/ObjectModel.h>

#include <ode/ode.h>

//struct PF_ObjectParams;
//struct PF_PlatformServices;
#include <plugin_framework/Plugin.h>
using namespace obrsp::plugin;
using namespace tinysg;

class CollisionQuery : Query
{
public:
	// static plugin interface
	static void* create(PF_ObjectParams *);
	static int destroy(void *);
	static const std::string Type;
	~CollisionQuery();

	// Inherited from Query
	virtual void getInfo(QueryInfo* info);
	virtual void execute(QueryArguments* arg);

	const PF_PlatformServices* services;

	// For collision checking
	static void collisionCallback(void* ptr, dGeomID o1, dGeomID o2);
private:
	CollisionQuery();

	QueryInfo info;
};

#endif /* COLLISIONQUERY_H_ */
