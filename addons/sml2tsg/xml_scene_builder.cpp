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

#include "xml_scene_builder.h"
#include "xml_attributes.h"
//#include "MeshImport.h"
#include "matrix.h"
#include <TinySG.h>
#include <NodeUtils.h>

#include <fstream>

namespace sceneml
{

XMLSceneBuilder::XMLSceneBuilder() :
	SceneBuilder()
{
	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): XML platform initialization error. " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	}
	
	memset(filename_, '\0', sizeof(filename_));
}

XMLSceneBuilder::XMLSceneBuilder(const char* filename) :
	SceneBuilder()
{
	memset(filename_, '\0', sizeof(filename_));
	memcpy(filename_, filename, sizeof(char)*strlen(filename));

	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): XML platform initialization error. " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	}
	
	this->isFileValid();
	this->readXMLDescription();
}

XMLSceneBuilder::~XMLSceneBuilder()
{
	parser_->release();
	XMLPlatformUtils::Terminate();
}

void XMLSceneBuilder::isFileValid()
{
	std::fstream fp(filename_, std::fstream::in);  // declarations of streams fp_in and fp_out
	if (!fp.good()) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << filename_ << " is not a valid XML file.";
		throw std::runtime_error(msg.str());
	}
	fp.close();
}

void XMLSceneBuilder::readXMLDescription( )
{
	// Initialize DOM parser and set properties
	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99); // What is LS? Need comment here!  -J.D.
	impl_ = DOMImplementationRegistry::getDOMImplementation(tempStr);
	parser_ = ((DOMImplementationLS*)impl_)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

	// FUTURE WORK - Lookup and use features of the DOM parser.. sure there is
	// something useful I could use in there.  -J.D.
	//if (parser->canSetFeature(XMLUni::fgDOMValidation, true))
	//	parser->setFeature(XMLUni::fgDOMValidation, true);
	//if (parser->canSetFeature(XMLUni::fgDOMNamespaces, true))
	//	parser->setFeature(XMLUni::fgDOMNamespaces, true);
	//if (parser->canSetFeature((XMLUni::fgDOMDatatypeNormalization, true))
	//	parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);


	// optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
	// and set it to the builder
	//MyDOMErrorHandler* errHandler = new myDOMErrorHandler();
	//parser->setErrorHandler(errHandler);
	
	// Now parse the actual document
	try {
		domDoc_ = parser_->parseURI( filename_ );
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message << std::endl;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message << std::endl;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
	
	return;
}

