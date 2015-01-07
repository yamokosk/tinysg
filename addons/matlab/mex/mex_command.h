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

#ifndef _SCENEML_MEX_COMMAND_H_FILE_
#define _SCENEML_MEX_COMMAND_H_FILE_

/*
 *	The following defines commands available via the mex interface.
 */
#define COMMANDS_DEF COMMAND(LoadScene, LoadScene)						\
					 COMMAND(SaveScene, SaveScene)						\
                     COMMAND(Update, Update)							\
                     COMMAND(GetNode, GetNode)							\
					 COMMAND(CreateChildNode, CommandNotImplemented)	\
					 COMMAND(CreateObject, CommandNotImplemented)		\
					 COMMAND(AttachObject, CommandNotImplemented)		\
					 COMMAND(TranslateNode, TranslateNode)				\
					 COMMAND(RotateNode, RotateNode)					\
					 COMMAND(CreatQuery, CommandNotImplemented)			\
					 COMMAND(DistanceQuery, DistanceQuery)				\
					 COMMAND(CollisionQuery, CollisionQuery)			\
					 COMMAND(GetAllObjects, GetAllObjects)				\
					 COMMAND(GetObject, GetObject)

enum command_indices {
#define	COMMAND(name, handler) COMMAND_##name,
	COMMANDS_DEF
#undef COMMAND
    COMMAND_COUNT /* The number of existing commands */
};

/*
 * .. and their respective handling functions.
 */
void handler_LoadScene (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_SaveScene (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_Update (int, mxArray *plhs[], int, const mxArray *prhs[]);

void handler_GetNode (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_CreateChildNode (int, mxArray *plhs[], int, const mxArray *prhs[]);

void handler_CreateObject (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_AttachObject (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GetObject (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_GetAllObjects (int, mxArray *plhs[], int, const mxArray *prhs[]);

void handler_TranslateNode (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_RotateNode (int, mxArray *plhs[], int, const mxArray *prhs[]);

void handler_CreateQuery (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_DistanceQuery (int, mxArray *plhs[], int, const mxArray *prhs[]);
void handler_CollisionQuery (int, mxArray *plhs[], int, const mxArray *prhs[]);

void handler_CommandNotImplemented (int, mxArray *plhs[], int, const mxArray *prhs[]);

typedef void (*command_handler_t)(int, mxArray *plhs[], int, const mxArray *prhs[]);

// Various utility functions
int getCommandID( const mxArray* );

#endif
