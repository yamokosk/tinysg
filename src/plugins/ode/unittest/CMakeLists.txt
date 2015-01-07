enable_testing()
include_directories( ${PROJECT_SOURCE_DIR}/src
					 ${PROJECT_SOURCE_DIR}/plugins/ODE
					 ${CPPUNIT_INCLUDE_DIRS}
					 ${Boost_INCLUDE_DIRS}
					 ${NEWMAT_INCLUDE_DIRS}
					 ${MUPARSER_HOME}/include
					 ${LOG4CXX_INCLUDE_DIRS}
					 ${ODE_HOME}/include)
link_directories( ${CPPUNIT_LIBRARY_DIRS}
				  ${LOG4CXX_LIBRARY_DIRS}
                  ${Boost_LIBRARY_DIRS}
				  ${NEWMAT_LIBRARY_DIRS}
				  ${MUPARSER_HOME}/lib
				  ${ODE_HOME}/lib )
if ( WIN32 )
    link_libraries( libcppunit.dll.a )
else ( WIN32 )
    link_libraries( ${CPPUNIT_LIBRARIES} ${LOG4CXX_LIBRARIES})
endif ( WIN32 )

file(GLOB UnitTests_SRCS "*Test.cpp" )
foreach(test ${UnitTests_SRCS})
    get_filename_component(TestName ${test} NAME_WE)
    include( ${TestName}.inc )
    set( srcs main.cpp ${test} ${test_srcs} )
    add_executable(${TestName} ${srcs})
    if ( WIN32 )
        target_link_libraries(${TestName} debug muparser_static_D optimized muparser_static
                                      debug newmatD optimized newmat)
    else ( WIN32 )
        target_link_libraries(${TestName} muparser newmat ode)
    endif ( WIN32 )
    add_test(${TestName} ${TestName}${CMAKE_EXECUTABLE_SUFFIX} )
endforeach(test)

set( UNITTESTS_PASSED TRUE )