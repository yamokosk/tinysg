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
 * TinySG.h
 *
 *  Created on: Jul 10, 2009
 *      Author: yamokosk
 */

#ifndef TINYSG_H_
#define TINYSG_H_

#include "config.h"
#include "SceneGraph.h"
#include "api/ObjectModel.h"

namespace tinysg
{
struct TinySG
{
#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

	static void Initialize();
};
}

#endif /* TINYSG_H_ */
