# Process subdirectories in correct order
add_subdirectory( plugins )

# Handle optional dependencies
if ( Log4cxx_FOUND )
	include_directories( ${Log4cxx_INCLUDE_DIRS} )
	link_libraries( ${Log4cxx_LIBRARIES} )
endif ( Log4cxx_FOUND )

if ( Ode_FOUND )
	link_directories( ${PROJECT_BINARY_DIR}/tinysg/plugins/ode )
	link_libraries( collisionquery )
endif ( Ode_FOUND )

include_directories( 
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/api
	/usr/local/obrsp/include
	plugins
	${Boost_INCLUDE_DIRS}
)

link_directories( 
	${Boost_LIBRARY_DIRS}
	${PROJECT_BINARY_DIR}/src/plugins
)

add_definitions(-DTIXML_USE_STL=1)
add_definitions("-DTINYSG_VERSION_MAJOR=${TINYSG_VERSION_MAJOR}")
add_definitions("-DTINYSG_VERSION_MINOR=${TINYSG_VERSION_MINOR}")
add_definitions("-DTINYSG_VERSION_PATCH=${TINYSG_VERSION_PATCH}")

# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")
file(GLOB API_HEADERS "api/*.h")

add_library( ${PROJECT_NAME} SHARED ${SRCS})

if ( APPLE )
	target_link_libraries( ${PROJECT_NAME} 
						   ${Boost_LIBRARY_DIRS}/libboost_thread.dylib 
						   /usr/local/obrsp/lib/libobrsp_linalg.dylib 
						   /usr/local/obrsp/lib/libobrsp_pluginframework.dylib 
						   rtdistancequery)
else ( APPLE )
	link_directories( /usr/local/obrsp/lib/ )
	target_link_libraries( ${PROJECT_NAME} 
						   boost_thread 
						   /usr/local/obrsp/lib/libobrsp_linalg.so 
						   obrsp_pluginframework
						   rtdistancequery)
endif ( APPLE )
					   
install( TARGETS ${PROJECT_NAME} DESTINATION lib )
install( FILES ${HEADERS} DESTINATION include/tinysg )
install( FILES ${API_HEADERS} DESTINATION include/tinysg/api )

message( STATUS "Will build shared library: ${PROJECT_NAME}" )

#add_subdirectory( python )
if ( BUILD_DEMOS )
	add_subdirectory( demo )
endif ( BUILD_DEMOS )