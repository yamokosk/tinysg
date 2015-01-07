#if !defined(EVART_SDK2_INTERFACE_H_INCLUDED_)
#define EVART_SDK2_INTERFACE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

/**
 * \file EvartSdk2Interface.h
 * \brief Declaration of a class which is a wrapper around the EVaRT SDK2.
 */

#include "EVaRT2.h"
#include <string>

// Logging
#include <log4cxx/logger.h>


/**
 * An interface for logging messages from EVaRT.
 */
class ISdkMessageLogger
{

public:

	/**
	 * Log a message from EVaRT.
	 * \param level, the level of the message
	 * \param msg, the message
	 */
	virtual void SdkMessageHasArrived(int level, char* msg) = 0;
};

/**
 * An interface for getting a frame of data from EVaRT.
 */
class ISdkDataListener
{

public:

	/**
	 * Receive a frame of data from EVaRT.
	 * \param data, a frame of data from the EVaRT SDK2
	 */
	virtual void SdkDataHasArrived(sFrameOfData* data) = 0;

	/**
	 * Determine if we are accepting data from EVaRT.
	 * \return true if you are accepting data, false otherwise
	 */
	virtual bool AcceptingSdkData() const = 0;
};

/**
 * A class to encapsulate the EVaRT SDK2.
 */
class EvartSdk2Interface : public ISdkMessageLogger
{
	static log4cxx::LoggerPtr logger;

public:

	/**
	 * Constructor.
	 * \param logfile, if non-NULL, message logging will be enabled
	 * \param level, the level of messages to log from EVaRT
	 */
	EvartSdk2Interface(const std::string& logFile = "", int level = VL_None);

	/**
	 * Destructor.
	 */
	~EvartSdk2Interface();

	/**
	 * Initialize the SDK2.
	 * \param clientComputer, the name or IP address of the local computer
	 * \param evartComputer, the name or IP address of the computer running EVaRT
	 * \param hostInfo, information about the SDK connection is returned here if non-NULL
	 * \param frameReduction, forwards 1 of N frames to data listener, where frameReduction == N
	 * \return true on success, false on error
	 */
	bool Initialize(const std::string& clientComputer, const std::string& evartComputer,
					sHostInfo* hostInfo = NULL, int frameReduction = 1);

	/**
	 * Uninitialize the SDK2.
	 */
	void Uninitialize();

	/**
	 * Log a message from EVaRT.
	 * \param level, the level of the message
	 * \param msg, the message
	 */
	virtual void SdkMessageHasArrived(int level, char* msg);

	/**
	 * Get the version of the SDK2.
	 * \return a string version of the SDK2
	 */
	static std::string VersionString();

	/**
	 * Query the conversion factor to mm.
	 * \param status, if non-NULL this will be set to false if an error occurs
	 * \param def, this is the default value to use in case there is an error
	 * \return the factor which converts the EVaRT unit to mm, or the default value if an error occurs
	 */
	double QueryUnitConversion(bool* status = NULL, double def = 1.0);

	/**
	 * Return the unit conversion, EVaRT is NOT queried.
	 * \return the current value of the unit conversion, without querying EVaRT
	 */
	double UnitConversion() const			{ return m_unitConversion; }

	/**
	 * Query the EVaRT up axis.
	 * \param status, if non-NULL this will be set to false if an error occurs
	 * \param def, this is the default value to use in case there is an error
	 * \return the EVaRT up-axis 0==X, 1==Y, 2==Z, returns the default value if an error occurs
	 */
	int QueryUpAxis(bool* status = NULL, int def = 1);

	/**
	 * Return the up-axis, EVaRT is NOT queried.
	 * \return the current value for the up-axis without querying EVaRT
	 */
	int UpAxis() const						{ return m_upAxis; }

	/**
	 * Query the EVaRT capture rate.
	 * \param status, in non-NULL this will be set to false if an error occurs
	 * \param def, this is the default value to use in case there is an error
	 * \return the current camera capture rate in frames per second, or the default value if an error occurs
	 */
	double QueryCaptureRate(bool* status = NULL, double def = 60.0);

	/**
	 * Return the capture rate, EVaRT is NOT queried.
	 * \return the current value for the capture rate without querying EVaRT
	 */
	double CaptureRate() const				{ return m_captureRate; }

	/**
	 * Return the effective frame rate, taking into account frame reduction.
	 * \return the effective frame rate, accounting for the frame reduction factor
	 */
	double EffectiveCaptureRate() const		{ return m_captureRate / c_frameReduction; }

	/**
	 * Ask EVaRT to reset the marker IDs.
	 * \return true on success, false on error
	 */
	bool ResetMarkerIds();

	/**
	 * Ask EVaRT to reset the marker IDs for a particular object.
	 * \param objectName, the name of the EVaRT marker set to reset IDs for
	 * \return true on success, false on error
	 */
	bool ResetMarkerIdsForObject(const char* objectName);

	/**
	 * Request EVaRT to start recording.
	 * \return true on success, false on error
	 */
	bool RequestStartRecording();

	/**
	 * Request EVaRT to stop recording.
	 * \return true on success, false on error
	 */
	bool RequestStopRecording();

	/**
	 * Tell EVaRT to start running in "Live" mode.
	 * This is the same as clicking the "Run" button on the EVaRT dashboard.
	 * \return true on success, false on error
	 */
	bool RequestLiveModeStart();

	/**
	 * Tell EVaRT to stop running in "Live" mode
	 * This is the same as clicking the "Pause" button on the EVaRT dashboard.
	 * \return true on success, false on error
	 */
	bool RequestLiveModePause();

