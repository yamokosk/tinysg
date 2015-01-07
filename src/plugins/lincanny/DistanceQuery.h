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
 * DistanceQuery.h
 *
 *  Created on: Dec 5, 2008
 *      Author: yamokosk
 */

#ifndef DISTANCEQUERY_H_
#define DISTANCEQUERY_H_

#include <api/ObjectModel.h>

#include <plugin_framework/Plugin.h>
namespace plugin = obrsp::plugin;

//struct PF_ObjectParams;
//struct PF_PlatformServices;

class DistanceQuery : tinysg::Query
{
public:
	// static plugin interface
	static void* create(plugin::PF_ObjectParams *);
	static int destroy(void *);
	static const std::string Type;
	~DistanceQuery();

	// Inherited from Query
	virtual void init();
	virtual void getInfo(tinysg::QueryInfo* info);
	virtual void execute(tinysg::QueryArguments* arg);

	const plugin::PF_PlatformServices* services;

private:
	DistanceQuery();
};

#endif /* DISTANCEQUERY_H_ */
