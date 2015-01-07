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

#include "SceneGraph.h"
#include "Archive.h"

#include <iostream>
#include <fstream>

#include "threadpool.hpp"
#include <boost/foreach.hpp>

#include <plugin_framework/PluginManager.h>
using namespace obrsp::plugin;

#include <api/Services.h>
#include <api/ObjectFactory.h>

// Static plugins
#if defined(TSG_HAVE_ODE)
#	include <ode/Plugin.h>
#endif

#include <lincanny/Plugin.h>

namespace tinysg
{

#if defined( TSG_HAVE_LOG4CXX )
log4cxx::LoggerPtr SceneGraph::logger( log4cxx::Logger::getLogger("TinySG.SceneGraph") );
#endif

// Static member initialization
unsigned long SceneGraph::NextGeneratedNameExt(0);
const std::string SceneGraph::World("_WORLD_");

/*SceneGraph& SceneGraph::getInstance()
{
	static SceneGraph instance;

	return instance;
}*/

int SceneGraph::InvokeService(const char* serviceName, void* serviceParams)
{
	if (::strcmp(serviceName, "log") == 0)
	{
		LogParams* lsp = static_cast<LogParams*>(serviceParams);
		TSG_LOG_INFO( lsp->filename << " on line " << lsp->line << ": " << lsp->message );
	}

	if (::strcmp(serviceName, "warning") == 0)
	{
		ReportWarningParams* lsp = static_cast<ReportWarningParams*>(serviceParams);
		TSG_LOG_WARN( lsp->filename << " on line " << lsp->line << ": " << lsp->message );
	}

	if (::strcmp(serviceName, "error") == 0)
	{
		ReportErrorParams* lsp = static_cast<ReportErrorParams*>(serviceParams);
		TSG_LOG_ERROR( lsp->filename << " on line " << lsp->line << ": " << lsp->message );
	}

	return 0;
}

SceneGraph::SceneGraph()
{
	TSG_LOG_DEBUG( "Scene graph initializing." );

	rootNode_.reset( new SceneNode(World) );
	rootNode_->setGraph(this);
	TSG_LOG_DEBUG( "Created world node." );

	// Initialize plugin manager
	PluginManager& pm = PluginManager::getInstance();
	pm.getPlatformServices().invokeService = InvokeService;

	// Load all dynamic plugins
	TSG_LOG_WARN( "Dynamic plugins DISABLED!" );
	//pm.loadAll(Directory::getCWD());
	//TSG_LOG_INFO( "Dynamic plugins loaded." );

	// Load any static plugins
	TSG_LOG_DEBUG( "Loading static plugins." );
#if defined(TSG_HAVE_ODE)
	PluginManager::initializePlugin(ODE_Plugin_InitPlugin);
#endif
	PluginManager::initializePlugin(LinCanny_Plugin_InitPlugin);
	TSG_LOG_DEBUG( "Static plugins loaded." );

	TSG_LOG_INFO( "SceneGraph initialized." );
}

SceneGraph::~SceneGraph()
{
	/*if ( PluginManager::getInstance().shutdown() < 0 )
	{
		TSG_LOG_WARN( "Error occured while shutting down plugin manager." );
	} else {
		TSG_LOG_INFO( "Plugin manager shutdown successfully." );
	}*/

	clearScene();

	TSG_LOG_INFO( "SceneGraph destroyed." );
}

SceneNode* SceneGraph::createNode(const std::string& name)
{
	SceneNode* node = new SceneNode(name);
	node->setGraph(this);

	nodes_[name] = node;
	TSG_LOG_DEBUG( "Created node \"" << name << "\"." );
	return node;
}

SceneNode* SceneGraph::getNode(const std::string& name) const
{
	if ( name == SceneGraph::World )
	{
		return rootNode_.get();
	} else {
		NodeMap::const_iterator iter = nodes_.find(name);

		if ( iter != nodes_.end() )
		{
			return iter->second;
		} else {
			TSG_LOG_WARN( "Node \"" << name << "\" does not exist in graph." );
			return NULL;
		}
	}
}

void SceneGraph::destroyAllNodes()
{
	TSG_LOG_INFO( "Destroying " << nodes_.size() << "." );
	std::pair<std::string, SceneNode*> pair;
	BOOST_FOREACH( pair, nodes_ )
	{
		delete pair.second;
		//n->removeAllChildren();
	}
	nodes_.clear();
	TSG_LOG_INFO( "All nodes destroyed." );
}

void SceneGraph::clearScene()
{
	// Clear root node of all children
	rootNode_->removeAllChildren();

	// Delete all SceneNodes, except root that is
	destroyAllNodes();

	TSG_LOG_INFO( "Scene cleared." );
}

void SceneGraph::update(void)
{
	TSG_LOG_DEBUG( "SceneGraph::update()" );

	// Create a thread pool with 4 threads
	boost::threadpool::pool threadPool(2);

	// Pass a refernce to the thread pool to all nodes so that they can schedule
	// themselves if need be.
	rootNode_->update(threadPool);

	threadPool.wait();	// wait until all partitions are sorted
}

void SceneGraph::__deprecated_update()
{
	TSG_LOG_DEBUG( "SceneGraph::__deprecated_update()" );
	rootNode_->__deprecated_update();
}

SceneObject* SceneGraph::createObject(const ObjectInfo& info)
{
	if ( objects_.find( info.name ) != objects_.end() )
	{
		// Error, object already exists in map
		TSG_LOG_ERROR( "The \"" << info.type
						<< "\" object named \"" << info.name
						<< "\" could not be created. Another object with the identical name already exists.");
		return NULL;
	}

	//SceneObject* obj = ObjectFactory::create(info);
	void* obj = PluginManager::getInstance().createObject(info.type);
	if ( obj == NULL )
	{
		// Error, plugin manager failed to create the object
		TSG_LOG_ERROR( "The \"" << info.type
						<< "\" object named \"" << info.name
						<< "\" could not be created. PluginManager returned a NULL object.");
		return NULL;
	}
	SceneObject* object = static_cast<SceneObject*>(obj);
	object->init(info);

	// Record object into the object tracking map
	objects_[info.name] = object;

	// Finally return the object
	return object;
}

SceneObject* SceneGraph::createObject(const std::string& name, const std::string& type)
{
	ObjectInfo info;
	info.name = name;
	info.type = type;
	return createObject(info);
}

SceneObject* SceneGraph::createObject(const std::string& name, const std::string& type, const PropertyContainer& properties)
{
	ObjectInfo info;
	info.name = name;
	info.type = type;
	info.parameters = properties;
	return createObject(info);
}

SceneObject* SceneGraph::getObject(const std::string& name) const
{
	ObjectMap::const_iterator iter = objects_.find(name);

	if ( iter != objects_.end() )
	{
		return iter->second;
	} else {
		TSG_LOG_ERROR( "The object named \"" << name
						 << "\" could not be found. Was it ever created in the first place?");
		return NULL;
	}
}

SceneGraph::SceneObjectIterator SceneGraph::getAllObjects()
{
	SceneGraph::SceneObjectIterator iter(objects_.begin(), objects_.end());
	return iter;
}

unsigned int SceneGraph::getNumObjects() const
{
	return (unsigned int)objects_.size();
}

QueryArguments SceneGraph::executeQuery(const std::string& querytype)
{
	QueryArguments args;
	this->executeQuery(querytype, args);
	return args;
}

void SceneGraph::executeQuery(const std::string& querytype, QueryArguments& args)
{
	QueryMap::iterator iter = queries_.find(querytype);
	Query* query = NULL;

	if ( iter == queries_.end() )
	{
		TSG_LOG_INFO( "Could not find query " << querytype << ". Creating one." );

		query = createQuery(querytype);
		if ( query != NULL )
		{
			query->init();
			queries_[querytype] = query;
		}
	}
	else
	{
		query = iter->second;
	}

	TSG_LOG_INFO( "Executing query: " << querytype );

	query->execute(&args);
}

Query* SceneGraph::createQuery(const std::string& type)
{
	void* obj = PluginManager::getInstance().createObject(type);
	if (obj != NULL)
	{
		TSG_LOG_INFO( "Created a \"" << type << "\" query." );
		Query* query = static_cast<Query*>(obj);
		return query;
	} else {
		TSG_LOG_WARN( "Requested query \"" << type << "\" could not be created." );
		return NULL;
	}
}


void SceneLoader::save(const char* filename, SceneGraph& g)
{
	OArchive ar(filename);
	ar.init();
	int version = 0;
	ar.serialize(g, version);
}

void SceneLoader::load(const char* filename, SceneGraph& g)
{
	IArchive ar(filename);
	ar.init();
	int version = 0;
	ar.serialize(g, version);
}

/*
const std::string SceneGraph::NodeType("node");

unsigned long SceneGraph::NextGeneratedNameExt(0);





SceneGraph::SceneGraph() :
	rootNode_(NULL),
	nodeFactory_( NodeType )
{
	registerFactory( &nodeFactory_ );
	rootNode_ = createNode(SceneGraph::World);
}

SceneGraph::~SceneGraph()
{
	clearScene();

	if (rootNode_ != NULL) delete rootNode_;
}

SceneNode* SceneGraph::createNode(const PropertyCollection* params)
{
	std::stringstream ss; ss << "unnamed_" << NextGeneratedNameExt++;
	return createNode(ss.str(), params);
}

SceneNode* SceneGraph::createNode(const std::string& name, const PropertyCollection* params)
{
	return createAndCastObject<SceneNode>(name, NodeType, params);
}

void SceneGraph::destroyNode(const std::string& name)
{
	destroyObject(name, NodeType);
}

void SceneGraph::destroyNode(SceneNode* node)
{
	destroyObject(node);
}

void SceneGraph::destroyAllNodes()
{
	destroyAllObjects( NodeType );
}

SceneNode* SceneGraph::getNode(const std::string& name) const
{
	return getAndCastObject<SceneNode>(name, NodeType);
}

void SceneGraph::clearScene(void)
{
	// Clear root node of all children
	rootNode_->removeAllChildren();

	// Delete all SceneNodes, except root that is
	destroyAllNodes();
}

void SceneGraph::update()
{
	// Cascade down the graph updating transforms & world bounds
	// In this implementation, just update from the root
	// Smarter SceneGraph subclasses may choose to update only
	//   certain scene graph branches
	rootNode_->update();
}

void SceneGraph::save(TinySG::Archive& ar)
{
	ObjectManager::ObjectCollection* oc = getCollection(NodeType);

	ar.createCollection("SceneGraph", oc->objects.size() );
	std::pair<std::string, Object*> pair;
	BOOST_FOREACH( pair, oc->objects )
	{
		SceneNode* node = boost::polymorphic_downcast<SceneNode*>(pair.second);
		ar.serializeObject("SceneGraph", *node );
	}
}

void SceneGraph::load(TinySG::Archive& ar)
{
	//Archive::Collection* c = ar.getCollection( "SceneGraph" );
	Archive::Collection* c = ar.getFirstCollection();

	if ( c != NULL )
	{
		for (unsigned int n=0; n < c->size(); ++n)
		{
			PropertyCollection pc = c->objects[n];
			createNode(pc.getValue("name"), &pc);
		}
	}
}*/

} // namespace TinySG
