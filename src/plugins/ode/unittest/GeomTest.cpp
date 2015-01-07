/*
 * GeomTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "GeomTest.h"

using namespace log4cxx;

LoggerPtr GeomTest::logger(Logger::getLogger("GeomTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( GeomTest );


void GeomTest::setUp()
{
	space = NULL;
	geom = NULL;
}

void GeomTest::tearDown()
{
	if (space != NULL)
		spacefact.destroyInstance(space);

	if (geom != NULL)
		geomfact.destroyInstance(geom);
}

void GeomTest::testCreateBox()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollection pc;
	pc.addPair( TinySG::RequiredProperty("type", "box") );
	pc.addPair( TinySG::RequiredProperty("length", "5") );
	pc.addPair( TinySG::RequiredProperty("width", "1.0+2.0") );
	pc.addPair( TinySG::RequiredProperty("height", "2.9") );

	geom = dynamic_cast<sgode::Geom*>( geomfact.createInstance("box", NULL, &pc) );

	CPPUNIT_ASSERT( geom->getOdeID() != NULL );
	CPPUNIT_ASSERT( geom->getGeomClass() == dBoxClass );
}

void GeomTest::testCreateSphere()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollection pc;
	pc.addPair( TinySG::RequiredProperty("type", "sphere") );
	pc.addPair( TinySG::RequiredProperty("radius", "5") );

	geom = dynamic_cast<sgode::Geom*>( geomfact.createInstance("sphere", NULL, &pc) );

	CPPUNIT_ASSERT( geom->getOdeID() != NULL );
	CPPUNIT_ASSERT( geom->getGeomClass() == dSphereClass );
}

void GeomTest::testCreateSpaceWithNULLParams()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	geom = dynamic_cast<sgode::Geom*>( geomfact.createInstance("simple", NULL, NULL) );
}

void GeomTest::testCreateUnknownSpaceType()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollection pc;
	pc.addPair( TinySG::RequiredProperty("type", "unknown") );

	geom = dynamic_cast<sgode::Geom*>( geomfact.createInstance("simple", NULL, &pc) );
}
