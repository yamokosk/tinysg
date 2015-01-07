/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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
 * config.h
 *
 *  Created on: Nov 14, 2008
 *      Author: yamokosk
 */

//#ifndef CONFIG_H_
//#define CONFIG_H_

//----------------------------------------------------------------------------
// Logger settings
#if defined( TSG_HAVE_LOG4CXX )

#include <log4cxx/logger.h>

#define ADD_LOGGING_SUPPORT				\
	static log4cxx::LoggerPtr logger;

#define INITIALIZE_LOGGER(classname)	\
	log4cxx::LoggerPtr classname::logger(log4cxx::Logger::getLogger(#classname));

#define TSG_LOG_TRACE(expr)				\
	LOG4CXX_TRACE(logger, expr);

#define TSG_LOG_DEBUG(expr)				\
	LOG4CXX_DEBUG(logger, expr);

#define TSG_LOG_INFO(expr)				\
	LOG4CXX_INFO(logger, expr);

#define TSG_LOG_WARN(expr)				\
	LOG4CXX_WARN(logger, __FUNCTION__ << "(): " << expr);

#define TSG_LOG_ERROR(expr)				\
	LOG4CXX_ERROR(logger, __FUNCTION__ << "(): " << expr);

#define TSG_LOG_FATAL(expr)				\
	LOG4CXX_FATAL(logger, __FUNCTION__ << "(): " << expr);

#else

#include <iostream>
#include <cstdio>

#define ADD_LOGGING_SUPPORT

#define INITIALIZE_LOGGER(classname)

#define TSG_LOG_TRACE(expr)

#define TSG_LOG_DEBUG(expr)

#define TSG_LOG_INFO(expr)

#define TSG_LOG_WARN(expr)				\
	std::cout << __FUNCTION__ << "(): " << expr << std::endl;

#define TSG_LOG_ERROR(expr)				\
	std::cout << __FUNCTION__ << "(): " << expr << std::endl;

#define TSG_LOG_FATAL(expr)				\
	std::cout << __FUNCTION__ << "(): " << expr << std::endl;

#endif

/*namespace tinysg
{

#ifdef _USE_DOUBLE_PRECISION_
typedef double Real;
#else
typedef float Real;
#endif

} // End namespace TinySG
*/

// Define TinySG version
#define TINYSG_VERSION    ((TINYSG_VERSION_MAJOR << 16) | (TINYSG_VERSION_MINOR << 8) | TINYSG_VERSION_PATCH)

/*
 * Initial platform/compiler-related stuff to set.
 */
#define TINYSG_PLATFORM_WIN32 1
#define TINYSG_PLATFORM_LINUX 2
#define TINYSG_PLATFORM_APPLE 3

/*
 * Finds the current platform
 */
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define TINYSG_PLATFORM TINYSG_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#   define TINYSG_PLATFORM TINYSG_PLATFORM_APPLE
#else
#   define TINYSG_PLATFORM TINYSG_PLATFORM_LINUX
#endif

//----------------------------------------------------------------------------
// Windows Settings
#if TINYSG_PLATFORM == TINYSG_PLATFORM_WIN32

// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined( TINYSG_STATIC_LIB )
		// Linux compilers don't have symbol import/export directives.
#   	define SOEXPORT
#   else
#   	if defined( TINYSG_NONCLIENT_BUILD )
#       	define SOEXPORT __declspec( dllexport )
#   	else
#           if defined( __MINGW32__ )
#               define SOEXPORT
#           else
#       	    define SOEXPORT __declspec( dllimport )
#           endif
#   	endif
#	endif
// Win32 compilers use _DEBUG for specifying debug builds.
#   ifdef _DEBUG
#       define TINYSG_DEBUG_MODE 1
#   else
#       define TINYSG_DEBUG_MODE 0
#   endif

// Disable unicode support on MingW at the moment, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLKIT_UNICODE__ in _mingw.h
#if defined( __MINGW32__ ) && !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__)
#	    define TINYSG_UNICODE_SUPPORT 1
#   else
#       define TINYSG_UNICODE_SUPPORT 0
#   endif
#else
#	define TINYSG_UNICODE_SUPPORT 1
#endif

#endif
//----------------------------------------------------------------------------

//#endif
