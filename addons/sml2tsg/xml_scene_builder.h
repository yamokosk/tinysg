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

#ifndef _SCENEML_XML_SCENE_BUILDER_H_FILE_
#define _SCENEML_XML_SCENE_BUILDER_H_FILE_

//#include "config.h"
#include "sceneobjects.h"
#include "transform.h"
//#include "Primitive.h"
#include "scene_builder.h"

// Required Xerces headers
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

namespace sceneml {

XERCES_CPP_NAMESPACE_USE

typedef std::pair<std::string, dReal*> NamedTransform;
typedef std::list< NamedTransform > TransformList;

//#define VERBOSE_BUILDING

// ConcreteBuilder
//class SCENEML_API XML SceneBuilder : public SceneBuilder
class  XMLSceneBuilder : public SceneBuilder
{
public:
	XMLSceneBuilder(const char* filename);
	~XMLSceneBuilder();

	// Redefinition of parent functions
	void buildSpaces(tinysg::SceneGraph& graph);
	void buildBodies(tinysg::SceneGraph& graph);
	void buildGeoms(tinysg::SceneGraph& graph);

private:	
	XMLSceneBuilder();
	
	// XML specific functions and data members
	void readXMLDescription( );
	void parseTransform(DOMNode *node, const std::string& parentName, TransformList& transforms);
	void parseSimpleTransform(DOMNode *node, const std::string& parentName, TransformList& transforms);
	//CoordinateTransformPtr parseMarkerTransform(DOMNode *node, Body *b=NULL);
	void isFileValid();
	
	DOMImplementation* impl_;
	DOMBuilder* parser_;
	DOMDocument* domDoc_;
	
	char filename_[256];
};

};

#endif
