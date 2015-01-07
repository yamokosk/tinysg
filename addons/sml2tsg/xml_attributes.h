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

#ifndef _SCENEML_ATTRIBUTES_H_FILE_
#define _SCENEML_ATTRIBUTES_H_FILE_

//#include "config.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI // Make sure M_PI is defined
#define My_PI 3.14159265358979323846
#else
#define My_PI M_PI
#endif

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// Required math expression parser
#include <muParser.h>

// Boost shared ptr
#include <boost/shared_ptr.hpp>

// ODE
#include <ode/ode.h>

namespace sceneml {

typedef boost::shared_ptr<dReal> dRealPtr;

XERCES_CPP_NAMESPACE_USE

//class SCENEML_API Attributes
class Attributes
{
public:
	typedef std::map<std::string, std::string> properties_t;
	
	Attributes();
	virtual ~Attributes();
	
	void add(const std::string& name, const std::string &val);
	void update(const std::string& name, const std::string &val);
	
	float getValAsReal(const std::string& name)
	{
		float val = 0.0;
		dRealPtr pval( this->get(name, 1) );
		val = *pval;
		return val;
	}
	
	int getValAsInt(const std::string& name)
	{
		return (int)getValAsReal(name);
	}
	
	dRealPtr getValAsVec(const std::string& name, int length)
	{
		dRealPtr val( this->get(name, length) );
		return val;
	}
	
	std::string getValAsStr(const std::string& name)
	{
		return this->get(name);
	}

private:
	std::string get(const std::string& name);
	float* get(const std::string& name, int length);
	
	//class MathParserImpl; // Forward declaration
	//std::auto_ptr<MathParserImpl> pimpl_;
	mu::Parser parser_;
	void parseValue(const char* str, float* val, int length);
	properties_t properties_;
};

typedef std::auto_ptr<Attributes> AttributesPtr;

// Abstract Builder
//class SCENEML_API AttributesBuilder
class AttributesBuilder
{
protected:
	AttributesPtr attrib_;
	std::string builderType_;
	
public:	
	AttributesBuilder(const DOMNode* node, const std::string& type) : node_(node), builderType_(type), attrib_(new Attributes) {};
	virtual ~AttributesBuilder() {}
	AttributesPtr GetAttributes() { return attrib_; }
 
	// General build methods
	virtual void createNewAttributes() {attrib_.reset(new Attributes);}
	virtual void getAttributes();
	virtual void getParameters()=0;
	virtual void verify()=0;
	
	const DOMNode* node_;
};
 
// Director
//class SCENEML_API AttributesDirector
class AttributesDirector
{
	AttributesBuilder* attribBuilder;
public:
	AttributesDirector() : attribBuilder(NULL) {}
	~AttributesDirector() {}
 
	void SetAttributesBuilder(AttributesBuilder* b) { attribBuilder = b; }
	AttributesPtr GetAttributes() {return attribBuilder->GetAttributes();}
	void ConstructAttributes()
	{
		attribBuilder->createNewAttributes();
		attribBuilder->getAttributes();
		attribBuilder->getParameters();
		attribBuilder->verify();
	}
};

// Space attributes
//class SCENEML_API SpaceAttributesBuilder : public AttributesBuilder
class SpaceAttributesBuilder : public AttributesBuilder
{
public:
	SpaceAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Body attributes
//class SCENEML_API BodyAttributesBuilder : public AttributesBuilder
class BodyAttributesBuilder : public AttributesBuilder
{
public:
	BodyAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Geom attributes
class GeomAttributesBuilder : public AttributesBuilder
{
public:
	GeomAttributesBuilder(const DOMNode* node);

	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Translation attributes
class TranslationAttributesBuilder : public AttributesBuilder
{
public:
	TranslationAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class RotationAttributesBuilder : public AttributesBuilder
{
public:
	RotationAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Pair attributes
class PairAttributesBuilder : public AttributesBuilder
{
public:
	PairAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Rotation attributes
class MarkerAttributesBuilder : public AttributesBuilder
{
public:
	MarkerAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

// Transform attributes
class TransformAttributesBuilder : public AttributesBuilder
{
public:
	TransformAttributesBuilder(const DOMNode* node);
	
	void createNewAttributes();
	void getAttributes();	
	void getParameters();
	void verify();
};

}; // END NAMESPACE

#endif
