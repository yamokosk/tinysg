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
 * Property.h
 *
 *  Created on: Dec 8, 2008
 *      Author: yamokosk
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <string>
#include <vector>
#include <map>
#include <cstring>

#include "Iterator.h"
#include <boost/any.hpp>

namespace tinysg {

struct Property;

typedef std::vector<Property> PropertyContainer;
typedef VectorIterator<PropertyContainer> PropertyVectorIterator;

struct Property
{
	explicit Property(const char* n) :
		name_(n)
	{

	}

	explicit Property(const std::string& n) :
		name_(n)
	{

	}

	explicit Property(const char* n, const boost::any& t) :
		name_(n), value_(t)
	{

	}

	explicit Property(const std::string& n, const boost::any& t) :
		name_(n), value_(t)
	{

	}

	/*Property(const Property& rhs) :
		name_( rhs.name_str() ),
		value_( rhs.const_value() )
	{

	}*/

	const char* name() const
	{
		return name_.c_str();
	}

	const std::string& name_str() const
	{
		return name_;
	}

	boost::any& value()
	{
		return value_;
	}

	std::string value_str()
	{
		try
		{
			std::string val = boost::any_cast<std::string>(value_);
			return val;
		}
		catch (boost::bad_any_cast &)
		{
			return std::string("ERROR! Value could not be converted to a string.");
		}
	}

	const boost::any& const_value() const
	{
		return value_;
	}

	template<class T>
	T getAndCastValue() const
	{
		return T( *( boost::unsafe_any_cast<T>(&value_) ) );
	}

	void add_parameter(const Property& prop)
	{
		properties.push_back(prop);
	}

	Property get_parameter(const std::string& name) const
	{
		for (int n=0; n < (int)properties.size(); ++n)
		{
			if ( properties[n].name_str() == name )
			{
				return properties[n];
			}
		}
		return Property(name);
	}

	bool has_parameters() const
	{
		return (properties.size() > 0);
	}

	void set_name(const std::string& name)
	{
		name_ = name;
	}

	void set_value(const boost::any& value)
	{
		value_ = value;
	}

private:
	std::string name_;
	boost::any value_;

	// Nested properties!
	PropertyContainer properties;
};

template< typename T >
bool contains( const boost::any& a )
{
	try
	{
		// we do the comparison with 'name' because across shared library boundries we get
		// two different type_info objects
		return( std::strcmp( typeid( T ).name(), a.type().name() ) == 0 );
	}
	catch( ... )
	{ }

	return false;
}

}  // namespace tinysg

#endif /* PROPERTY_H_ */
