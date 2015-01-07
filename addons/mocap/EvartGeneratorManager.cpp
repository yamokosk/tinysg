/*
 * MoCapManager.cpp
 *
 *  Created on: Apr 15, 2009
 *      Author: yamokosk
 */

#include "EvartGeneratorManager.h"
#include <boost/foreach.hpp>

using namespace log4cxx;

LoggerPtr EvartGeneratorManager::logger(Logger::getLogger("EvartGeneratorManager"));

EvartGeneratorManager& EvartGeneratorManager::getInstance()
{
	static EvartGeneratorManager instance;

	return instance;
}

EvartGeneratorManager::EvartGeneratorManager() :
	mAcceptingEvartData(false),
	mLastEvartFrameNumber(0),
	mEvartSdk("evart_debug.txt", VL_Debug)
{
	// TODO Auto-generated constructor stub

}

EvartGeneratorManager::~EvartGeneratorManager()
{
	mEvartSdk.SetDataListener(NULL);
	mEvartSdk.Uninitialize();
}

void
EvartGeneratorManager::init(const std::string& serverIP, const std::string& clientIP)
{
	// Wipe out the marker to generator map. It will be reinitialized by this function.
	mMarkerIdGeneratorMap.clear();

	// Register ourselves as a listener
	mEvartSdk.SetDataListener(this);
	LOG4CXX_DEBUG(logger, "I am logged as the data listener.");

	// Initialize the EVaRT SDK2
	if ( !mEvartSdk.Initialize(clientIP, serverIP) )
	{
		LOG4CXX_ERROR(logger, "Communication with EVaRT SDK failed. Check network connections." );
		return;
	} else
	{
		LOG4CXX_DEBUG(logger, "EVaRT initialized.");
	}

	// Query the list of objects that EVaRT is currently tracking
	sBodyDefs* evartObjects = mEvartSdk.GetBodyDefinitions();

	if ( evartObjects != NULL )
	{
		mapMarkerNamesToGenerators(evartObjects);
		// Free memory for the EVaRT body definitions
		mEvartSdk.FreeBodyDefinitions(evartObjects);
	} else
	{
		mEvartSdk.Uninitialize();
		LOG4CXX_ERROR(logger, "Failed to get marker names for EVaRT." );
		return;
	}

	/*if ( mEvartSdk.RequestLiveModeStart() )
	{
		LOG4CXX_DEBUG(logger, "EVaRT is running.");

	} else
	{
		LOG4CXX_DEBUG(logger, "EVaRT is not listening to me.");
	}*/

	// We are now ready to accept frames of data from EVaRT
	mAcceptingEvartData = true;
}

bool EvartGeneratorManager::AcceptingSdkData() const
{
	return mAcceptingEvartData;
}

void EvartGeneratorManager::SdkDataHasArrived(sFrameOfData* frame)
{
	LOG4CXX_INFO(logger, "Frame " << frame->iFrame << " received.");

	// EVaRT frame number
	if ( ( mLastEvartFrameNumber + 1) != frame->iFrame )
	{
		LOG4CXX_WARN(logger, " Dropped a EVaRT data frame. Current frame = " << frame->iFrame << ". Last processed frame = " << mLastEvartFrameNumber << "." );
	}
	mLastEvartFrameNumber = frame->iFrame;

	// Update the global markers in the different generators
	for ( unsigned int bodynum=0; bodynum < frame->nBodies; ++bodynum )
	{
		// Average residuals of marker data
		//frame->BodyData[bodynum].fAvgMarkerResidual;

		for ( unsigned int markernum=0; markernum < frame->BodyData[bodynum].nMarkers; ++markernum )
		{
			MarkerIdGeneratorMap::iterator itr = mMarkerIdGeneratorMap.find(markernum);
			if (( itr != mMarkerIdGeneratorMap.end() ) &&
				(frame->BodyData[bodynum].Markers[markernum][0] < XEMPTY))
			{
				Vector3 coord( frame->BodyData[bodynum].Markers[markernum][0],
							   frame->BodyData[bodynum].Markers[markernum][1],
							   frame->BodyData[bodynum].Markers[markernum][2] );
				(itr->second)->updateMarkerWorldCoordinates(markernum, coord);
			}
		}
	}

	// Spawn a separate thread to handle the updating of data
	BOOST_FOREACH( MocapPoseGenerator* gen, mMocapPoseGenerators )
	{
		gen->updatePose();
	}
}

void EvartGeneratorManager::mapMarkerNamesToGenerators(sBodyDefs* bodyDefs)
{
	// Horribly inefficient, but this only needs to be done once.
	for (unsigned int i = 0; i < bodyDefs->nBodyDefs; i++)
	{
		for (unsigned int j = 0; j < bodyDefs->BodyDefs[i].nMarkers; j++)
		{
			std::string markerName( bodyDefs->BodyDefs[i].szMarkerNames[j] );

			BOOST_FOREACH( MocapPoseGenerator* gen, mMocapPoseGenerators )
			{
				if ( gen->hasMarkerById(j) )
				{
					mMarkerIdGeneratorMap[j] = gen;
				}
			}
		}
	}
}




