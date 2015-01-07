# - Try to find MuParser
# Once done, this will define
#
#  MuParser_FOUND - system has MuParser
#  MuParser_INCLUDE_DIRS - the MuParser include directories
#  MuParser_LIBRARIES - link these to use MuParser

if (MuParser_INCLUDE_DIRS AND MuParser_LIBRARIES)
	# in cache already
	set(MuParser_FOUND TRUE)
else (MuParser_INCLUDE_DIRS AND MuParser_LIBRARIES)
	include(LibFindMacros)

	# Use pkg-config to get hints about paths
	libfind_pkg_check_modules(MuParser_PKGCONF muparser)
	
	# Include dir
	find_path(MuParser_INCLUDE_DIR
		NAMES muParser/muParser.h
		PATHS 
			${MuParser_PKGCONF_INCLUDE_DIRS}
			/usr/include/
			/usr/local/include/
	)

	# Finally the library itself
	find_library(MuParser_LIBRARY
		NAMES muparser
		PATHS ${MuParser_PKGCONF_LIBRARY_DIRS}
				/usr/lib
				/usr/local/lib
	)

	# Set the include dir variables and the libraries and let libfind_process do the rest.
	# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
	set(MuParser_PROCESS_INCLUDES MuParser_INCLUDE_DIR)
	set(MuParser_PROCESS_LIBS MuParser_LIBRARY)
	libfind_process(MuParser)
	
	if (MuParser_INCLUDE_DIRS AND MuParser_LIBRARIES)
		set(MuParser_FOUND TRUE)
	endif (MuParser_INCLUDE_DIRS AND MuParser_LIBRARIES)
endif (MuParser_INCLUDE_DIRS AND MuParser_LIBRARIES)
