# - Try to find Cppunit
# Once done, this will define
#
#  Cppunit_FOUND - system has Cppunit
#  Cppunit_INCLUDE_DIRS - the Cppunit include directories
#  Cppunit_LIBRARIES - link these to use Cppunit

if (Cppunit_INCLUDE_DIRS AND Cppunit_LIBRARIES)
	# in cache already
	set(Cppunit_FOUND TRUE)
else (Cppunit_INCLUDE_DIRS AND Cppunit_LIBRARIES)
	include(LibFindMacros)

	# Use pkg-config to get hints about paths
	libfind_pkg_check_modules(Cppunit_PKGCONF cppunit)
	
	# Include dir
	find_path(Cppunit_INCLUDE_DIR
		NAMES cppunit/TestSuite.h
		PATHS 
			${Cppunit_PKGCONF_INCLUDE_DIRS}
			/usr/include/
			/usr/local/include/
	)

	# Finally the library itself
	find_library(Cppunit_LIBRARY
		NAMES cppunit
		PATHS ${Cppunit_PKGCONF_LIBRARY_DIRS}
				/usr/lib
				/usr/local/lib
	)

	# Set the include dir variables and the libraries and let libfind_process do the rest.
	# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
	set(Cppunit_PROCESS_INCLUDES Cppunit_INCLUDE_DIR)
	set(Cppunit_PROCESS_LIBS Cppunit_LIBRARY)
	libfind_process(Cppunit)
	
	if (Cppunit_INCLUDE_DIRS AND Cppunit_LIBRARIES)
		set(Cppunit_FOUND TRUE)
	endif (Cppunit_INCLUDE_DIRS AND Cppunit_LIBRARIES)
endif (Cppunit_INCLUDE_DIRS AND Cppunit_LIBRARIES)