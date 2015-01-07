/*
 * CollisionTest.h
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
#include <ObjectManager.h>
#include <CollisionQuery.h>

class CollisionTest : public CppUnit::TestFixture
{
	static log4cxx::LoggerPtr logger;

	CPPUNIT_TEST_SUITE( CollisionTest );
	CPPUNIT_TEST( testSimpleGeomsInCollision );
	CPPUNIT_TEST( testSimpleGeomsNotInCollision );
	CPPUNIT_TEST_SUITE_END();

protected:
	TinySG::ObjectManager mgr_;
	dWorldID world_;
public:
	void setUp();
	void tearDown();

protected:
	// Level 1 test cases
	void testSimpleGeomsInCollision();
	void testSimpleGeomsNotInCollision();
};

#endif /* NODETEST_H_ */
