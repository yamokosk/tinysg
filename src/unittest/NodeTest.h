/*
 * NodeTest.h
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
#include "SceneNode.h"

class NodeTest : public CppUnit::TestFixture
{
	static log4cxx::LoggerPtr logger;

	CPPUNIT_TEST_SUITE( NodeTest );
	CPPUNIT_TEST( testNumChildren );
	CPPUNIT_TEST( testGetChildByIndex );
	CPPUNIT_TEST( testGetChildByName );
	CPPUNIT_TEST( testRemoveChildByIndex );
	CPPUNIT_TEST( testRemoveChildByName );
	CPPUNIT_TEST( testRemoveAllChildren );
	CPPUNIT_TEST( testTranslateRelativeToLocal );
	CPPUNIT_TEST( testTranslateRelativeToParent );
	CPPUNIT_TEST( testTranslateRelativeToWorld );
	CPPUNIT_TEST( testRotateRelativeToLocal );
	CPPUNIT_TEST( testRotateRelativeToParent );
	CPPUNIT_TEST( testRotateRelativeToWorld );
	CPPUNIT_TEST( testProcessUpdates );
	CPPUNIT_TEST_EXCEPTION( testAddChildWithParent, TinySG::InvalidParametersException );
	CPPUNIT_TEST_EXCEPTION( testAddChildWithSameName, TinySG::ItemIdentityException );
	CPPUNIT_TEST_EXCEPTION( testGetChildByBadIndex, TinySG::InvalidParametersException );
	CPPUNIT_TEST_EXCEPTION( testGetChildByBadName, TinySG::ItemIdentityException );
	CPPUNIT_TEST_EXCEPTION( testRemoveChildByBadIndex, TinySG::InvalidParametersException );
	CPPUNIT_TEST_EXCEPTION( testRemoveChildByBadName, TinySG::ItemIdentityException );
	CPPUNIT_TEST_SUITE_END();

protected:
	TinySG::SceneNode *n1, *n2, *n3, *n4, *n2_copy;
	//TinySG::NodeFactory fact;

public:
	void setUp();
	void tearDown();

protected:
	// Level 1 test cases
	void testNumChildren();
	void testGetChildByIndex();
	void testGetChildByName();
	void testRemoveChildByIndex();
	void testRemoveChildByName();
	void testRemoveAllChildren();

	void testTranslateRelativeToLocal();
	void testTranslateRelativeToParent();
	void testTranslateRelativeToWorld();

	void testRotateRelativeToLocal();
	void testRotateRelativeToParent();
	void testRotateRelativeToWorld();

	// Level 2 test cases
	void testProcessUpdates();
	//void testChangeParentPose();

	// Exception test cases
	void testAddChildWithParent();
	void testAddChildWithSameName();
	void testGetChildByBadIndex();
	void testGetChildByBadName();
	void testRemoveChildByBadIndex();
	void testRemoveChildByBadName();
};

#endif /* NODETEST_H_ */
