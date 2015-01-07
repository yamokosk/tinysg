find_package( Cppunit )
if ( NOT ${Cppunit_FOUND} )
	if ( ${PERFORM_UNIT_TESTS} )
		message( WARNING "	Turning off unit tests because you don't seem to have CppUnit installed.")
		set( PERFORM_UNIT_TESTS OFF )
	endif ( ${PERFORM_UNIT_TESTS} )
else ( NOT ${Cppunit_FOUND} )
	message( STATUS "	CppUnit include: ${Cppunit_INCLUDE_DIRS}" )
endif ( NOT ${Cppunit_FOUND} )

enable_testing()
include_directories( ${PROJECT_SOURCE_DIR}
					 ${Cppunit_INCLUDE_DIRS}
					 ${Boost_INCLUDE_DIRS}
					 ${MuParser_INCLUDE_DIRS}
					 ${Log4cxx_INCLUDE_DIRS})

if ( WIN32 )
    link_libraries( libcppunit.dll.a )
else ( WIN32 )
    link_libraries( ${Cppunit_LIBRARIES} ${Log4cxx_LIBRARIES})
endif ( WIN32 )


add_definitions("-DTINYSG_VERSION_MAJOR=${TINYSG_VERSION_MAJOR}")
add_definitions("-DTINYSG_VERSION_MINOR=${TINYSG_VERSION_MINOR}")
add_definitions("-DTINYSG_VERSION_PATCH=${TINYSG_VERSION_PATCH}")

file(GLOB UnitTests_SRCS "*Test.cpp" )
foreach(test ${UnitTests_SRCS})
    get_filename_component(TestName ${test} NAME_WE)
    #message("Adding test ${TestName}")
    include( ${TestName}.inc )
    if (build_test)
		set( srcs main.cpp ${test} ${test_srcs} )
		#message(STATUS "srcs = ${srcs}")
		add_executable(${TestName} ${srcs})
		if ( WIN32 )
		    target_link_libraries(${TestName} debug muparser_static_D optimized muparser_static)
		else ( WIN32 )
		    target_link_libraries(${TestName} muparser)
		endif ( WIN32 )
		add_test(${TestName} ${TestName}${CMAKE_EXECUTABLE_SUFFIX} )
    endif (build_test)
endforeach(test)