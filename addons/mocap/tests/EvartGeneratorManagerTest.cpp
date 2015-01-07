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
#include <ios>
#include <iostream>
#include <limits>

#include<stdio.h>
#include<time.h>
#include<signal.h>

#include "EvartGeneratorManagerTest.h"

using namespace log4cxx;

LoggerPtr EvartGeneratorManagerTest::logger(Logger::getLogger("EvartGeneratorManagerTest"));

CPPUNIT_TEST_SUITE_REGISTRATION( EvartGeneratorManagerTest );

int __nsleep(const struct timespec *req, struct timespec *rem)
{
	struct timespec temp_rem;
	if(nanosleep(req,rem)==-1)
		__nsleep(rem,&temp_rem);
	else
		return 1;
}

int msleep(unsigned long milisec)
{
	struct timespec req={0},rem={0};
	time_t sec=(int)(milisec/1000);
	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;
	__nsleep(&req,&rem);
	return 1;
}

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

void EvartGeneratorManagerTest::setUp()
{

}

void EvartGeneratorManagerTest::tearDown()
{

}

void EvartGeneratorManagerTest::testEVaRTInitialization()
{
	LOG4CXX_INFO(logger, "Test: " << __FUNCTION__);

	EvartGeneratorManager& mgr = EvartGeneratorManager::getInstance();

	CPPUNIT_ASSERT( !mgr.AcceptingSdkData() );

	mgr.init("10.1.1.199", "10.1.1.153");

	//std::cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
	//std::cin.get();
	unsigned int n=0;
	while ( n < 5 )
	{
		n++;
		msleep(1000);
	}

	CPPUNIT_ASSERT( mgr.AcceptingSdkData() );
}
