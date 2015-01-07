/*
 * MoCapManager.h
 *
 *  Created on: Apr 15, 2009
 *      Author: yamokosk
 */

#ifndef MOCAPMANAGER_H_
#define MOCAPMANAGER_H_

// Logging
#include <log4cxx/logger.h>

#include <map>
#include <list>

#include "EvartSdk2Interface.h"
#include "MocapPoseGenerator.h"

class EvartGeneratorManager : public ISdkDataListener
{
	static log4cxx::LoggerPtr logger;

	typedef std::map<unsigned int, MocapPoseGenerator*> MarkerIdGeneratorMap;
	typedef std::list<MocapPoseGenerator*> MocapPoseGeneratorList;
public:
	static EvartGeneratorManager& getInstance();

	virtual ~EvartGeneratorManager();

	void init(const std::string& serverIP, const std::string& clientIP);

	/**
	 * Receive a frame of data from EVaRT.
	 * \param data, a frame of data from the EVaRT SDK2
	 */
	virtual void SdkDataHasArrived(sFrameOfData* data);

	/**
	 * Determine if we are accepting data from EVaRT.
	 * \return true if you are accepting data, false otherwise
	 */
	virtual bool AcceptingSdkData() const;

private:
	// Constructor is private.. only one of these should ever be instantiated.
	EvartGeneratorManager();

	void mapMarkerNamesToGenerators(sBodyDefs* bodyDefs);

	EvartSdk2Interface mEvartSdk;
	MocapPoseGeneratorList mMocapPoseGenerators;
	MarkerIdGeneratorMap mMarkerIdGeneratorMap;
	bool mAcceptingEvartData;
	unsigned int mLastEvartFrameNumber;
};

#endif /* MOCAPMANAGER_H_ */
