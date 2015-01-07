/*
 * NodeTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: yamokosk
 */

#include <cppunit/config/SourcePrefix.h>
#include <linalg/Vector3.h>
#include <linalg/Quaternion.h>
#include <api/ObjectModel.h>
#include <sstream>
#include "MocapPoseGeneratorTest.h"

using namespace log4cxx;

LoggerPtr MocapPoseGeneratorTest::logger(Logger::getLogger("MocapPoseGeneratorTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( MocapPoseGeneratorTest );

class MyNode : public Node
{
	static log4cxx::LoggerPtr logger;

public:
	MyNode(const std::string& name) : mRecentlyUpdated(false), mName(name) {};

	bool getUpdateFlag()
	{
		return mRecentlyUpdated;
	}

	void resetUpdateFlag()
	{
		mRecentlyUpdated = false;
	}

	void updatePose( const Real* translation=NULL, const Real* rotation=NULL )
	{
		std::stringstream ss;
		ss << mName << " instructed to update." << std::endl;
		if ( translation != NULL )
		{
			ss << "-- Pos: " << translation[0] << ", " << translation[1] << ", " << translation[2] << "." << std::endl;
		}

		if ( rotation != NULL )
		{
			ss << "-- Ori: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << ", " << rotation[3] << "." << std::endl;
		}
		LOG4CXX_INFO(logger, " " << ss.str());

		mRecentlyUpdated = true;
	}

private:
	bool mRecentlyUpdated;
	std::string mName;
};

LoggerPtr MyNode::logger(Logger::getLogger("MyNode"));

void MocapPoseGeneratorTest::setUp()
{

}

void MocapPoseGeneratorTest::tearDown()
{

}

void MocapPoseGeneratorTest::testGeneratorUpdate()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	MyNode n1("node1"), n2("node2");
	MocapPoseGenerator gen;

	gen.attachNode(&n1);
	gen.attachNode(&n2);

	CPPUNIT_ASSERT( !n1.getUpdateFlag() );
	CPPUNIT_ASSERT( !n2.getUpdateFlag() );

	gen.notifyMoved();

	CPPUNIT_ASSERT( n1.getUpdateFlag() );
	CPPUNIT_ASSERT( n2.getUpdateFlag() );

	n1.resetUpdateFlag();
	n2.resetUpdateFlag();

	CPPUNIT_ASSERT( !n1.getUpdateFlag() );
	CPPUNIT_ASSERT( !n2.getUpdateFlag() );

	gen.detachNode(&n2);
	gen.notifyMoved();

	CPPUNIT_ASSERT( n1.getUpdateFlag() );
	CPPUNIT_ASSERT( !n2.getUpdateFlag() );
}

void MocapPoseGeneratorTest::testAddMarkers()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	MocapPoseGenerator gen;

	/*
	 * Define local markers
	 */
	gen.addMarker( Marker(10, "RSK1", Vector3(0.0, 0.0, 0.0)) );
	gen.addMarker( Marker(11, "RSK2", Vector3(1.0, 0.0, 0.0)) );
	gen.addMarker( Marker(12, "RSK3", Vector3(0.0, 1.0, 0.0)) );
	gen.addMarker( Marker(13, "RSK4", Vector3(0.0, 0.0, 1.0)) );

	CPPUNIT_ASSERT( gen.numberOfMarkers() == 4  );

	CPPUNIT_ASSERT( gen.hasMarkerById(10) );
	CPPUNIT_ASSERT( gen.hasMarkerById(12) );

	CPPUNIT_ASSERT( !gen.hasMarkerById(1) );
	CPPUNIT_ASSERT( !gen.hasMarkerById(100) );
}
