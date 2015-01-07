/*
 * GeomTest.h
 *
 *  Created on: Oct 22, 2008
 *      Author: yamokosk
 */

#ifndef SPACETEST_H_
#define SPACETEST_H_

// Logging
#include <log4cxx/logger.h>
// CppUnit
#include <cppunit/extensions/HelperMacros.h>
// Class to test
#include <Space.h>
#include <Geom.h>

class GeomTest : public CppUnit::TestFixture
{
	static log4cxx::LoggerPtr logger;

	CPPUNIT_TEST_SUITE( GeomTest );
	CPPUNIT_TEST( testCreateBox );
	CPPUNIT_TEST( testCreateSphere );
	CPPUNIT_TEST_EXCEPTION( testCreateSpaceWithNULLParams, TinySG::InvalidParametersException );
	CPPUNIT_TEST_EXCEPTION( testCreateUnknownSpaceType, TinySG::InvalidParametersException );
	CPPUNIT_TEST_SUITE_END();

protected:
	sgode::Space *space;
	sgode::SpaceFactory spacefact;

	sgode::Geom *geom;
	sgode::GeomFactory geomfact;

public:
	void setUp();
	void tearDown();

protected:
	// Level 1 test cases
	void testCreateBox();
	void testCreateSphere();

	// Exception test cases
	void testCreateSpaceWithNULLParams();
	void testCreateUnknownSpaceType();
};

#endif /* NODETEST_H_ */