void XMLSceneBuilder::buildSpaces(tinysg::SceneGraph& graph)
{
	try {
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("space", tagname, 29);
		DOMNodeList* allSpaceItems = domDoc_->getElementsByTagName(tagname);
		
		// Loop over all of the spaces
		for (unsigned int spaceIndex = 0; spaceIndex < allSpaceItems->getLength(); ++spaceIndex) 
		{
			DOMNode* thisSpaceItem = allSpaceItems->item(spaceIndex);
		
			AttributesDirector attribDirector;

			SpaceAttributesBuilder spaceAttribBuilder(thisSpaceItem);
			attribDirector.SetAttributesBuilder( &spaceAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();

			// With successfully parsed attributes we can build an ODE space
			dSpaceID spaceID = NULL, parentID = NULL;
			SceneObject* space = NULL;

			// Get parent if one exists
			//std::string spaceParent = attrib->getValAsStr("parent");
			//if ( spaceParent.compare("_NOPARENT_") != 0 ) {
			//	parentID = scene_->getSpace( spaceParent );
			//}
			
			// Get the type now so we build the correct type
			std::string spaceType = attrib->getValAsStr("type");				
			if ( spaceType.compare("simple") == 0 )
			{
				//spaceID = dSimpleSpaceCreate(parentID);
				space = graph.createObject(attrib->getValAsStr("name"), "ODESimpleSpace");
			}
			else if (spaceType.compare("hash") == 0)
			{
				dRealPtr center = attrib->getValAsVec("center", 3);
				dRealPtr extents = attrib->getValAsVec("extents", 3);
				float depth = attrib->getValAsReal("depth");
				
				//spaceID = dQuadTreeSpaceCreate(parentID, center.get(), extents.get(), (int)depth);
				std::cout << "The ODE plugin for TinySG only supports SimpleSpaces." << std::endl;
			}
			else if (spaceType.compare("quadtree") == 0)
			{
				//spaceID = dHashSpaceCreate(parentID);
				
				float minlevel = attrib->getValAsReal("minlevel");
				float maxlevel = attrib->getValAsReal("maxlevel");
				//dHashSpaceSetLevels(spaceID, (int)minlevel, (int)maxlevel);
				std::cout << "The ODE plugin for TinySG only supports SimpleSpaces." << std::endl;
			}
		
			//Then create a new space object
			//scene_->addSpace(attrib->getValAsStr("name"), spaceID);

			// Attach to world node
			if ( space != NULL )
				graph.getNode(tinysg::SceneGraph::World)->attach(space);
		}
		
		// Now that all spaces have been created, look for <pair> tags to
		// create collision pairs
		// Possibly turn collision pairs into persistent queries.
		XMLString::transcode("pair", tagname, 29);
		DOMNodeList* allPairItems = domDoc_->getElementsByTagName(tagname);
		QueryArguments args;
		
		for (unsigned int pairIndex = 0; pairIndex < allPairItems->getLength(); ++pairIndex)
		{
			DOMNode* thisPairItem = allPairItems->item(pairIndex);
			
			AttributesDirector attribDirector;
			
			PairAttributesBuilder pairAttribBuilder(thisPairItem);
			attribDirector.SetAttributesBuilder( &pairAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();

			SceneObject* s1 = graph.getObject( attrib->getValAsStr("space1") );
			SceneObject* s2 = graph.getObject( attrib->getValAsStr("space2") );

			// With successfully parsed attributes we can build an ODE space
			//scene_->addCollisionPair(attrib->getValAsStr("space1"), attrib->getValAsStr("space2"));
			args.parameters.push_back(
				Property("CollisionPair", SceneObjectPair(s1, s2)) );
		}
		
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const std::string& str) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << str;
		throw std::runtime_error(msg.str());
	} catch (const std::runtime_error &ex) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << ex.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
	//std::cout << "Finished building spaces." << std::endl;
}

void XMLSceneBuilder::buildBodies(tinysg::SceneGraph& graph)
{
	try
	{
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("body", tagname, 29);
		DOMNodeList* allBodyItems = domDoc_->getElementsByTagName(tagname);

		// Loop over all of the bodies
		for (unsigned int bodyIndex = 0; bodyIndex < allBodyItems->getLength(); ++bodyIndex) 
		{
			DOMNode* thisBodyItem = allBodyItems->item(bodyIndex);
		
			AttributesDirector attribDirector;

			BodyAttributesBuilder bodyAttribBuilder(thisBodyItem);
			attribDirector.SetAttributesBuilder( &bodyAttribBuilder );
			attribDirector.ConstructAttributes();

			AttributesPtr attrib = attribDirector.GetAttributes();
			
			//dBodyID b = dBodyCreate(scene_->getWorld());
			//Body* body = scene_->createBody(attrib->getValAsStr("name"), b);
			//Body* prox = scene_->getBody(attrib->getValAsStr("parent"));
			//body->setProxObj(prox);
			//prox->addDistBody(body);
			tinysg::SceneNode *parent = NULL, *child = NULL;
			//std::cout << "SML PARENT: " << attrib->getValAsStr("parent") << ", ";
			if ( attrib->getValAsStr("parent") == "world" )
			{
				parent = graph.getNode( tinysg::SceneGraph::World );
			}
			else
			{
				parent = graph.getNode( attrib->getValAsStr("parent") );
			}

			//CompositeTransformPtr pRootTransform( new CompositeTransform() );
			//parseTransform(thisBodyItem, pRootTransform.get(), body);
			//body->setCompositeTransform(pRootTransform);
			//body->invalidate();

			TransformList tlist;
			parseTransform(thisBodyItem, attrib->getValAsStr("name"), tlist);

			int nSteps = tlist.size();
			TransformList::iterator iter = tlist.begin();
			for (int n=0; n < nSteps; ++n)
			{
				child = parent->createChild(iter->first);
				dReal* dT = (iter->second);

				// Moves the node along the cartesian axes.
				// _T(0,3) = T[0*4+3] = T[3]
				// _T(1,3) = T[1*4+3] = T[7]
				// _T(2,3) = T[2*4+3] = T[11]

				//std::cout << "Parent: " << parent->getName() << ", child: " << child->getName() << std::endl;
				translate(child, dT[3], dT[7], dT[11], tinysg::TS_PARENT);

				dMatrix3 dR; dQuaternion dQ;
				dRFromT(dR, dT);
				dRtoQ (dR, dQ);

				rotate(child, dQ[0], dQ[1], dQ[2], dQ[3], tinysg::TS_PARENT);

				delete dT;
				parent = child;
				iter++;
			}

			child = parent->createChild( attrib->getValAsStr("name") );
#ifdef VERBOSE_BUILDING
			std::cout << "Parent: " << parent->getName() << ", child: " << child->getName() << std::endl << std::endl;
#endif
		}
		
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const std::string& str) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << str;
		throw std::runtime_error(msg.str());
	} catch (const std::runtime_error &ex) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << ex.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
#ifdef VERBOSE_BUILDING
	std::cout << "Finished building bodies." << std::endl;
#endif
}

