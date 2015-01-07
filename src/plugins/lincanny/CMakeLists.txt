add_subdirectory( algorithm )

include_directories( ${PROJECT_SOURCE_DIR}/src 
					 /usr/local/obrsp/include
					 ${Boost_INCLUDE_DIRS})
link_directories( algorithm 
				  /usr/local/obrsp/lib )
				  
# Gather all *.cpp and *.h files into their respective variables
file(GLOB TSG_LC_SRCS "*.cpp")
file(GLOB TSG_LC_HEADERS "*.h")

set(TARGET rtdistancequery)
add_library( ${TARGET} STATIC ${TSG_LC_SRCS})
target_link_libraries(${TARGET} lcalgorithm obrsp_linalg)

message( STATUS "Will build static plugin: ${TARGET}" )