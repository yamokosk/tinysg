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
 * Archive.cpp
 *
 *  Created on: Dec 9, 2008
 *      Author: yamokosk
 */

#include "Archive.h"

#include "SceneGraph.h"
#include "SceneNode.h"
#include "Stringify.h"
#include "NodeUtils.h"

#include <linalg/Vector3.h>
#include <linalg/Vector4.h>
#include <linalg/Quaternion.h>
#include <linalg/MathExpression.h>
using namespace obrsp::linalg;

namespace tinysg
{

// Static member initialization
const std::string Archive::NodeTagName("node");
const std::string Archive::PropertyTagName("property");
const std::string Archive::PositionTagName("position");
const std::string Archive::OrientationTagName("orientation");
const std::string Archive::ObjectTagName("object");
const std::string Archive::UnnamedValue("_UNNAMED_");
const std::string Archive::EmptyValue("_EMPTY_");
const std::string Archive::UnknownValue("_UNKNOWN_");

#if defined( TSG_HAVE_LOG4CXX )
log4cxx::LoggerPtr OArchive::logger( log4cxx::Logger::getLogger("TinySG.OArchive") );
log4cxx::LoggerPtr IArchive::logger( log4cxx::Logger::getLogger("TinySG.IArchive") );
#endif

/*
 * Base class implementation
 */
Archive::Archive(const std::string& f) :
	filename(f),
	currXmlElement(NULL),
	root(NULL),
	doc(filename.c_str())
{

}

/*
 * Output archive function implementations
 */
void OArchive::init()
{
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	// Create root node
	root = new TiXmlElement("TinySG");
	doc.LinkEndChild( root );
}

void OArchive::serialize(SceneGraph& graph, int& ver)
{
	// Write version
	std::stringstream ss; ss << ver;
	root->SetAttribute("version", ss.str().c_str() );

	// Create new node block
	currXmlElement = createXmlNode(root, Archive::NodeTagName);

	// Save node name
	addAttribute("name", SceneGraph::World);

	// Root has no parent.. so just pass on to specific SceneNode serialize function
	serialize(graph, *(graph.getNode(SceneGraph::World)), ver );

	// Responsible for writing file
	doc.SaveFile();
}

void OArchive::serialize(SceneGraph& graph, SceneNode& node, int& ver)
{
#ifdef DEBUG_TRACE
	std::cout << "Serializing node " << node.getName() << std::endl;
#endif
	// Record node position
	TiXmlElement* position = createXmlNode(currXmlElement, Archive::PositionTagName);
	addAttribute(position, "value", node.getPosition().toString() );

	// Record node orientation
	TiXmlElement* orientation = createXmlNode(currXmlElement, Archive::OrientationTagName);
	addAttribute(orientation, "format", "AngleAxis");
	Real angle; Vector3 axis;
	node.getOrientation().ToAngleAxis(angle, axis);
	std::stringstream ss; ss << angle;
	addAttribute(orientation, "angle", ss.str() );
	addAttribute(orientation, "axis", axis.toString() );

	// Record attached objects
	SceneObjectIterator objects = node.getAttachedObjects();
	while ( objects.hasMoreElements() )
	{
		SceneObject* object = objects.getNext();
		ObjectInfo info;
		object->getInfo(info);
		serialize(graph, info, ver );
	}

	// Record children
	SceneNode::ChildIterator children = node.getChildren();
	while ( children.hasMoreElements() )
	{
		SceneNode* child = children.getNext();

		// Create new node block
		currXmlElement = createXmlNode(root, Archive::NodeTagName);

		// Save node name
		addAttribute("name", child->getName());
		addAttribute("parent", node.getName());

		serialize(graph, *child, ver );
	}
#ifdef DEBUG_TRACE
	std::cout << "Node serialized: " << node.getName() << std::endl;
#endif
}

void OArchive::serialize(SceneGraph& graph, ObjectInfo& info, int& ver)
{
#ifdef DEBUG_TRACE
	std::cout << "Serializing object " << info.name << std::endl;
#endif
	// Record node position
	TiXmlElement* objectElement = createXmlNode(currXmlElement, Archive::ObjectTagName);
	addAttribute(objectElement, "name", info.name);
	addAttribute(objectElement, "type", info.type);

	// Get object properties
	BOOST_FOREACH( Property prop, info.parameters )
	{
		TiXmlElement* propElement = createXmlNode(objectElement, Archive::PropertyTagName);

#ifdef DEBUG_TRACE
		std::cout << "- Property: " << prop.name_str() << std::endl;
#endif
		addAttribute(propElement, "name", prop.name_str());
		StringTuple tuple = stringify(prop.const_value());
#ifdef DEBUG_TRACE
		if ( prop.name_str() == "filename" )
		{
			//std::cout << "-- filename: " << boost::any_cast<std::string>(prop.value()) << std::endl;
			std::cout << "-- filename: " << tuple.first << ", " << tuple.second << std::endl;
		}
#endif
		addAttribute(propElement, "class", tuple.first);
		addAttribute(propElement, "value", tuple.second);
	}
#ifdef DEBUG_TRACE
	std::cout << "Object Serialized: " << info.name << std::endl;
#endif
}

void OArchive::serialize(SceneGraph& graph, Property& prop, int& ver)
{

}

TiXmlElement* OArchive::createXmlNode(TiXmlElement* parent, const std::string& name)
{
	TiXmlElement* node = new TiXmlElement( name.c_str() );
	parent->LinkEndChild( node );
	return node;
}

void OArchive::addParameter( const std::string& name, const std::string& value )
{
	TiXmlElement* parameter = new TiXmlElement(Archive::PropertyTagName);
	parameter->SetAttribute("type", name.c_str());
	parameter->SetAttribute("value", value.c_str());
	currXmlElement->LinkEndChild(parameter);
}

void OArchive::addAttribute( const std::string& name, const std::string& value )
{
	currXmlElement->SetAttribute(name.c_str(), value.c_str());
}

void OArchive::addAttribute( TiXmlElement* element, const std::string& name, const std::string& value )
{
	element->SetAttribute(name.c_str(), value.c_str());
}


/*
 * Input archive implementations
 */
void IArchive::init()
{
	if ( !doc.LoadFile(filename.c_str()) )
	{
		// Throw error
		return;
	}

	TiXmlHandle hDoc(&doc);
	root = hDoc.FirstChildElement().Element();

	// should always have a valid root but handle gracefully if it does
	if ( root == NULL )
	{
		// Throw an error
		return;
	}

	// Check that its one of our XML files
	if ( strcmp(root->Value(), "TinySG") != 0 )
	{
		// Throw an error
		return;
	}
}

void IArchive::serialize(SceneGraph& graph, int& ver)
{
	// Set the version
	ver = 0;
	root->Attribute("version", &ver);

	// Get first node off root. Should be root node
	currXmlElement = root->FirstChildElement(Archive::NodeTagName);

	// Check this is indeed the _WORLD_ node
	std::string rootName = currXmlElement->Attribute("name");
	if ( rootName.compare(SceneGraph::World) != 0 )
	{
		throw std::string("World node was not the first node in the XML archive.");
	}

	// This node is automatically created when the SceneGraph object is created. So
	// there's nothing more to do but call the SceneNode specific serializer on the root node.
	serialize( graph, *(graph.getNode(SceneGraph::World)), ver );

	// Once Root node is done, load rest of the nodes
	currXmlElement = currXmlElement->NextSiblingElement(Archive::NodeTagName);
	while ( currXmlElement != NULL )
	{
		const char* nodeName = currXmlElement->Attribute("name");
		SceneNode* node = NULL;

		// If the node we are loading doesn't have a name or someone used the
		// '_UNNAMED_' variable, then we need to call a different member function
		// of the SceneGraph class. Otherwise use the specified name.
		if ( (nodeName == NULL) || (Archive::UnnamedValue == std::string(nodeName)) )
		{
			node = graph.createNode( SceneNode::generateName() );
		} else {
			node = graph.createNode( std::string(nodeName) );
		}
		//serialize( *node, ver );
		serialize( graph, *node, ver );
		currXmlElement = currXmlElement->NextSiblingElement(Archive::NodeTagName);
	}
}

void IArchive::serialize(SceneGraph& graph, SceneNode& node, int& ver)
{
	TiXmlElement* elementPtr = NULL;

	// Get parent name (if it has one)
	const char* parentName = currXmlElement->Attribute("parent");
	if ( parentName != NULL )
	{
		TSG_LOG_DEBUG( "Setting parent of " << node.getName() << " to " << parentName );
		graph.getNode(parentName)->addChild(&node);
		//node.setParent( graph.getNode(parentName) );
	}

	// Get node position
	elementPtr = currXmlElement->FirstChildElement(Archive::PositionTagName);
	std::string positionStr = elementPtr->Attribute("value");
	Vector3 pos = ExpressionFactory::getAsSequence<Vector3>(positionStr, 3);
	translate(&node, pos, tinysg::TS_PARENT);

	// Get node orientation
	elementPtr = currXmlElement->FirstChildElement(Archive::OrientationTagName);
	Real angle = ExpressionFactory::getAsReal(elementPtr->Attribute("angle"));
	Vector3 axis = ExpressionFactory::getAsSequence<Vector3>(elementPtr->Attribute("axis"), 3);
	//node.setOrientation( Quaternion(angle, axis) );
	rotate(&node, Quaternion(angle,axis), tinysg::TS_PARENT);

	// Load attached objects, but first store pointer to this node element
	TiXmlElement* thisNodeElement = currXmlElement;
	currXmlElement = currXmlElement->FirstChildElement(Archive::ObjectTagName);
	while ( currXmlElement != NULL )
	{
		ObjectInfo info;
		serialize(graph, info, ver);
		SceneObject* sceneObject = graph.createObject(info);

		node.attach( sceneObject );

		// Get/process next serialized scene object
		currXmlElement = currXmlElement->NextSiblingElement(Archive::ObjectTagName);
	}

	// Restore the currXmlElement back to this node element.
	currXmlElement = thisNodeElement;
}

void IArchive::serialize(SceneGraph& graph, ObjectInfo& info, int& ver)
{
	const char* objectName = currXmlElement->Attribute("name");
	const char* objectType = currXmlElement->Attribute("type");
	std::string objectNameStr("");

	// First, if there is no type we have a problem.. report the problem and skip
	// to the next element
	if ( objectType == NULL )
	{
		TSG_LOG_ERROR( filename << " (" << currXmlElement->Row() << "): Object type not specified.");
		return;
	}

	// Similar to the nodes above, if the object we are loading doesn't have a name
	// or someone used the '_UNNAMED_' variable, then we need to generate one.
	if ( (objectName == NULL) || (std::string(objectName) == Archive::UnnamedValue) )
	{
		objectNameStr = std::string(objectType) + "_" + SceneGraph::generateName();
	} else {
		objectNameStr = objectName;
	}

	// Parse property tags to finish the creation of this object info
	info.type = objectType;
	info.name = objectNameStr;

	// Remember where we are so we can come back to it.
	TiXmlElement* elementMemory = currXmlElement;

	// NEW WAY
	currXmlElement = currXmlElement->FirstChildElement(Archive::PropertyTagName);
	while (currXmlElement != NULL)
	{
		Property parameter("unknown");
		serialize(graph, parameter, ver);

		// Add parameter to property
		info.addProperty(parameter);

		// Get/process next property element
		currXmlElement = currXmlElement->NextSiblingElement(Archive::PropertyTagName);
	}

	// OLD WAY
	/*TiXmlElement* paramElement = currXmlElement->FirstChildElement(Archive::PropertyTagName);
	while (paramElement != NULL)
	{
		std::string paramName = paramElement->Attribute("name");
		std::string paramClass = paramElement->Attribute("class");
		std::string paramValue = paramElement->Attribute("value");

		// First check that the class is not empty or unknown. If it is, just skip to the next
		// property.
		if ( (paramClass == Archive::UnknownValue) || (paramClass == Archive::EmptyValue) )
		{
			paramElement = paramElement->NextSiblingElement(Archive::PropertyTagName);
			continue;
		}

		// Otherwise it is a property we should be able to load
		if ( paramClass == "scene_object_ptr" )
		{
			boost::any sceneObject = boost::any( graph.getObject(paramValue) );
			info.addProperty( Property(paramName, sceneObject) );
		}
		else
		{
			info.addProperty( Property(paramName, destringify(paramClass, paramValue)) );
		}

		// Get/process next property element
		paramElement = paramElement->NextSiblingElement(Archive::PropertyTagName);
	}*/

	// Restore where we left off
	currXmlElement = elementMemory;

	// Create object type from the info structure
	TSG_LOG_INFO( "Creating scene object \"" << info.name << "\"." );
}

void IArchive::serialize(SceneGraph& graph, Property& prop, int& ver)
{
	std::string paramName = currXmlElement->Attribute("name");
	std::string paramClass = currXmlElement->Attribute("class");
	std::string paramValue = currXmlElement->Attribute("value");

	prop.set_name(paramName);

	// First check that the class is not empty or unknown. If it is, just skip to the next
	// property.
	if ( (paramClass == Archive::UnknownValue) || (paramClass == Archive::EmptyValue) )
	{
		//paramElement = paramElement->NextSiblingElement(Archive::PropertyTagName);
		prop.set_value(Archive::UnknownValue);
		return;
	}

	// Otherwise it is a property we should be able to load
	if ( paramClass == "scene_object_ptr" )
	{
		boost::any sceneObject = boost::any( graph.getObject(paramValue) );
		prop.set_value(sceneObject);
	}
	else
	{
		prop.set_value( destringify(paramClass, paramValue) );
	}

	// Remember where we are so we can come back to it.
	TiXmlElement* elementMemory = currXmlElement;

	// Recurse over sub-properties
	currXmlElement = currXmlElement->FirstChildElement(Archive::PropertyTagName);
	while (currXmlElement != NULL)
	{
		Property parameter("unknown");
		serialize(graph, parameter, ver);

		// Add parameter to property
		prop.add_parameter(parameter);

		// Get/process next property element
		currXmlElement = currXmlElement->NextSiblingElement(Archive::PropertyTagName);
	}

	currXmlElement = elementMemory;
}

} // end namespace tinysg
