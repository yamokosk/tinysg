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

#ifndef _SML_ERROR_H_
#define _SML_ERROR_H_

// External includes
#include <string>
#include <sstream>
#include <exception>

namespace TinySG {

	class Exception : public std::exception
	{
	protected:
		long line;
		int number;
		std::string typeName;
		std::string description;
		std::string source;
		std::string file;
		std::string fullDesc;
	public:
		/** Static definitions of error codes.
			@todo
				Add many more exception codes, since we want the user to be able
				to catch most of them.
		*/
		enum ExceptionCodes {
			ERR_CANNOT_WRITE_TO_FILE,
			ERR_INVALID_STATE,
			ERR_INVALIDPARAMS,
			ERR_DUPLICATE_ITEM,
			ERR_ITEM_NOT_FOUND,
			ERR_FILE_NOT_FOUND,
			ERR_INTERNAL_ERROR,
			ERR_RT_ASSERTION_FAILED,
			ERR_NOT_IMPLEMENTED,
			ERR_DYNAMIC_LOAD_FAILED
		};

		/** Default constructor.
		*/
		Exception( int number, const std::string& description, const std::string& source );

		/** Advanced constructor.
		*/
		Exception( int number, const std::string& description, const std::string& source, const char* type, const char* file, long line );

		/** Copy constructor.
		*/
		Exception(const Exception& rhs);

		/// Needed for  compatibility with std::exception
		~Exception() throw() {}

		/** Assignment operator.
		*/
		void operator = (const Exception& rhs);

		/** Returns a string with the full description of this error.
			@remarks
				The description contains the error number, the description
				supplied by the thrower, what routine threw the exception,
				and will also supply extra platform-specific information
				where applicable. For example - in the case of a rendering
				library error, the description of the error will include both
				the place in which OGRE found the problem, and a text
				description from the 3D rendering library, if available.
		*/
		virtual const std::string getFullDescription(void) const;

		/** Gets the error code.
		*/
		virtual int getNumber(void) const throw();

		/** Gets the source function.
		*/
		virtual const std::string &getSource() const { return source; }

		/** Gets source file name.
		*/
		virtual const std::string &getFile() const { return file; }

		/** Gets line number.
		*/
		virtual long getLine() const { return line; }

		/** Returns a string with only the 'description' field of this exception. Use
			getFullDescriptionto get a full description of the error including line number,
			error number and what function threw the exception.
		*/
		virtual const std::string &getDescription(void) const { return description; }

		/// Override std::exception::what
		const char* what() const throw() { return getFullDescription().c_str(); }

	};


	/** Template struct which creates a distinct type for each exception code.
	@note
	This is useful because it allows us to create an overloaded method
	for returning different exception types by value without ambiguity.
	From 'Modern C++ Design' (Alexandrescu 2001).
	*/
	template <int num>
	struct ExceptionCodeType
	{
		enum { number = num };
	};

	// Specialised exceptions allowing each to be caught specifically
	// backwards-compatible since exception codes still used

	class UnimplementedException : public Exception
	{
	public:
		UnimplementedException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "UnimplementedException", file, line) {}
	};

	class FileNotFoundException : public Exception
	{
	public:
		FileNotFoundException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "FileNotFoundException", file, line) {}
	};

	class IOException : public Exception
	{
	public:
		IOException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "IOException", file, line) {}
	};

	class InvalidStateException : public Exception
	{
	public:
		InvalidStateException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "InvalidStateException", file, line) {}
	};

	class InvalidParametersException : public Exception
	{
	public:
		InvalidParametersException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "InvalidParametersException", file, line) {}
	};

	class ItemIdentityException : public Exception
	{
	public:
		ItemIdentityException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "ItemIdentityException", file, line) {}
	};

	class InternalErrorException : public Exception
	{
	public:
		InternalErrorException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "InternalErrorException", file, line) {}
	};

	class RuntimeAssertionException : public Exception
	{
	public:
		RuntimeAssertionException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "RuntimeAssertionException", file, line) {}
	};

	class DynamicLoadException : public Exception
	{
	public:
		DynamicLoadException(int number, const std::string& description, const std::string& source, const char* file, long line)
		: Exception(number, description, source, "DynamicLoadException", file, line) {}
	};

	/** Class implementing dispatch methods in order to construct by-value
		exceptions of a derived type based just on an exception code.
	@remarks
		This nicely handles construction of derived Exceptions by value (needed
		for throwing) without suffering from ambiguity - each code is turned into
		a distinct type so that methods can be overloaded. This allows OGRE_EXCEPT
		to stay small in implementation (desirable since it is embedded) whilst
		still performing rich code-to-type mapping.
	*/
	class ExceptionFactory
	{
	private:
		/// Private constructor, no construction
		ExceptionFactory() {}
	public:
		static UnimplementedException create(
			ExceptionCodeType<Exception::ERR_NOT_IMPLEMENTED> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return UnimplementedException(code.number, desc, src, file, line);
		}
		static FileNotFoundException create(
			ExceptionCodeType<Exception::ERR_FILE_NOT_FOUND> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return FileNotFoundException(code.number, desc, src, file, line);
		}
		static IOException create(
			ExceptionCodeType<Exception::ERR_CANNOT_WRITE_TO_FILE> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return IOException(code.number, desc, src, file, line);
		}
		static InvalidStateException create(
			ExceptionCodeType<Exception::ERR_INVALID_STATE> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return InvalidStateException(code.number, desc, src, file, line);
		}
		static InvalidParametersException create(
			ExceptionCodeType<Exception::ERR_INVALIDPARAMS> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return InvalidParametersException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_ITEM_NOT_FOUND> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_DUPLICATE_ITEM> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static InternalErrorException create(
			ExceptionCodeType<Exception::ERR_INTERNAL_ERROR> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return InternalErrorException(code.number, desc, src, file, line);
		}
		static RuntimeAssertionException create(
			ExceptionCodeType<Exception::ERR_RT_ASSERTION_FAILED> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return RuntimeAssertionException(code.number, desc, src, file, line);
		}
		static DynamicLoadException create(
			ExceptionCodeType<Exception::ERR_DYNAMIC_LOAD_FAILED> code,
			const std::string& desc,
			const std::string& src, const char* file, long line)
		{
			return DynamicLoadException(code.number, desc, src, file, line);
		}
	};

#ifndef SML_EXCEPT
#define SML_EXCEPT(num, desc) throw TinySG::ExceptionFactory::create( \
		TinySG::ExceptionCodeType<num>(), desc, __FUNCTION__, __FILE__, __LINE__ );
#endif

} // namespace TinySG
#endif //_ERROR_H_
