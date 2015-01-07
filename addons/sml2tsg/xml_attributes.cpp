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

#include "xml_attributes.h"

// Required math expression parser
#include <muParser.h>

namespace sceneml {

/*class Attributes::MathParserImpl
{
public:
	MathParserImpl();
	virtual ~MathParserImpl() {};
	void parseValue(const char* str, float* val);
private:
	mu::Parser parser_;	
};

Attributes::MathParserImpl::MathParserImpl() 
{
	parser_.DefineConst("pi", (double)M_PI);
}

void Attributes::MathParserImpl::parseValue(const char* str, float* val)
{
	char cstr[512], *tok = NULL;
	std::vector<float> retVal;
	void *f = NULL;
	
	// Copy input string to char array
	memset(cstr, '\0', sizeof(cstr));
	memcpy(cstr, str, strlen( str ) );
	
	// Step through each token, evaluate it, and store it in our return vector
	tok = strtok(cstr, " ,");
	int n = 0;
	while (tok != NULL) 
	{		
		try {
			parser_.SetExpr(tok);
		
			// Evaluate string
			val[n] = (float)parser_.Eval();
		} catch (mu::Parser::exception_type &e) {
			val[n] = 0.0;
			std::cout << e.GetMsg() << std::endl;
		}
		
		// Get new token
		tok = strtok(NULL, " ,");
		n++;
	}
	return;
}*/

//Attributes::Attributes() : pimpl_( new MathParserImpl )
Attributes::Attributes()
{
	parser_.DefineConst("pi", (double)My_PI);
}

Attributes::~Attributes() 
{
	properties_.clear();
}

void Attributes::add(const std::string& name, const std::string &val)
{
	properties_t::iterator it = properties_.find(name);
	if (it == properties_.end()) {
		properties_[name] = val;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "() - Parameter " + name + " already exists in attributes.";
		throw std::runtime_error(msg.str());
	}
}

void Attributes::update(const std::string& name, const std::string &val)
{
	properties_t::iterator it = properties_.find(name);
	if (it != properties_.end()) {
		properties_[name] = val;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "() - Parameter " + name + " not found in attributes.";
		throw std::runtime_error(msg.str());
	};
}

std::string Attributes::get(const std::string& name)
{
	properties_t::iterator it = properties_.find(name);
	if (it != properties_.end()) {
		return it->second;
	} else {
		std::ostringstream msg;
		msg << __FUNCTION__ << "() - Parameter " + name + " not found in attributes.";
		throw std::runtime_error(msg.str());
	};
}

float* Attributes::get(const std::string& name, int length)
{
	std::string str = this->get(name);
	
	float *val = new float[length];
	this->parseValue(str.c_str(), val, length);
	return val;
}

void Attributes::parseValue(const char* str, float* val, int length)
{
	//pimpl_->parseValue(str, val);
	char cstr[512], *tok = NULL;
	std::vector<float> retVal;
	void *f = NULL;
	
	// Copy input string to char array
	memset(cstr, '\0', sizeof(cstr));
	memcpy(cstr, str, strlen( str ) );
	
	// Step through each token, evaluate it, and store it in our return vector
	tok = strtok(cstr, " ,");
	for (int n=0; n < length; ++n)
	{
		if ( tok == NULL ) {
			//std::ostringstream msg;
			//msg << __FUNCTION__ << "(): Requested vector length (" << length
			//    << ") appearantly exceeds the number of tokens in the attribute: "
			//    << cstr;
			//throw std::runtime_error(msg.str());
			val[n] = 0.0;
		} else {
			try {
				parser_.SetExpr(tok);
			
				// Evaluate string
				val[n] = (float)parser_.Eval();
			} catch (mu::Parser::exception_type &e) {
				val[n] = 0.0;
				std::cout << e.GetMsg() << std::endl;
			}
		
			// Get new token
			tok = strtok(NULL, " ,");
		}
	}
	return;
}

// ----------------------------------------------------------------------------
//	Base Attributes Builder
// ----------------------------------------------------------------------------
void AttributesBuilder::getAttributes()
{
	if ( node_->hasAttributes() ) {
		DOMNamedNodeMap* theAttributes = node_->getAttributes();
		int numAttributes = theAttributes->getLength();
	
		for (int n=0; n < numAttributes; ++n) {
			DOMNode* attrib = theAttributes->item(n);

			const XMLCh* xmlch_Name = attrib->getNodeName();
			char* attrName = XMLString::transcode(xmlch_Name);

			const XMLCh* xmlch_Value = attrib->getNodeValue();
			char* attrValue = XMLString::transcode(xmlch_Value);
		
			try {
				//attrib_->update(pname.get(), pval.get());
				attrib_->update(attrName, attrValue);
			} catch (std::runtime_error &ex) {
				std::cerr << ex.what() << std::endl
						<< builderType_ << __FUNCTION__ << "(): "
						<< "In space \"" << attrib_->getValAsStr("name")
						<< "\", unrecognized attribute \"" << attrName << "=" << attrValue
						<< "\". Ignoring it." << std::endl;
			};

			XMLString::release( &attrName );
			XMLString::release( &attrValue );
		}
		
	}
}

// ----------------------------------------------------------------------------
//	Space Attributes Builder
// ----------------------------------------------------------------------------
SpaceAttributesBuilder::SpaceAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "SpaceAttributesBuilder") 
{ }

void SpaceAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("name", "_NODATA_");
	attrib_->add("parent", "_NOPARENT_");
	attrib_->add("type", "simple");
}

void SpaceAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void SpaceAttributesBuilder::getParameters()
{
	// First set default parameters.
	// The parameters are dependent on the type of space
	//	simple:	No parameters
	//	hash:	center,	dVector3
	//			extents,	dVector3
	//			depth,	int
	//	quatree:	minlevel,	int
	//			maxlevel,	int
	std::string type = attrib_->getValAsStr("type");
	
	if (!type.compare("simple")) return; // No parameters for simple space
	else if (!type.compare("hash")) {
		attrib_->add("center", "0 0 0");
		attrib_->add("extents", "0 0 0");
		attrib_->add("depth", "4");
	} else if (!type.compare("quadtree")) {
		attrib_->add("minlevel", "-1");
		attrib_->add("maxlevel", "8");
	} else {
		return;
	}
	
	DOMNodeList* allChildNodes = node_->getChildNodes();

	// Loop over all of the spaces
	for (unsigned int c = 0; c < allChildNodes->getLength(); ++c) {
		DOMNode* thisChildItem = allChildNodes->item(c);
			    			
		if (thisChildItem->getNodeType() == DOMNode::ELEMENT_NODE) {
		
			char* pChildTagName = XMLString::transcode(thisChildItem->getNodeName());
			
			if ( strcmp(pChildTagName, "parameter") == 0 ) {
     			
				if ( thisChildItem->hasAttributes() ) {
					DOMNamedNodeMap* theAttributes = thisChildItem->getAttributes();
					int numAttributes = theAttributes->getLength();
			
					if (numAttributes == 2) {
						DOMNode* nodeName = theAttributes->item(0);
						DOMNode* nodeValue = theAttributes->item(1);

						const XMLCh* xmlch_Name = nodeName->getNodeValue();
						char* attrName = XMLString::transcode(xmlch_Name);

						const XMLCh* xmlch_Value = nodeValue->getNodeValue();
						char* attrValue = XMLString::transcode(xmlch_Value);

						
						try {
							attrib_->update(attrName, attrValue);
						} catch (std::runtime_error &ex) {
							std::cerr << ex.what() << std::endl
									<< builderType_ << "::getParameters() - "
									<< "In space \"" << attrib_->getValAsStr("name")
									<< "\", parameter \"" << attrName << "=" << attrValue
									<< "\" is illegal for this space type ("
									<< attrib_->getValAsStr("type")
									<< "). Ignoring it." << std::endl;
						}

						XMLString::release( &attrName );
						XMLString::release( &attrValue );
					}
				}
			}
			
			delete [] pChildTagName;
		}
	}
}

void SpaceAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string name = attrib_->getValAsStr("name");
	if (!name.compare("_NODATA_"))
		throw std::runtime_error(builderType_ + "::verify() - A space has no name!");
}

// ----------------------------------------------------------------------------
//	Body Attributes Builder
// ----------------------------------------------------------------------------
BodyAttributesBuilder::BodyAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "BodyAttributesBuilder") 
{
	
}

void BodyAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("name", "_NODATA_");
	attrib_->add("parent", "_NODATA_");
}

void BodyAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void BodyAttributesBuilder::getParameters()
{
	// Bodies currently have no parameters
	return;
}

void BodyAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string name = attrib_->getValAsStr("name");
	if (!name.compare("_NODATA_"))
		throw std::runtime_error(builderType_ + "::verify() - A body has no name!");
}