void XMLSceneBuilder::buildGeoms(tinysg::SceneGraph& graph)
{
	try {
		// With a valid DOMDocument, its time to populate the Scene structure.
		XMLCh tagname[30];
		XMLString::transcode("geometry", tagname, 29);
		DOMNodeList* allGeomItems = domDoc_->getElementsByTagName(tagname);
		
		// Loop over all of the spaces
		for (unsigned int geomIndex = 0; geomIndex < allGeomItems->getLength(); ++geomIndex) 
		{
			DOMNode* thisGeomItem = allGeomItems->item(geomIndex);
		
			AttributesDirector attribDirector;

			GeomAttributesBuilder geomAttribBuilder(thisGeomItem);
			attribDirector.SetAttributesBuilder( &geomAttribBuilder );
			attribDirector.ConstructAttributes();
			AttributesPtr geomAttrib = attribDirector.GetAttributes();
			
			// Get geom's body
			DOMNode* thisGeomBodyItem = thisGeomItem->getParentNode();
			AttributesDirector bodyDirector;
			BodyAttributesBuilder bodyAttribBuilder(thisGeomBodyItem);
			bodyDirector.SetAttributesBuilder( &bodyAttribBuilder );
			bodyDirector.ConstructAttributes();
			AttributesPtr bodyAttrib = bodyDirector.GetAttributes();
			//Body* body = scene_->getBody(bodyAttrib->getValAsStr("name"));
			tinysg::SceneNode* parent = graph.getNode( bodyAttrib->getValAsStr("name") );
			
			// Get geom's space
			DOMNode* thisGeomSpaceItem = thisGeomBodyItem->getParentNode();
			AttributesDirector spaceDirector;
			SpaceAttributesBuilder spaceAttribBuilder(thisGeomSpaceItem);
			spaceDirector.SetAttributesBuilder( &spaceAttribBuilder );
			spaceDirector.ConstructAttributes();
			AttributesPtr spaceAttrib = spaceDirector.GetAttributes();
			//dSpaceID spaceID = scene_->getSpace(spaceAttrib->getValAsStr("name"));
			SceneObject* space = graph.getObject( spaceAttrib->getValAsStr("name") );
			
			// Construct the ODE object - based on type of course
			std::string type = geomAttrib->getValAsStr("type");
			//dGeomID g = NULL, t = NULL;
			//PolyhedronPtr mesh;
			PropertyContainer object_properties;
			object_properties.push_back( Property("space", space) );
			SceneObject* object = NULL;
			
			if (!type.compare("box"))
			{
				float length = geomAttrib->getValAsReal("length");
				float width = geomAttrib->getValAsReal("width");
				float height = geomAttrib->getValAsReal("height");
				//g = dCreateBox(NULL, length, width, height);
				object_properties.push_back( Property("lengths", Vector3(length, width, height)) );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODEBox", object_properties);
			}
			else if (!type.compare("ccylinder"))
			{
				float length = geomAttrib->getValAsReal("length");
				float radius = geomAttrib->getValAsReal("radius");
				//g = dCreateCCylinder(NULL, radius, length);
				object_properties.push_back( Property("length", length) );
				object_properties.push_back( Property("radius", radius) );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODECappedCylinder", object_properties);
			}
			else if (!type.compare("cylinder"))
			{
				float length = geomAttrib->getValAsReal("length");
				float radius = geomAttrib->getValAsReal("radius");
				//g = dCreateCylinder(NULL, radius, length);
				object_properties.push_back( Property("length", length) );
				object_properties.push_back( Property("radius", radius) );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODECylinder", object_properties);
			}
			else if (!type.compare("sphere"))
			{
				float radius = geomAttrib->getValAsReal("radius");
				//g = dCreateSphere(NULL, radius);
				object_properties.push_back( Property("radius", radius) );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODESphere", object_properties);
			}
			else if (!type.compare("plane"))
			{
				float nx = geomAttrib->getValAsReal("normal_x");
				float ny = geomAttrib->getValAsReal("normal_y");
				float nz = geomAttrib->getValAsReal("normal_z");
				float d = geomAttrib->getValAsReal("d");
				//g = dCreatePlane(NULL, nx, ny, nz, d);
				object_properties.push_back( Property("params", Vector4(nx,ny,nz,d)) );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODEPlane", object_properties);
			}
			else if (!type.compare("mesh"))
			{
				// Got to make a TriMeshObj!.. tough one.
				std::string filename = geomAttrib->getValAsStr("filename");
				/*int pos = filename.find_last_of(".");
				std::string extension = filename.substr(pos+1);
				float scale = geomAttrib->getValAsReal("scale");
				
				if (!extension.compare("obj")) {
					mesh.reset(new POLYHEDRON());
					mesh->filename = filename;
					if ( importOBJ(mesh.get(), scale) != 0 )
					{
						std::ostringstream msg;
						msg << "importOBJ(): Returned an error!" << std::endl;
						throw std::runtime_error(msg.str());
					}
				} else if (!extension.compare("stl")) {
					mesh.reset(new POLYHEDRON());
					mesh->filename = filename;
					if ( importSTL(mesh.get(), scale) != 0 )
					{
						std::ostringstream msg;
						msg << "importOBJ(): Returned an error!" << std::endl;
						throw std::runtime_error(msg.str());
					}
				} else {
					std::ostringstream msg;
					msg << extension << " is an unrecognized 3D model type. Currently only stl and obj files are supported." << std::endl;
					throw std::runtime_error(msg.str());
				}
				
				dTriMeshDataID Data = dGeomTriMeshDataCreate();
				dGeomTriMeshDataBuildSingle(Data,
                                 (void*)mesh->vertices.get(), mesh->vertex_stride, mesh->vertex_count,
                                 (void*)mesh->indices.get(),mesh->index_count,mesh->index_stride);
				g = dCreateTriMesh (NULL, Data, NULL, NULL, NULL);*/

				// Get color & alpha information for this mesh
				dRealPtr rgb = geomAttrib->getValAsVec("color", 3);
				for (int n=0; n < 3; ++n) (rgb.get())[n] *= (1/255.0);
				dReal alpha = geomAttrib->getValAsReal("alpha");

				Property mesh_properties("filename", filename);
				mesh_properties.add_parameter( Property("color", Vector3((rgb.get())[0], (rgb.get())[1], (rgb.get())[2])) );
				mesh_properties.add_parameter( Property("alpha", Real(alpha)) );

				object_properties.push_back( mesh_properties );
				object = graph.createObject(geomAttrib->getValAsStr("name"), "ODETriangleMesh", object_properties);
			}
			else
			{
				std::ostringstream msg;
				msg << type << " is an unrecognized geom type. Currently only stl and obj files are supported." << std::endl;
				throw std::runtime_error(msg.str());
			}
			
			// At this point the geom has NOT been added to the correct space. First
			// we need to check if there is a transform between this geom and its body.
			// That effects who is added to what space.
			//TransformList_t tlist = parseTransform(thisGeomItem);
			/*CompositeTransformPtr pTransform(new CompositeTransform());
			parseTransform(thisGeomItem, pTransform.get());
			
			//if (tlist.size() > 0) {
			if (pTransform->size() > 0) {
				// There exists a transform between the body and geom.. therefore
				// we need a transform geom in ODE to accurately represent this.
				t = dCreateGeomTransform(NULL);
				dGeomTransformSetGeom(t, g);
				if (type.compare("plane")) dGeomSetBody(t, body->id());
				dSpaceAdd(spaceID, t);
			} else {
				if (type.compare("plane")) dGeomSetBody(g, body->id());
				dSpaceAdd(spaceID, g);
			}
			
			Geom* geom = scene_->createGeom(geomAttrib->getValAsStr("name"), g, t);
			geom->setProxObj(body);
			geom->setMesh(mesh);
			geom->setCompositeTransform(pTransform);
			body->addGeom(geom);
			
			// Get color/alpha/collision info
			dRealPtr rgb = geomAttrib->getValAsVec("color", 3);
			for (int n=0; n < 3; ++n) (rgb.get())[n] *= (1/255.0);
			geom->setColor(rgb.get());
			geom->setAlpha( geomAttrib->getValAsReal("alpha") );
			geom->setCollisionCheck( geomAttrib->getValAsInt("checkcollision") );*/
			TransformList tlist;
			parseTransform(thisGeomItem, geomAttrib->getValAsStr("name"), tlist);

			int nSteps = tlist.size();
			if ( nSteps > 1)
			{
				std::cout << "Error! Geoms aren't allowed to have mutable variables." << std::endl;
			}
			else if ( nSteps == 1)
			{
				TransformList::iterator iter = tlist.begin();
				tinysg::SceneNode *child = parent->createChild(iter->first);
				dReal* dT = (iter->second);

				translate(child, dT[3], dT[7], dT[11], tinysg::TS_PARENT);

				dMatrix3 dR; dQuaternion dQ;
				dRFromT(dR, dT);
				dRtoQ (dR, dQ);

				rotate(child, dQ[0], dQ[1], dQ[2], dQ[3], tinysg::TS_PARENT);

				child->attach(object);

				delete dT;
			}
			else
			{
				parent->attach(object);
			}
			
			// Invalidate and move on to next geom
			//geom->invalidate();
		}
		
	} catch (const XMLException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const DOMException& toCatch) {
		std::ostringstream msg;
		char* message = XMLString::transcode(toCatch.getMessage());
		msg << __FUNCTION__ << "(): " << message;
		XMLString::release(&message);
		throw std::runtime_error(msg.str());
	} catch (const std::string& str) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << str;
		throw std::runtime_error(msg.str());
	} catch (const std::runtime_error &ex) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): " << ex.what();
		throw std::runtime_error(msg.str());
	} catch (...) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Unknown or unexpected error.";
		throw std::runtime_error(msg.str());
	}
