if ( NOT Ode_FOUND )
	message(ERROR ": ODE required for the ${PROJECT_NAME} addon.")
endif ( NOT Ode_FOUND )

include_directories( 
	${PROJECT_SOURCE_DIR}/src
	/usr/local/obrsp/include
	${Boost_INCLUDE_DIRS}
	/usr/local/xerces-c/include
	/usr/local/include
)

link_directories( 
	${Boost_LIBRARY_DIRS}
	${PROJECT_BINARY_DIR}/src
)

# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_executable( sml2tsg ${SRCS})
target_link_libraries( sml2tsg
					   tinysg 
					   /usr/local/xerces-c/lib/libxerces-c.28.dylib
					   newmat
					   muparser)
 
install( TARGETS sml2tsg DESTINATION bin )

message( STATUS "Will build shared library: sml2tsg" )

#if ( BUILD_DEMOS )
#	add_subdirectory( demo )
#endif ( BUILD_DEMOS )