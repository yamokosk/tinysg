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

#include "Exception.h"

namespace TinySG {

	Exception::Exception(int num, const std::string& desc, const std::string& src) :
		line( 0 ),
		number( num ),
		description( desc ),
		source( src )
	{
	}

	Exception::Exception(int num, const std::string& desc, const std::string& src, const char* typ, const char* fil, long lin) :
		line( lin ),
		number( num ),
		typeName(typ),
		description( desc ),
		source( src ),
		file( fil )
	{
	}

	Exception::Exception(const Exception& rhs) :
		line( rhs.line ), number( rhs.number ), description( rhs.description ), source( rhs.source ), file( rhs.file )
	{
	}

	void Exception::operator = ( const Exception& rhs )
	{
		description = rhs.description;
		number = rhs.number;
		source = rhs.source;
		file = rhs.file;
		line = rhs.line;
		typeName = rhs.typeName;
	}

	const std::string Exception::getFullDescription(void) const
	{
		std::stringstream desc;
		if (fullDesc.empty())
		{
			std::stringstream desc;

			desc <<  "TinySG EXCEPTION(" << number << ":" << typeName << "): "
				 << description << " in " << source;

			if( line > 0 )
			{
				desc << " at " << file << " (line " << line << ")";
			}

			return desc.str();
		}

		return fullDesc;
	}

	int Exception::getNumber(void) const throw()
	{
		return number;
	}

}

