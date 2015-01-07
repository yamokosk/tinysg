# - Try to find Ode
# Once done, this will define
#
#  Ode_FOUND - system has Ode
#  Ode_INCLUDE_DIRS - the Ode include directories
#  Ode_LIBRARIES - link these to use Ode

if (Ode_INCLUDE_DIRS AND Ode_LIBRARIES)
	# in cache already
	set(Ode_FOUND TRUE)
else (Ode_INCLUDE_DIRS AND Ode_LIBRARIES)
	include(LibFindMacros)

	# Use pkg-config to get hints about paths
	libfind_pkg_check_modules(Ode_PKGCONF ode)
	
	# Include dir
	find_path(Ode_INCLUDE_DIR
		NAMES ode/ode.h
		PATHS 
			${Ode_PKGCONF_INCLUDE_DIRS}
			/usr/include/
			/usr/local/include/
	)

	# Finally the library itself
	find_library(Ode_LIBRARY
		NAMES ode
		PATHS ${Ode_PKGCONF_LIBRARY_DIRS}
				/usr/lib
				/usr/local/lib
	)

	# Set the include dir variables and the libraries and let libfind_process do the rest.
	# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
	set(Ode_PROCESS_INCLUDES Ode_INCLUDE_DIR)
	set(Ode_PROCESS_LIBS Ode_LIBRARY)
	libfind_process(Ode)
	
	if (Ode_INCLUDE_DIRS AND Ode_LIBRARIES)
		set(Ode_FOUND TRUE)
	endif (Ode_INCLUDE_DIRS AND Ode_LIBRARIES)
endif (Ode_INCLUDE_DIRS AND Ode_LIBRARIES)