// ----------------------------------------------------------------------------
//	Geom Attributes Builder
// ----------------------------------------------------------------------------
GeomAttributesBuilder::GeomAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "GeomAttributesBuilder") 
{
	
}

void GeomAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("name", "_NODATA_");
	attrib_->add("parent", "_NODATA_");
	attrib_->add("type", "_NODATA_");
	attrib_->add("color", "1.0 1.0 1.0");
	attrib_->add("scale", "1.0");
	attrib_->add("alpha", "1.0");
	attrib_->add("checkcollision", "1.0");
}

void GeomAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void GeomAttributesBuilder::getParameters()
{
	// First set default parameters.
	// The parameters are dependent on the type of geom
	//	box:			length, width, height (all doubles)
	//	ccylinder:	radius, length (all doubles)
	//	sphere:		radius (all doubles)
	//	plane:		normal_x, normal_y, normal_z, d (all doubles)
	//	mesh:		filname (std::string)	
	std::string type = attrib_->getValAsStr("type");
	
	if (!type.compare("box")) {
		attrib_->add("length", "1");
		attrib_->add("width", "1");
		attrib_->add("height", "1");
	} else if (!type.compare("ccylinder")) {
		attrib_->add("radius", "1");
		attrib_->add("length", "3");
	} else if (!type.compare("cylinder")) {
		attrib_->add("radius", "1");
		attrib_->add("length", "3");
	} else if (!type.compare("sphere")) {
		attrib_->add("radius", "1");
	} else if (!type.compare("plane")) {
		attrib_->add("normal_x", "0");
		attrib_->add("normal_y", "0");
		attrib_->add("normal_z", "1");
		attrib_->add("d", "0");
	} else if (!type.compare("mesh")) {
		attrib_->add("filename", "_NODATA_");
	} else {
		return;
	}
	
	DOMNodeList* allChildNodes = node_->getChildNodes();

	// Loop over all of the parameters
	for (unsigned int c = 0; c < allChildNodes->getLength(); ++c) {
		DOMNode* thisChildItem = allChildNodes->item(c);
			    			
		if (thisChildItem->getNodeType() == DOMNode::ELEMENT_NODE) {
		
			char* pChildTagName = XMLString::transcode(thisChildItem->getNodeName());
			
			if ( strcmp(pChildTagName, "parameter") == 0 ) {
     			
				if ( thisChildItem->hasAttributes() ) 
				{
					DOMNamedNodeMap* theAttributes = thisChildItem->getAttributes();
					int numAttributes = theAttributes->getLength();
			
					if (numAttributes == 2) { // Parameters can only 1 Name/Value pair
						DOMNode* nodeName = theAttributes->item(0);
						DOMNode* nodeValue = theAttributes->item(1);

						const XMLCh* xmlch_Name = nodeName->getNodeValue();
						char* attrName = XMLString::transcode(xmlch_Name);

						const XMLCh* xmlch_Value = nodeValue->getNodeValue();
						char* attrValue = XMLString::transcode(xmlch_Value);
						
						try {
							attrib_->update(attrName, attrValue);
						} catch (std::runtime_error &ex) {
							std::cerr << ex.what() << std::endl
									<< builderType_ << "::getParameters() - "
									<< "In geom \"" << attrib_->getValAsStr("name")
									<< "\", parameter \"" << attrName << "=" << attrValue
									<< "\" is illegal for this geom type ("
									<< attrib_->getValAsStr("type")
									<< "). Ignoring it." << std::endl;
						};
						
						XMLString::release( &attrName );
						XMLString::release( &attrValue );		
					}
				}
			}
			
			delete [] pChildTagName;
		}
	}
}

void GeomAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string name = attrib_->getValAsStr("name");
	if (!name.compare("_NODATA_"))
		throw std::runtime_error(builderType_ + "::verify() - A geom has not been named!");
		
	std::string type = attrib_->getValAsStr("type");
	if (!name.compare("_NODATA_"))
		throw std::runtime_error(builderType_ + "::verify() - Geom (" + name + ") has no type!");
		
	if (!type.compare("mesh")) {
		std::string filename = attrib_->getValAsStr("filename");
		
		if (!filename.compare("_NODATA_")) {
			throw std::runtime_error(builderType_ + "::verify() - Geom (" + name + ") is a mesh but no mesh filename was given."); 
		}
	}
}

// ----------------------------------------------------------------------------
//	Translation Attributes Builder
// ----------------------------------------------------------------------------
TranslationAttributesBuilder::TranslationAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "TranslationAttributesBuilder") 
{

}

void TranslationAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	// type="z" value="0" mutable="true" name
	attrib_->add("type", "_NODATA_"); // Type is not defined for translations.. but its here for consistency
	attrib_->add("value", "0 0 0");
	attrib_->add("mutable", "0");
	attrib_->add("name", "_NODATA_");
}

void TranslationAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void TranslationAttributesBuilder::getParameters()
{
	// No parameters
	return;
}

void TranslationAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string isMutable = attrib_->getValAsStr("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = attrib_->getValAsStr("name");
		
		if (!name.compare("_NODATA_"))
			throw std::runtime_error(builderType_ + "::verify() - A translation is marked mutable but has no name!");
	}
}

// ----------------------------------------------------------------------------
//	Rotation Attributes Builder
// ----------------------------------------------------------------------------
RotationAttributesBuilder::RotationAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "RotationAttributesBuilder") 
{

}

void RotationAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("type", "_NODATA_");
	attrib_->add("value", "0");
	attrib_->add("mutable", "0");
	attrib_->add("name", "_NODATA_");
}

void RotationAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void RotationAttributesBuilder::getParameters()
{
	// No parameters
	return;
}

void RotationAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string isMutable = attrib_->getValAsStr("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = attrib_->getValAsStr("name");
		
		if (!name.compare("_NODATA_"))
			throw std::runtime_error(builderType_ + "::verify() - A rotation is marked mutable but has no name!");
	}
	
	std::string type = attrib_->getValAsStr("type");
	if (!type.compare("_NODATA_")) {
		throw std::runtime_error(builderType_ + "::verify() - A rotation did not specify a type!");
	}
}

// ----------------------------------------------------------------------------
//	Pair Attributes Builder
// ----------------------------------------------------------------------------
PairAttributesBuilder::PairAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "PairAttributesBuilder") 
{

}

void PairAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("space1", "_NODATA_");
	attrib_->add("space2", "_NODATA_");
}

void PairAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void PairAttributesBuilder::getParameters()
{
	// No parameters
	return;
}

void PairAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string type = attrib_->getValAsStr("space1");
	if (!type.compare("_NODATA_")) {
		throw std::runtime_error(builderType_ + "::verify() - A pair did not specify space1!");
	}
	
	type = attrib_->getValAsStr("space2");
	if (!type.compare("_NODATA_")) {
		throw std::runtime_error(builderType_ + "::verify() - A pair did not specify space2!");
	}
}


// ----------------------------------------------------------------------------
//	Marker Attributes Builder
// ----------------------------------------------------------------------------
MarkerAttributesBuilder::MarkerAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "MarkerAttributesBuilder") 
{
}

void MarkerAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("type", "_NODATA_");
	attrib_->add("value", "0 0 0");
	attrib_->add("mutable", "0");
	attrib_->add("name", "_NODATA_");
}

void MarkerAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void MarkerAttributesBuilder::getParameters()
{
	// No parameters
	return;
}

void MarkerAttributesBuilder::verify()
{
	// Check to make sure we have good values
	std::string isMutable = attrib_->getValAsStr("mutable");
	
	if (!isMutable.compare("true")) {
		std::string name = attrib_->getValAsStr("name");
		
		if (!name.compare("_NODATA_"))
			throw std::runtime_error(builderType_ + "::verify() - A marker is marked mutable but has no name!");
	}
	
	std::string type = attrib_->getValAsStr("type");
	if (!type.compare("_NODATA_")) {
		throw std::runtime_error(builderType_ + "::verify() - A marker did not specify a type!");
	}
}


// ----------------------------------------------------------------------------
//	Transform Attributes Builder
// ----------------------------------------------------------------------------
TransformAttributesBuilder::TransformAttributesBuilder(const DOMNode* node) : 
	AttributesBuilder(node, "TransformAttributesBuilder") 
{

}

void TransformAttributesBuilder::createNewAttributes()
{
	AttributesBuilder::createNewAttributes();

	attrib_->add("type", "simple");
}

void TransformAttributesBuilder::getAttributes()
{
	AttributesBuilder::getAttributes();
}

void TransformAttributesBuilder::getParameters()
{
	// No parameters
	return;
}

void TransformAttributesBuilder::verify()
{
	std::string type = attrib_->getValAsStr("type");
	if (!type.compare("_NODATA_")) {
		throw std::runtime_error(builderType_ + "::verify() - A transform did not specify a type!");
	}
}


}; // END NAMESPACE
