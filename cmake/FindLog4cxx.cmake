# - Try to find Log4cxx
# Once done, this will define
#
#  Log4cxx_FOUND - system has Log4cxx
#  Log4cxx_INCLUDE_DIRS - the Log4cxx include directories
#  Log4cxx_LIBRARIES - link these to use Log4cxx

if (Log4cxx_INCLUDE_DIRS AND Log4cxx_LIBRARIES)
	# in cache already
	set(Log4cxx_FOUND TRUE)
else (Log4cxx_INCLUDE_DIRS AND Log4cxx_LIBRARIES)
	include(LibFindMacros)

	# Use pkg-config to get hints about paths
	libfind_pkg_check_modules(Log4cxx_PKGCONF log4cxx)
	
	# Include dir
	find_path(Log4cxx_INCLUDE_DIR
		NAMES log4cxx/logger.h
		PATHS 
			${Log4cxx_PKGCONF_INCLUDE_DIRS}
			/usr/include/
			/usr/local/include/
	)

	# Finally the library itself
	find_library(Log4cxx_LIBRARY
		NAMES log4cxx
		PATHS ${Log4cxx_PKGCONF_LIBRARY_DIRS}
				/usr/lib
				/usr/local/lib
	)

	# Set the include dir variables and the libraries and let libfind_process do the rest.
	# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
	set(Log4cxx_PROCESS_INCLUDES Log4cxx_INCLUDE_DIR)
	set(Log4cxx_PROCESS_LIBS Log4cxx_LIBRARY)
	libfind_process(Log4cxx)
	
	if (Log4cxx_INCLUDE_DIRS AND Log4cxx_LIBRARIES)
		set(Log4cxx_FOUND TRUE)
	endif (Log4cxx_INCLUDE_DIRS AND Log4cxx_LIBRARIES)
endif (Log4cxx_INCLUDE_DIRS AND Log4cxx_LIBRARIES)
