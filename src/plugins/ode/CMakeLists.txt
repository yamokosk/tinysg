include_directories( ${PROJECT_SOURCE_DIR}/src
					 ${Boost_INCLUDE_DIRS}
					 ${Ode_INCLUDE_DIRS}
					 /usr/local/obrsp/include )
link_directories( /usr/local/obrsp/lib )

if ( ${PERFORM_UNIT_TESTS} )
	add_subdirectory( unittest )
endif ( ${PERFORM_UNIT_TESTS} )

# Gather all *.cpp and *.h files into their respective variables
file(GLOB TSG_ODE_SRCS "*.cpp")

set(TARGET collisionquery)
add_library( ${TARGET} STATIC ${TSG_ODE_SRCS})
target_link_libraries(${TARGET} obrsp_mesh obrsp_linalg ${Ode_LIBRARIES})

message( STATUS "Will build static plugin: ${TARGET}" )