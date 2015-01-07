/*
 * MocapGeneratorTest.h
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#ifndef NODETEST_H_
#define NODETEST_H_

// Logging
#include <log4cxx/logger.h>
// CppUnit
#include <cppunit/extensions/HelperMacros.h>
// Class to test
#include <addons/mocap/EvartGeneratorManager.h>

class EvartGeneratorManagerTest : public CppUnit::TestFixture
{
	static log4cxx::LoggerPtr logger;

	CPPUNIT_TEST_SUITE( EvartGeneratorManagerTest );
	CPPUNIT_TEST( testEVaRTInitialization );
	//CPPUNIT_TEST_EXCEPTION( testAddChildWithParent, TinySG::InvalidParametersException );
	CPPUNIT_TEST_SUITE_END();

protected:

public:
	void setUp();
	void tearDown();

protected:
	// Level 1 test cases
	void testEVaRTInitialization();

	// Exception test cases
	//void testAddChildWithParent();
};

#endif /* NODETEST_H_ */
