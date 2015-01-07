include( WriteMatlabCompile.cmake )

include_directories( ${PROJECT_SOURCE_DIR}/src
					 /usr/local/obrsp/include
					 ${Boost_INCLUDE_DIRS}
					 ${MATLAB_INCLUDE_DIR} )

link_directories( 
	${PROJECT_BINARY_DIR}/src
	${Boost_LIBRARY_DIRS}
)

#message( STATUS "MATLAB_LIBRARY_DIR = ${MATLAB_LIBRARY_DIR}")

# Gather together the sources
file(GLOB SRCS "*.cpp")

# Try compiling outside Matlab
set(target mex_tinysg)
add_definitions(-DMATLAB_MEX_FILE)
add_definitions(-DTIXML_USE_STL=1)
#add_definitions(-DDEBUG_TINYSG)
add_library(${target} SHARED ${SRCS})
target_link_libraries(${target} ${MATLAB_MEX_LIBRARY} ${MATLAB_MX_LIBRARY} tinysg)

if (WIN32)
	set_target_properties(${target} PROPERTIES SUFFIX ".mexw32")
	message(STATUS "Setting mex prefix to .mexw32")
endif (WIN32)

if (UNIX AND NOT APPLE)
	set_target_properties(${target} PROPERTIES LINK_FLAGS "--version-script")
	set_target_properties(${target} PROPERTIES SUFFIX ".mexglx")
	message(STATUS "Setting mex prefix to .mexglx")
endif (UNIX AND NOT APPLE)

if (UNIX AND APPLE)
	set_target_properties(${target} PROPERTIES SUFFIX ".mexmaci")
	message(STATUS "Setting mex prefix to .mexmaci")
endif (UNIX AND APPLE)
	
install( TARGETS ${target} DESTINATION tinysg/addons/matlab )
