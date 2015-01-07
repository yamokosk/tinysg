include_directories( 
	${PROJECT_SOURCE_DIR}/src
	/usr/local/obrsp/include
	${Boost_INCLUDE_DIRS}
)

link_directories( ${Boost_LIBRARY_DIRS}
				  /usr/local/obrsp/lib )
	  
# Gather all *.cpp and *.h files into their respective variables
file(GLOB SRCS "demo_*.cpp")

message( STATUS " == Demos ==" )

foreach( filename ${SRCS} )
	get_filename_component(DemoName ${filename} NAME_WE)
#	if ( Log4cxx_FOUND )
#		include_directories( ${Log4cxx_INCLUDE_DIRS} )
#		link_libraries( ${Log4cxx_LIBRARIES} )
#	endif ( Log4cxx_FOUND )
	add_executable( ${DemoName} demowrapper.cpp ${filename} )
	target_link_libraries( ${DemoName} tinysg )
	message( STATUS "Will build demo: ${DemoName}" )
endforeach( filename )