	/**
	 * Set the name of the next take in EVaRT
	 * The string passed should have the form <take_name><take_number> like "BobRunning_1"
	 * \param takeName, the name of the next take
	 * \return true on success, false on error
	 */
	bool SetTakeName(const char* takeName);

	/**
	 * Get the current list of tracking object definitions in EVaRT.
	 * Use FreeBodyDefinitions to free the memory when you are finished with it.
	 * \param maxRetry, the maximum number of times to query for the body definitions
	 * \return a pointer to a sBodyDefs structure, or NULL on error
	 */
	sBodyDefs* GetBodyDefinitions(int maxRetry = 2);

	/**
	 * Get the current frame of data in EVaRT.
	 * Use FreeFrame to free the memory when you are finished with it.
	 * \return a pointer to a sFrameOfData structure, or NULL on error
	 */
	sFrameOfData* GetCurrentFrame();

	/**
	 * Get the list of tracking objects and the current frame from EVaRT
	 * \param bodyDefs, points to a newly allocated structure on successful return, NULL on error
	 * \param frameOfData, points to a newly allocated structure on successful return, NULL on error
	 */
	void GetBodyDefsAndCurrentFrame(sBodyDefs** bodyDefs, sFrameOfData** frameOfData);

	/**
	 * Send a skeleton base pose to EVaRT
	 * \param hierarchy, the definition of the skeleton's hierarchy
	 * \param basePose, the skeleton's base position
	 * \return true on success, false on error
	 */
	bool SendSkeletonBasePose(sHierarchy* hierarchy, tSegmentData* basePose);

	/**
	 * Set the frame reduction factor.
	 * \param frameReduction, means forward 1/N frames to data listener, where frameReduction == N
	 */
	void SetFrameReduction(int frameReduction)
	{
		c_frameReduction = frameReduction < 1 ? 1 : frameReduction;
		c_framesUntilForward = 0;
	}

	/**
	 * Get the frame reduction factor.
	 * \return the frame reduction factor
	 */
	int GetFrameReduction() const			{ return c_frameReduction; }

	/**
	 * Free a body definitions structure.
	 * \param bodyDefs, a pointer to the structure to be freed
	 */
	static void FreeBodyDefinitions(sBodyDefs* bodyDefs);

	/**
	 * Copy a frame of data.
	 * \param srcFrame, the frame to copy
	 * \return a pointer the the copied frame, or NULL on error
	 */
	static sFrameOfData* CopyFrame(const sFrameOfData* src);

	/**
	 * Free a frame of data.
	 * This should only be used to free frames that were created using CopyFrame().
	 * \param frame, a pointer to the structure to free
	 */
	static void FreeFrame(sFrameOfData* frame);

	/**
	 * Find a given tracking object by name.
	 * \param bodyDefs, the body definitions queried from EVaRT
	 * \param name, the name of the tracking object to find
	 * \return the index of the tracking object if found, -1 otherwise
	 */
	static int FindObjectByName(sBodyDefs* bodyDefs, char* name);

	/**
	 * Set the message logger object.
	 * \param logger, a pointer to an object which can handle messages
	 */
	static void SetMessageLogger(ISdkMessageLogger* logger)		{ c_messageLogger = logger; }

	/**
	 * Set the data listener object.
	 * \param listener, a pointer to an object which can handle frames of data
	 */
	static void SetDataListener(ISdkDataListener* listener)		{ c_dataListener = listener; }

	/**
	 * The function called by the SDK2 when a message is sent.
	 * \param msgType, the type of message
	 * \param msg, the actual message
	 */
	static void MessageHandler(int msgType, char* msg);

	/**
	 * The function called by the SDK2 when a frame of data is sent.
	 * \param data, the frame of data
	 */
	static void DataHandler(sFrameOfData* data);

private:

	static ISdkMessageLogger* c_messageLogger;	//!< a pointer to an object to handle messages
	static ISdkDataListener* c_dataListener;	//!< a pointer to an object to handle frames of data
	static int c_frameReduction;				//!< forward 1 of N frames to data listener, where m_frameReduction == N
	static int c_framesUntilForward;			//!< must wait for this number to get to zero before forwarding the next frame

	bool m_initialized;						//!< has the SDK been initialized
	double m_unitConversion;				//!< the number which converts the EVaRT unit to mm
	int m_upAxis;							//!< the EVaRT up-axis, 0==X, 1==Y, 2==Z
	double m_captureRate;					//!< the camera capture rate in fps

	FILE* m_logFile;						//!< where to log messages if logging has been enabled
	int m_msgCount;							//!< number of messages received
};

/**
 * A simple wrapper class around the sFrameOfData structure, mainly to handle creation/destruction.
 */
class FrameOfDataWrapper
{

public:

	/**
	 * Constructor.
	 * Create a new frame of data by copying the source frame.
	 * \param src, the source frame of data
	 */
	FrameOfDataWrapper(sFrameOfData* src) : m_ptr(0)	{ m_ptr = EvartSdk2Interface::CopyFrame(src); }

	/**
	 * Destructor.
	 */
	~FrameOfDataWrapper()		{ EvartSdk2Interface::FreeFrame(m_ptr); }

	/**
	 * Get a pointer to the underlying sFrameOfData struct.
	 * \return a pointer to the underlying sFrameOfData structure
	 */
	sFrameOfData* GetFrame()	{ return m_ptr; }

private:

	sFrameOfData* m_ptr;		//!< pointer to the frame of data we are accessing

};

#endif