#ifdef VERBOSE_BUILDING
	std::cout << "Finished building geoms." << std::endl;
#endif
}

//TransformList_t XMLSceneBuilder::parseTransform(DOMNode *node)
void XMLSceneBuilder::parseTransform(DOMNode *node, const std::string& parentName, TransformList& transforms)
{
	DOMNodeList *allChildItems = node->getChildNodes();

	for (unsigned int c=0; c < allChildItems->getLength(); ++c)
	{
		DOMNode *thisChildItem = allChildItems->item(c);
		
		if (thisChildItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			std::auto_ptr<char> pChildTagName( XMLString::transcode(thisChildItem->getNodeName()) );
						
			// Find the transform tag
			if (strcmp(pChildTagName.get(), "transform") == 0) 
			{
				AttributesDirector attribDirector;
				TransformAttributesBuilder builder(thisChildItem);
				
				attribDirector.SetAttributesBuilder( &builder );
				attribDirector.ConstructAttributes();
				
				AttributesPtr attrib = attribDirector.GetAttributes();
				
				std::string type = attrib->getValAsStr("type");
				if ( !type.compare("marker") )
				{
					std::cout << "Warning! Body " << parentName << " uses a marker transform. Feature not supported by TinySG (yet)." << std::endl;
					continue;
				}

				parseSimpleTransform(thisChildItem, parentName, transforms);
			}
		}
	} // End of FOR loop
	
	return;
}

