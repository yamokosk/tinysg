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

#ifndef _TINYSG_SCENEMGR_H_FILE_
#define _TINYSG_SCENEMGR_H_FILE_

#include "config.h"

// External includes
#include <string>
#include <map>

// Internal includes
#include "SceneNode.h"

namespace tinysg
{

class SceneGraph
{
#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

	static unsigned long NextGeneratedNameExt;

	typedef std::map<std::string, SceneObject*> ObjectMap;
	typedef std::map<std::string, SceneNode*> NodeMap;
	typedef std::map<std::string, Query*> QueryMap;
public:
	typedef MapIterator<ObjectMap> SceneObjectIterator;

	SceneGraph();
	~SceneGraph();

	// Name of world node
	static const std::string World;
	static int InvokeService(const char* serviceName, void* serviceParams);
	static std::string generateName()
	{
		std::stringstream ss; ss << "Unnamed" << NextGeneratedNameExt++;
		return ss.str();
	}

	// Override standard Singleton retrieval.
	//static SceneGraph& getInstance();

	// Node management
	SceneNode* createNode(const std::string& name);
	void destroyAllNodes();
	SceneNode* getNode(const std::string& name) const;

	// Scene management
	void clearScene();
	void update();
	void __deprecated_update();

	// Object management
	SceneObject* createObject(const ObjectInfo& info);
	SceneObject* createObject(const std::string& name, const std::string& type);
	SceneObject* createObject(const std::string& name, const std::string& type, const PropertyContainer& properties);
	SceneObject* getObject(const std::string& name) const;
	SceneGraph::SceneObjectIterator getAllObjects(void);
	unsigned int getNumObjects() const;

	// Query management
	QueryArguments executeQuery(const std::string& querytype);
	void executeQuery(const std::string& querytype, QueryArguments& args);

private:
	Query* createQuery(const std::string& type);

	SceneNodePtr rootNode_;
	NodeMap nodes_;
	ObjectMap objects_;
	QueryMap queries_;
};


struct SceneLoader
{
	static void save(const char* filename, SceneGraph& g);
	static void load(const char* filename, SceneGraph& g);
};

}

#endif
