cmake_minimum_required( VERSION 2.4 FATAL_ERROR )
cmake_policy(SET CMP0003 OLD) 

# ---------------------------------------------------------------------------------------------------------------------------
# Project information - name, version, and the project's cmake directory
# ---------------------------------------------------------------------------------------------------------------------------
PROJECT(tinysg)
SET( PROJECT_VERSION 1.0.0 )

STRING( REGEX MATCHALL "[0-9]+" PROJECT_VERSIONS ${PROJECT_VERSION} )
LIST( GET PROJECT_VERSIONS 0 TINYSG_VERSION_MAJOR)
LIST( GET PROJECT_VERSIONS 1 TINYSG_VERSION_MINOR)
LIST( GET PROJECT_VERSIONS 2 TINYSG_VERSION_PATCH)
MESSAGE( "TinSG version ${PROJECT_VERSION} (${TINYSG_VERSION_MAJOR}.${TINYSG_VERSION_MINOR}.${TINYSG_VERSION_PATCH})" )

# ---------------------------------------------------------------------------------------------------------------------------
# Project options
# ---------------------------------------------------------------------------------------------------------------------------
option( BUILD_DEMOS "Build the demo programs?" ON )
option( BUILD_ADDONS "Build the TinySG addons?" ON )
option( BUILD_STATIC_LIB "Build static library?" ON )
option( PERFORM_UNIT_TESTS "Perform unit tests?" ON )

# Easier to read error messages in Eclipse CDT
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0 -Wall")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0 -Wall")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# Throwing another bone at eclipse - helps "discover" include paths
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# ------------------------------------------------------------------------------
# Set build type and other build options
# ------------------------------------------------------------------------------
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE "Debug")

# ------------------------------------------------------------------------------
# Require packages
# ------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

message( STATUS " == Required Libraries == " )
find_package( Boost 1.35.0 )
message( STATUS "	Boost include: ${Boost_INCLUDE_DIRS}" )

message( STATUS " == Optional Libraries == " )
find_package( Log4cxx )
if ( Log4cxx_FOUND )
	add_definitions(-DTSG_HAVE_LOG4CXX)
	message( STATUS "	Log4CXX include: ${Log4cxx_INCLUDE_DIRS}" )
endif ( Log4cxx_FOUND )

find_package( MuParser )
if ( MuParser_FOUND )
	add_definitions(-DTSG_HAVE_MUPARSER)
	message( STATUS "	MuParser include: ${MuParser_INCLUDE_DIRS}" )
endif ( MuParser_FOUND )

find_package( Ode )
if ( Ode_FOUND )
	add_definitions(-DTSG_HAVE_ODE)
	message( STATUS "	ODE include: ${Ode_INCLUDE_DIRS}" )
endif ( Ode_FOUND )

# ------------------------------------------------------------------------------
# Process sub-directories
# ------------------------------------------------------------------------------
add_subdirectory( src )

if ( BUILD_ADDONS )
	add_subdirectory( addons )
endif ( BUILD_ADDONS )