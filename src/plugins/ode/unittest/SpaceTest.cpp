/*
 * SpaceTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include <ode/ode.h>
#include "SpaceTest.h"

using namespace log4cxx;

LoggerPtr SpaceTest::logger(Logger::getLogger("SpaceTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( SpaceTest );


void SpaceTest::setUp()
{
	space = NULL;
}

void SpaceTest::tearDown()
{
	if (space != NULL)
		fact.destroyInstance(space);
}

void SpaceTest::testCreateSimpleSpace()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollection pc;
	pc.addPair( TinySG::RequiredProperty("type", "simple") );

	space = dynamic_cast<sgode::Space*>( fact.createInstance("simple", NULL, &pc) );

	CPPUNIT_ASSERT( space->getOdeID() != NULL );
	CPPUNIT_ASSERT( space->getGeomClass() == dSimpleSpaceClass );
}

void SpaceTest::testCreateSpaceWithNULLParams()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	space = dynamic_cast<sgode::Space*>( fact.createInstance("simple", NULL, NULL) );
}

void SpaceTest::testCreateUnknownSpaceType()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollection pc;
	pc.addPair( TinySG::RequiredProperty("type", "unknown") );

	space = dynamic_cast<sgode::Space*>( fact.createInstance("simple", NULL, &pc) );
}
