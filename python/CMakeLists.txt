find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
if ( PYTHONLIBS_FOUND )
	message( STATUS "PythonLibs found: ${PYTHON_INCLUDE_PATH}" )
endif ( PYTHONLIBS_FOUND )

include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories( /usr/local/obrsp/lib
				  ${PROJECT_BINARY_DIR}/src )
link_libraries( tinysg )

set(CMAKE_SWIG_FLAGS "-shadow")

set_source_files_properties(tinysg.i PROPERTIES CPLUSPLUS ON)
#set_source_files_properties(tinysg.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(tinysg python tinysg.i)
swig_link_libraries(tinysg ${PYTHON_LIBRARIES})