void XMLSceneBuilder::parseSimpleTransform(DOMNode *node, const std::string& parentName, TransformList& transforms)
{
	//CompositeTransform* pSimpleTransform = new CompositeTransform();
	//CoordinateTransformPtr pSimpleTransform( new CompositeTransform() );
	
	int nodeCount = 0;
	CompositeTransformPtr pProxTransforms( new CompositeTransform() );

	DOMNodeList *allTransformItems = node->getChildNodes();

	for (unsigned int t=0; t < allTransformItems->getLength(); ++t) 
	{
		DOMNode *thisTransformItem = allTransformItems->item(t);
		
		if (thisTransformItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pTransformTagName = XMLString::transcode(thisTransformItem->getNodeName());

			// Parse the attributes.
			AttributesDirector attribDirector;
			AttributesPtr attrib; std::auto_ptr<AttributesBuilder> pBuilder;
			
			if ( strcmp(pTransformTagName, "translation") == 0 )
			{
				pBuilder.reset( new TranslationAttributesBuilder(thisTransformItem) );
			}
			else if ( strcmp(pTransformTagName, "rotation") == 0 )
			{
				pBuilder.reset( new RotationAttributesBuilder(thisTransformItem) );
			}
			else
			{
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Found an illegal tag " << pTransformTagName << " within a simple transformation.";
				XMLString::release(&pTransformTagName);
				throw std::runtime_error(msg.str());
			}
			attribDirector.SetAttributesBuilder( pBuilder.get() );
			attribDirector.ConstructAttributes();
			attrib = attribDirector.GetAttributes();
			
			// Create the transform object
			//SimpleTransform *aTransform = NULL;
			CoordinateTransformPtr aTransform;
			if ( strcmp(pTransformTagName, "translation") == 0 )
			{
				aTransform.reset( new SimpleTransform("translation", "_NODATA_") );
			}
			else if ( strcmp(pTransformTagName, "rotation") == 0 )
			{
				aTransform.reset( new SimpleTransform("rotation", attrib->getValAsStr("type")) );
			}
			XMLString::release(&pTransformTagName);

			// Add value
			dRealPtr value = attrib->getValAsVec("value", 3);
			((SimpleTransform*)aTransform.get())->setData(value);
			
			// If value is mutable, register it with the scene
			float checkIfMutable = attrib->getValAsReal("mutable");
			if ( !checkIfMutable )
			{
				// Transform is not mutable, so combine it with the previous transform
				pProxTransforms->add(aTransform);
			}
			else
			{
				// It is mutable. Need to do a couple of things here.
				//	1. We are done accumulating transforms into
				// 	into pProxTransforms. Push it into the list.
				if ( pProxTransforms->size() > 0 )
				{
					std::stringstream nodeName;
					nodeName << parentName << "_" << nodeCount++;
#ifdef VERBOSE_BUILDING
					std::cout << "- Adding intermediate transform: " << nodeName.str() << std::endl;
#endif
					dReal* dT = new dReal[16];
					memcpy(dT, pProxTransforms->getTMatrix(), sizeof(dMatrix4));

					// No need to add a node unless we have accumulated some.
					NamedTransform namedTransform(nodeName.str(), dT);
					transforms.push_back( namedTransform );
				}

				//	2. Create a single named transform. This will be the
				//	named transform the user can get access to later.
				{
#ifdef VERBOSE_BUILDING
					std::cout << "- Adding a mutable transform: " << attrib->getValAsStr("name") << std::endl;
#endif
					dReal* dT = new dReal[16];
					memcpy(dT, aTransform->compute(), sizeof(dMatrix4));

					NamedTransform namedTransform(attrib->getValAsStr("name"), dT);
					transforms.push_back( namedTransform );
				}

				//  3. Finally wipe out old pProxTransforms to
				//	accumulate more transforms into.
				pProxTransforms.reset( new CompositeTransform() );
			}
			
			// Add simple transform to the composite
			//((CompositeTransform*)pSimpleTransform.get())->add( aTransform );
		}   
	}

	// Cleanup. If we have any left over accumulated transforms, add them
	// to the list.
	if ( pProxTransforms->size() > 0 )
	{
		std::stringstream nodeName;
		nodeName << parentName << "_" << nodeCount++;
#ifdef VERBOSE_BUILDING
		std::cout << "- Adding intermediate transform: " << nodeName.str() << std::endl;
#endif

		dReal* dT = new dReal[16];
		memcpy(dT, pProxTransforms->getTMatrix(), sizeof(dMatrix4));
		NamedTransform namedTransform(nodeName.str(), dT);
		transforms.push_back( namedTransform );
	}
				
	//return transList;
	//return pSimpleTransform;
}

