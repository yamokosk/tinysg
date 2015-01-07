if ( ${PERFORM_UNIT_TESTS} AND ${Cppunit_FOUND} )
	message( STATUS "going to build tests")
	add_subdirectory( tests )
endif ( ${PERFORM_UNIT_TESTS} AND ${Cppunit_FOUND} )
