/*
 * CollisionTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include "CollisionTest.h"

#include <Space.h>
#include <Geom.h>

using namespace log4cxx;

LoggerPtr CollisionTest::logger(Logger::getLogger("CollisionTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( CollisionTest );


void CollisionTest::setUp()
{
	dInitODE();
	world_ = dWorldCreate();

	mgr_.registerFactory( (new sgode::GeomFactory()) );
	mgr_.registerFactory( (new sgode::SpaceFactory()) );
	mgr_.registerFactory( (new sgode::CollisionPairFactory()) );
	mgr_.addQuery( new sgode::CollisionQuery() );

	TinySG::PropertyCollectionPtr pc;

	// Create some spaces and a space pair
	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("type", "simple") );
	mgr_.createObject("s1", "ODE_SPACE", pc.get());
	mgr_.createObject("s2", "ODE_SPACE", pc.get());

	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("space1", "s1") );
	pc->addPair( TinySG::RequiredProperty("space2", "s2") );
	mgr_.createObject("pair", "ODE_COLLISION_PAIR", pc.get());
}

void CollisionTest::tearDown()
{
	dWorldDestroy(world_);
	dCloseODE();
}

void CollisionTest::testSimpleGeomsInCollision()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollectionPtr pc;

	// Create some spheres for collision testing
	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("type", "sphere") );
	pc->addPair( TinySG::RequiredProperty("radius", "1") );
	pc->addPair( TinySG::OptionalProperty("position", "0 0 0"));
	sgode::Geom* g1 = dynamic_cast<sgode::Geom*>( mgr_.createObject("g1", "ODE_GEOM", pc.get()) );

	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("type", "sphere") );
	pc->addPair( TinySG::RequiredProperty("radius", "1") );
	pc->addPair( TinySG::OptionalProperty("position", "1.5 0 0"));
	sgode::Geom* g2 = dynamic_cast<sgode::Geom*>( mgr_.createObject("g2", "ODE_GEOM", pc.get()) );

	// Add each sphere to a space
	sgode::Space* s1 = dynamic_cast<sgode::Space*>( mgr_.getObject("s1", "ODE_SPACE") );
	s1->addGeom(g1);

	sgode::Space* s2 = dynamic_cast<sgode::Space*>( mgr_.getObject("s2", "ODE_SPACE") );
	s2->addGeom(g2);

	// Register query with manager and perform query
	sgode::CollisionReport* result = dynamic_cast<sgode::CollisionReport*>(mgr_.performQuery("ODE_COLLISION_CHECK"));
	LOG4CXX_INFO(logger, "Num objects in collision: " << result->getContactData().size() );
	CPPUNIT_ASSERT( result->inCollision() == true );
	delete result;
}

void CollisionTest::testSimpleGeomsNotInCollision()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	TinySG::PropertyCollectionPtr pc;

	// Create some spheres for collision testing
	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("type", "sphere") );
	pc->addPair( TinySG::RequiredProperty("radius", "1") );
	pc->addPair( TinySG::OptionalProperty("position", "0 0 0"));
	sgode::Geom* g1 = dynamic_cast<sgode::Geom*>( mgr_.createObject("g1", "ODE_GEOM", pc.get()) );

	pc.reset( (new TinySG::PropertyCollection) );
	pc->addPair( TinySG::RequiredProperty("type", "sphere") );
	pc->addPair( TinySG::RequiredProperty("radius", "1") );
	pc->addPair( TinySG::OptionalProperty("position", "3 0 0"));
	sgode::Geom* g2 = dynamic_cast<sgode::Geom*>( mgr_.createObject("g2", "ODE_GEOM", pc.get()) );

	// Add each sphere to a space
	sgode::Space* s1 = dynamic_cast<sgode::Space*>( mgr_.getObject("s1", "ODE_SPACE") );
	s1->addGeom(g1);

	sgode::Space* s2 = dynamic_cast<sgode::Space*>( mgr_.getObject("s2", "ODE_SPACE") );
	s2->addGeom(g2);

	// Register query with manager and perform query
	sgode::CollisionReport* result = dynamic_cast<sgode::CollisionReport*>(mgr_.performQuery("ODE_COLLISION_CHECK"));
	LOG4CXX_INFO(logger, "Num objects in collision: " << result->getContactData().size() );
	CPPUNIT_ASSERT( result->inCollision() == false );
	delete result;
}