/*CoordinateTransformPtr XMLSceneBuilder::parseMarkerTransform(DOMNode *node, Body *b)
{
	//MarkerTransform* pMarkerTransform = new MarkerTransform();
	CoordinateTransformPtr pMarkerTransform( new MarkerTransform() );
	int nLocalCoord = 0, nGlobalCoord = 0;
	
	DOMNodeList *allTransformItems = node->getChildNodes();

	for (unsigned int t=0; t < allTransformItems->getLength(); ++t) 
	{
		DOMNode *thisTransformItem = allTransformItems->item(t);
		
		if (thisTransformItem->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* pTransformTagName = XMLString::transcode(thisTransformItem->getNodeName());
			if ( strcmp(pTransformTagName, "marker") != 0 ) 
			{
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Tag " << pTransformTagName << " inside a marker transformation is illegal.";
				throw std::runtime_error(msg.str());
			}
			XMLString::release(&pTransformTagName);
			
			// Get marker attributes
			AttributesDirector attribDirector;
			MarkerAttributesBuilder markerAttribBuilder(thisTransformItem);
			attribDirector.SetAttributesBuilder( &markerAttribBuilder );
			attribDirector.ConstructAttributes();
			AttributesPtr attrib = attribDirector.GetAttributes();
			
			dRealPtr value = attrib->getValAsVec("value", 3);
			std::string type = attrib->getValAsStr("type");
			if ( !type.compare("local") ){
				((MarkerTransform*)pMarkerTransform.get())->addLocalCoord( value );
				nLocalCoord++;
			} else if ( !type.compare("global") ) {
				((MarkerTransform*)pMarkerTransform.get())->addGlobalCoord( value );
				nGlobalCoord++;
			} else {
				std::ostringstream msg;
				msg << __FUNCTION__ << "(): Marker type " << type << " is illegal. It can either be local or global.";
				throw std::runtime_error(msg.str());
			}

			// If value is mutable, register it with the scene			
			float isMutable = attrib->getValAsReal("mutable");
			if ( isMutable ) {
				if (b == NULL) {
					std::ostringstream msg;
					msg << __FUNCTION__ << "(): Found a mutable variable for a geometry. This is not allowed!";
					throw std::runtime_error(msg.str());
				} else {
					scene_->addMutableValue(attrib->getValAsStr("name"), value, b);
				}
			}
		}   
	}
	
	if (nLocalCoord != nGlobalCoord) {
		std::ostringstream msg;
		msg << __FUNCTION__ << "(): Found " << nLocalCoord << " local and " << nGlobalCoord
		    << " global coordinates. These must be equal!";
		throw std::runtime_error(msg.str());
	}
		
	return pMarkerTransform;
}*/

}; // NAMESPACE
