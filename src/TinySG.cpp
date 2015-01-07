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
 * TinySG.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: yamokosk
 */

#include "TinySG.h"

#if defined( TSG_HAVE_LOG4CXX )
#include <log4cxx/patternlayout.h>
#include <log4cxx/fileappender.h>
using namespace log4cxx;
#endif

namespace tinysg {

#if defined( TSG_HAVE_LOG4CXX )
log4cxx::LoggerPtr TinySG::logger( log4cxx::Logger::getLogger("TinySG") );
#endif

void TinySG::Initialize()
{
#if defined( TSG_HAVE_LOG4CXX )
	static const LogString TTCC_CONVERSION_PATTERN(LOG4CXX_STR("%-5p (%r) %c - %m%n"));
	LayoutPtr layout(new PatternLayout(TTCC_CONVERSION_PATTERN));
	AppenderPtr file_appender(new FileAppender(layout, "tinysg.log", false));
	logger->addAppender( file_appender );
#endif

	TSG_LOG_INFO("TinySG library initializing.");

	// Calling SceneGraph::getInstance() will initialize the rest of the library
	// and load the plugin manager.
	//SceneGraph::getInstance();

	TSG_LOG_INFO("TinySG library initialized.");
}

}  // namespace tinysg
