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
 * demowrapper.h
 *
 *  Created on: Jul 9, 2009
 *      Author: yamokosk
 */

#ifndef DEMOWRAPPER_H_
#define DEMOWRAPPER_H_

#include <string>
#include <sstream>
#include <iostream>

// TinySG includes
#include <TinySG.h>
#include <NodeUtils.h>

using namespace tinysg;

#if defined( TSG_HAVE_LOG4CXX )

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

#define TSG_LOG_INFO(expr)				\
	LOG4CXX_INFO(logger, expr);

#define TSG_LOG_WARN(expr)				\
	LOG4CXX_WARN(logger, __FUNCTION__ << "(): " << expr);

#define TSG_LOG_ERROR(expr)				\
	LOG4CXX_ERROR(logger, __FUNCTION__ << "(): " << expr);

#else

#include <iostream>
#include <cstdio>

#define TSG_LOG_INFO(expr)

#define TSG_LOG_WARN(expr)				\
	std::cerr << __FUNCTION__ << "(): " << expr << std::endl;

#define TSG_LOG_ERROR(expr)				\
	std::cerr << __FUNCTION__ << "(): " << expr << std::endl;

#endif

#define VERIFY( statement )													\
	if ( (statement) )														\
	{																		\
		std::cout << "TRUE: " << #statement << std::endl;					\
	} else {																\
		std::cout << "Error! This is FALSE: " << #statement << std::endl;	\
	}


#define TRACE		\
	std::cout << __FUNCTION__ << "(): " << __LINE__ << std::endl;

/*
 * Overview
 *
 * Creates a very simple scene graph, attaches some 3D (ODE) objects to the
 * graph, and does a few collision queries.
 *
 * Details
 *
 * Support for ODE (Open dynamics engine) comes pre-built with TinySG, along
 * with support for another distance query algorithm (see other demo for details).
 * This demo shows how to create a very simple scene graph depicted below:
 *
 *		   World
 * 		   /  \
 * 		n1	   n2
 * 	   /
 *	 n3
 *
 * This just represents the graph's connectivity. Physically the nodes will be located
 * as follows:
 *
 *		   +y
 *			^
 * 			|
 * 			|
 * 			|
 * 			-	   n2(2,1)
 * 			|
 * 			|
 * 			|
 * 			|
 * 			-	   n1(1,1)	  	      n3(3,1)
 * 			|
 *  		|
 * 			|
 * 			|
 *			*---------|---------|---------|--> +x
 */
void createSimpleGraph(SceneGraph& graph);

// Defined by the individual demo programs
bool rundemo(int argc, char **argv);

#endif /* DEMOWRAPPER_H_ */
