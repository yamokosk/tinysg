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

#include "mex_common.h"
#include "mex_command.h"
#include "mex_error.h"

// Global variables
bool g_bLibraryIsInit;
SceneGraphPtr g_SceneGraph;

extern command_handler_t command_handlers[];
extern const char *command_names[];

// Local function prototypes
void InitLibrary(void);
void MexExitFcn(void);

/*
 * Mex entry point
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	try {

		if (!g_bLibraryIsInit) InitLibrary();

		// Get command type
		int commandID = getCommandID( RHS_ARG_1 );

		// Get pointer to correct handler function and then handle this particular command
		command_handler_t cmdHandler = command_handlers[commandID];
		cmdHandler(nlhs, plhs, nrhs, prhs);

	} catch (const std::runtime_error& e) {
		std::ostringstream msg;
		msg << "Runtime error: " << e.what();
		ERROR_MSG(RUNTIME_ERROR, msg.str().c_str());
	}

	return;
}

void InitLibrary(void)
{
	if ( mexAtExit(MexExitFcn) ) {
		ERROR_MSG(RUNTIME_ERROR, "myExitFcn not registered properly");
	}

#if defined( TSG_HAVE_LOG4CXX )
	log4cxx::LoggerPtr  root_logger( log4cxx::Logger::getRootLogger() );
#	if defined( DEBUG_TINYSG )
	root_logger->setLevel(log4cxx::Level::getDebug());
#	else
	root_logger->setLevel(log4cxx::Level::getError());
#	endif
#endif
	tinysg::TinySG::Initialize();

	g_bLibraryIsInit = true;
}

void MexExitFcn(void)
{
	mexWarnMsgTxt("TinySG library being unloaded. Current scene will be destroyed.");

	g_SceneGraph.release();
	g_bLibraryIsInit = false;
}
