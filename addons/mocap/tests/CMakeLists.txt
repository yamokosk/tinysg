enable_testing()
include_directories( ${PROJECT_SOURCE_DIR}
					 ${Cppunit_INCLUDE_DIRS}
					 ${Boost_INCLUDE_DIRS}
					 ${Log4cxx_INCLUDE_DIRS})

if ( WIN32 )
    link_libraries( libcppunit.dll.a )
else ( WIN32 )
    link_libraries( ${Cppunit_LIBRARIES} ${Log4cxx_LIBRARIES})
endif ( WIN32 )

file(GLOB UnitTests_SRCS "*Test.cpp" )
foreach(test ${UnitTests_SRCS})
    get_filename_component(TestName ${test} NAME_WE)
    message(STATUS "Adding test ${TestName}")
    include( ${TestName}.inc )
    if ( build_test )
    	set( srcs main.cpp ${test} ${test_srcs} )
		add_executable(${TestName} ${srcs})
		add_test(${TestName} ${TestName}${CMAKE_EXECUTABLE_SUFFIX} )
	endif ( build_test )
endforeach(test)