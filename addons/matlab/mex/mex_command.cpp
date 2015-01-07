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
#include "mex_struct_def.h"
#include "mex_conversion.h"

extern SceneGraphPtr g_SceneGraph;

const char *command_names[] = {
#define	COMMAND(name, handler) #name ,
	COMMANDS_DEF
#undef COMMAND
	NULL
};

command_handler_t command_handlers[] = {
#define COMMAND(name, handler) &handler_##handler ,
	COMMANDS_DEF
#undef COMMAND
    NULL
};

using namespace tinysg;

void handler_LoadScene (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) )
	{
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	// Release old scene if we need to
	if (g_SceneGraph.get() != NULL) g_SceneGraph.release();

	g_SceneGraph.reset( new SceneGraph );

	// XML scene description filename in RHS_ARG_2
	SceneLoader::load(StringMx::convert(RHS_ARG_2).c_str(), *g_SceneGraph);
	g_SceneGraph->__deprecated_update();
}

void handler_SaveScene (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	// XML scene description filename in RHS_ARG_2
	SceneLoader::save(StringMx::convert(RHS_ARG_2).c_str(), * g_SceneGraph);
}

void handler_Update (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	g_SceneGraph->__deprecated_update();
}

void handler_GetNode (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	try
	{
		MxSceneGraph mxSceneGraph( *g_SceneGraph );

		// Create output structure for Matlab and populate
		LHS_ARG_1 = mxSceneGraph.getNode(RHS_ARG_2);
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_CreateChildNode (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	/*if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

*/
}

void handler_CreateObject (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	/*if ( !mxIsStruct( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a structure.");
	}

	// Object info struct in RHS_ARG_2
	ObjectInfo info;
	mxArray_to_objectinfo(info, RHS_ARG_2);

	// Get the object
	//SceneGraph& graph = SceneGraph::getInstance();
	SceneObject* object = graph.createObject( info );

	// Create tmp mxArray to collect all the outputs
	mxArray *tmp[NUM_OBJECT_FIELDS];
	Scenesceneobject_to_mxArray(tmp, node);

	// Create output structure for Matlab and populate
	LHS_ARG_1 = mxCreateStructMatrix(1, 1, NUM_OBJECT_FIELDS, fnames_object);
	for (int n=0; n < NUM_OBJECT_FIELDS; ++n) {
		mxSetField(LHS_ARG_1, 0, fnames_object[n], tmp[n]);
	}*/
}

void handler_AttachObject (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	/*if ( !mxIsChar( RHS_ARG_2 ) ) {
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	if ( !mxIsChar( RHS_ARG_3 ) ) {
		ERROR_MSG(INVALID_ARG, "Third argument must be a char array.");
	}

	// Node name in RHS_ARG_2, object name in RHS_ARG_3
	char* nodeName = mxArrayToString( RHS_ARG_2 );
	char* objName = mxArrayToString( RHS_ARG_3 );

	// Get the node and object
	//SceneGraph& graph = SceneGraph::getInstance();
	SceneNode* node = NULL; SceneObject* object = NULL;
	if ( (node = graph.getNode( std::string(nodeName) )) == NULL )
	{
		mxFree(nodeName);
		mxFree(objName);
		ERROR_MSG(INVALID_ARG, "Node not found.");
	}

	if ( (object = graph.getObject( std::string(objName) )) == NULL )
	{
		mxFree(nodeName);
		mxFree(objName);
		ERROR_MSG(INVALID_ARG, "Object not found.");
	}

	node->attach(object);
	mxFree(nodeName);
	mxFree(objName);*/
}

void handler_GetObject (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) )
	{
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	try
	{
		MxSceneGraph mxSceneGraph( *g_SceneGraph );

		// Create output structure for Matlab and populate
		LHS_ARG_1 = mxSceneGraph.getObject(RHS_ARG_2);
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_GetAllObjects (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	try
	{
		MxSceneGraph mxSceneGraph( *g_SceneGraph );

		// Create output structure for Matlab and populate
		LHS_ARG_1 = mxSceneGraph.getAllObjects();
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_TranslateNode (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) )
	{
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	try
	{
		SceneNode* node = g_SceneGraph->getNode( StringMx::convert(RHS_ARG_2) );

		//translate(node, Vector3Mx::convert(RHS_ARG_3), tinysg::TS_PARENT);
		node->setPosition( Vector3Mx::convert(RHS_ARG_3) );
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_RotateNode (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) )
	{
		ERROR_MSG(INVALID_ARG, "Second argument must be a char array.");
	}

	try
	{
		SceneNode* node = g_SceneGraph->getNode( StringMx::convert(RHS_ARG_2) );

		//rotate(node, QuaternionMx::convert(RHS_ARG_3), tinysg::TS_LOCAL);
		node->setOrientation( QuaternionMx::convert(RHS_ARG_3) );
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_DistanceQuery (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	try
	{
		QueryArguments res = g_SceneGraph->executeQuery( "LCDistanceQuery" );

		LHS_ARG_1 = MxQueryArguments::convert(res);
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_CollisionQuery (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	if ( !mxIsChar( RHS_ARG_2 ) )
	{
		ERROR_MSG(INVALID_ARG, "Second argument must be a space name.");
	}

	if ( !mxIsChar( RHS_ARG_3 ) )
	{
		ERROR_MSG(INVALID_ARG, "Third argument must be a space name.");
	}

	try
	{
		SceneObject* s1 = g_SceneGraph->getObject( StringMx::convert(RHS_ARG_2) );
		SceneObject* s2 = g_SceneGraph->getObject( StringMx::convert(RHS_ARG_3) );

		if ( ( s1 != NULL ) && ( s2 != NULL) )
		{
			QueryArguments args;

			args.parameters.push_back(
					Property("CollisionPair", SceneObjectPair(s1, s2)) );
			g_SceneGraph->executeQuery( "ODECollisionQuery", args );
			LHS_ARG_1 = mxCreateDoubleScalar( (double)args.objectsInCollision.size() );
		}
		else
		{
			ERROR_MSG(INVALID_ARG, "One of the specified spaces does not exist.");
		}
	}
	catch (const std::string& msg)
	{
		ERROR_MSG(INVALID_ARG, msg.c_str());
	}
}

void handler_CommandNotImplemented (int, mxArray *plhs[], int, const mxArray *prhs[])
{
	ERROR_MSG(INVALID_ARG, "Command not yet implemented.");
}

// Various utility functions
int getCommandID( const mxArray* arg )
{
	if ( !mxIsChar( arg ) ) {
		ERROR_MSG(INVALID_ARG, "First argument must be the command type (char array).");
	}

	char* cmdname = mxArrayToString( arg );

	for (int n=0; n < COMMAND_COUNT; ++n)
	{
		if ( strcmp(cmdname, command_names[n]) == 0 ) {
			mxFree(cmdname);
			return n;
		}
	}

	mxFree(cmdname);
	ERROR_MSG(INVALID_ARG, "Command not found.");
	return 0;
}
