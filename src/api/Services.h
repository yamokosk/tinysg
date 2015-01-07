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
 * Services.h
 *
 *  Created on: Nov 21, 2008
 *      Author: yamokosk
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <string>

namespace tinysg {

typedef struct LogParams
{
	std::string filename;
	unsigned int line;
	std::string message;
} LogParams;

typedef struct ReportWarningParams
{
	std::string filename;
	unsigned int line;
	std::string message;
} ReportWarningParams;

typedef struct ReportErrorParams
{
	std::string filename;
	unsigned int line;
	std::string message;
} ReportErrorParams;

}  // namespace tinysg

#define LOG_MESSAGE(params, msg)		\
{										\
	tinysg::LogParams lp;				\
	lp.filename = __FILE__;				\
	lp.line = __LINE__;					\
	lp.message = msg;					\
	params->invokeService("log", &lp);	\
}

#define LOG_WARNING(params, msg)			\
{											\
	tinysg::ReportWarningParams lp;			\
	lp.filename = __FILE__;					\
	lp.line = __LINE__;						\
	lp.message = msg;						\
	params->invokeService("warning", &lp);	\
}

#define LOG_ERROR(params, msg)				\
{											\
	tinysg::ReportErrorParams lp;			\
	lp.filename = __FILE__;					\
	lp.line = __LINE__;						\
	lp.message = msg;						\
	params->invokeService("error", &lp);	\
}

#endif /* TINYSGSERVICES_H_ */
