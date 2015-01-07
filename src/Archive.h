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
 * Archive.h
 *
 *  Created on: Dec 8, 2008
 *      Author: yamokosk
 */

#ifndef ARCHIVE_H_
#define ARCHIVE_H_

#include "config.h"

#include <string>

#include "tinyxml.h"

#include <api/ObjectModel.h>

namespace tinysg
{

class SceneGraph;
class SceneNode;

/*
 * Archive interface
 */
class Archive
{
public:
	static const std::string NodeTagName;
	static const std::string PropertyTagName;
	static const std::string PositionTagName;
	static const std::string OrientationTagName;
	static const std::string ObjectTagName;

	static const std::string UnnamedValue;
	static const std::string EmptyValue;
	static const std::string UnknownValue;

	Archive(const std::string& f);
	virtual ~Archive() {};

	virtual void init() = 0;
	virtual void serialize(SceneGraph& object, int& ver) = 0;

private:
	virtual void serialize(SceneGraph& graph, SceneNode& object, int& ver) = 0;
	virtual void serialize(SceneGraph& graph, ObjectInfo& object, int& ver) = 0;
	virtual void serialize(SceneGraph& graph, Property& prop, int& ver) = 0;

protected:
	std::string filename;
	TiXmlElement* currXmlElement;
	TiXmlElement* root;
	TiXmlDocument doc;
};

/*
 * Output archive
 */

//#define DEBUG_TRACE
#ifdef DEBUG_TRACE
#	include <iostream>
#endif

class OArchive : public Archive
{
#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

public:
	OArchive(const std::string& f) : Archive(f) {};
	virtual ~OArchive() {};

	virtual void init();
	virtual void serialize(SceneGraph& object, int& ver);

private:
	virtual void serialize(SceneGraph& graph, SceneNode& object, int& ver);
	virtual void serialize(SceneGraph& graph, ObjectInfo& object, int& ver);
	virtual void serialize(SceneGraph& graph, Property& prop, int& ver);

private:
	TiXmlElement* createXmlNode(TiXmlElement* parent, const std::string& name);
	void addParameter( const std::string& name, const std::string& value );
	void addAttribute( const std::string& name, const std::string& value );
	void addAttribute( TiXmlElement* element, const std::string& name, const std::string& value );
};

/*
 * Output archive
 */
class IArchive : public Archive
{
#if defined( TSG_HAVE_LOG4CXX )
	static log4cxx::LoggerPtr logger;
#endif

public:
	IArchive(const std::string& f) : Archive(f) {};
	virtual ~IArchive() {};

	virtual void init();
	virtual void serialize(SceneGraph& object, int& ver);

private:
	virtual void serialize(SceneGraph& graph, SceneNode& object, int& ver);
	virtual void serialize(SceneGraph& graph, ObjectInfo& object, int& ver);
	virtual void serialize(SceneGraph& graph, Property& prop, int& ver);
};

} // end namespace tinysg
#endif /* ARCHIVE_H_ */
