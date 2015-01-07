set( TARGET_NAME "lcalgorithm" )

#add_subdirectory( unittest )

include_directories( ${PROJECT_SOURCE_DIR}
					 ${Boost_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIRS} )
				  
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library( ${TARGET_NAME} STATIC ${SRCS})