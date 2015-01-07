/**
 * \file EvartSdk2Interface.cpp
 * \brief Definition of the wrapper around the EVaRT SDK2.
 */

#include "EvartSdk2Interface.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

// Initialize static variables
ISdkMessageLogger* EvartSdk2Interface::c_messageLogger = NULL;
ISdkDataListener* EvartSdk2Interface::c_dataListener = NULL;
int EvartSdk2Interface::c_frameReduction = 1;
int EvartSdk2Interface::c_framesUntilForward = 0;

using namespace log4cxx;

LoggerPtr EvartSdk2Interface::logger(Logger::getLogger("EvartSdk2Interface"));

// Constructor
EvartSdk2Interface::EvartSdk2Interface(const std::string& logFile, int level)
{
	m_initialized		= false;	// initialize has not been called
	m_unitConversion	= 1.0;		// assume EVaRT is in mm
	m_upAxis			= 1;		// assume EVaRT is Y-up
	m_captureRate		= 60.0;		// assume EVaRT is capturing at 60 fps
	m_logFile			= NULL;		// assume logging disabled
	m_msgCount			= 0;		// no messages received yet

	// set the verbosity level for messages from the SDK2
	if (level < VL_None)	level = VL_None;
	if (level > VL_Debug)	level = VL_Debug;
	EVaRT2_SetVerbosityLevel(level);

	// open a log file if specified
	if (logFile.length() > 0)
	{
		// this may fail, in which case logging is disabled
		m_logFile = fopen(logFile.c_str(), "w");

		// set myself as the message logger
		SetMessageLogger(this);
	}

	// set the message handler and data handler functions
    EVaRT2_SetErrorMsgHandlerFunc( MessageHandler );
    if ( EVaRT2_SetDataHandlerFunc( DataHandler ) != RC_Okay )
    {
    	LOG4CXX_ERROR(logger, "Error happened when registering DataHandler ");
    } else {
    	LOG4CXX_DEBUG(logger, "DataHandler registered successfully.");
    }
}

// Destructor
EvartSdk2Interface::~EvartSdk2Interface()
{
	Uninitialize();

	// close the log file if open
	if (m_logFile)
	{
		fclose(m_logFile);
	}

	SetMessageLogger(NULL);
	SetDataListener(NULL);
}

// Initialize the SDK2
bool EvartSdk2Interface::Initialize(const std::string& clientComputer, const std::string& evartComputer,
									sHostInfo* hostInfo, int frameReduction)
{
	int rc;

	// uninitialize first
	Uninitialize();

	// set the frame reduction factor
	SetFrameReduction(frameReduction);

	// try to initialize
	rc = EVaRT2_Initialize(const_cast<char*>(clientComputer.c_str()), const_cast<char*>(evartComputer.c_str()));
	if (rc == RC_Okay)
	{
		m_initialized = true;

		// query back the host info
		if (hostInfo)
		{
			// zero the host info structure and query EVaRT
			memset(hostInfo, 0, sizeof(*hostInfo));
			EVaRT2_GetHostInfo(hostInfo);
		}
	}

	return m_initialized;
}


// Uninitialize the SDK2
void EvartSdk2Interface::Uninitialize()
{
	if (m_initialized)
	{
		EVaRT2_Exit();
		m_initialized = false;
	}
}

// Return the version string for the SDK2
std::string EvartSdk2Interface::VersionString()
{
	unsigned char v[4];
	char buf[32];

	EVaRT2_GetSdkVersion(v);

	sprintf(buf, "%u.%u.%u", v[1], v[2], v[3]);

	return buf;
}

// Query the unit conversion
double EvartSdk2Interface::QueryUnitConversion(bool* status, double def)
{
	// assume there will be an error
	if (status) *status = false;

	// initialize to the default value in case there is an error
	m_unitConversion = def;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("GetConversionToMillimeters", &response, &nBytes);

		if (rc == RC_Okay)
		{
			m_unitConversion = *(float*)response;
			if (status) *status = true;
		}
	}

	return m_unitConversion;
}

// Query the up-axis
int EvartSdk2Interface::QueryUpAxis(bool* status, int def)
{
	// assume there will be an error
	if (status) *status = false;

	// initialize to the default value in case there is an error
	m_upAxis = def;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("GetUpAxis", &response, &nBytes);

		if (rc == RC_Okay)
		{
			m_upAxis = *(int*)response;
			if (status) *status = true;
		}
	}

	return m_upAxis;
}

// Query the capture rate
double EvartSdk2Interface::QueryCaptureRate(bool* status, double def)
{
	// assume there will be an error
	if (status) *status = false;

	// initialize to the default value in case there is an error
	m_captureRate = def;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("GetContextFrameRate", &response, &nBytes);

		if (rc == RC_Okay)
		{
			m_captureRate = *(float*)response;
			if (status) *status = true;
		}
	}

	return m_captureRate;
}

// Reset marker identifications
bool EvartSdk2Interface::ResetMarkerIds()
{
	bool success = false;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("ResetIDs", &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Reset marker identifications for a particular object
bool EvartSdk2Interface::ResetMarkerIdsForObject(const char* objectName)
{
	if (!objectName) return false;
	if (strlen(objectName) == 0) return false;

	bool success = false;

	if (m_initialized)
	{
		char command[512];
		void* response;
		int nBytes;
		int rc;

		// Form the full command
		sprintf(command, "ResetIDs=%s", objectName);

		rc = EVaRT2_Request(command, &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Request EVaRT to start recording
bool EvartSdk2Interface::RequestStartRecording()
{
	bool success = false;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("StartRecording", &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Request EVaRT to stop recording
bool EvartSdk2Interface::RequestStopRecording()
{
	bool success = false;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("StopRecording", &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Request live mode start
bool EvartSdk2Interface::RequestLiveModeStart()
{
	bool success = false;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("LiveMode", &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Request live mode pause
bool EvartSdk2Interface::RequestLiveModePause()
{
	bool success = false;

	if (m_initialized)
	{
		void* response;
		int nBytes;
		int rc;

		rc = EVaRT2_Request("Pause", &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Set the next take name
bool EvartSdk2Interface::SetTakeName(const char* takeName)
{
	if (!takeName) return false;
	if (strlen(takeName) == 0) return false;

	bool success = false;

	if (m_initialized)
	{
		char command[512];
		void* response;
		int nBytes;
		int rc;

		// Form the full command
		sprintf(command, "SetOutputName=%s", takeName);

		rc = EVaRT2_Request(command, &response, &nBytes);

		if (rc == RC_Okay) success = true;
	}

	return success;
}

// Get the current list of tracking object definitions in EVaRT.
sBodyDefs* EvartSdk2Interface::GetBodyDefinitions(int maxRetry)
{
	sBodyDefs* ptr = NULL;

	if (!m_initialized) return ptr;

	while (ptr == NULL && maxRetry-- > 0)
	{
		ptr = EVaRT2_GetBodyDefs();
	}

	return ptr;
}

// Get the current frame of data in EVaRT
sFrameOfData* EvartSdk2Interface::GetCurrentFrame()
{
	sFrameOfData* ptr = NULL;

	if (!m_initialized) return ptr;

	sFrameOfData* src = EVaRT2_GetCurrentFrame();
	if (src)
	{
		ptr = CopyFrame(src);
	}

	return ptr;
}

// Get the list of tracking objects and the current frame in EVaRT
void EvartSdk2Interface::GetBodyDefsAndCurrentFrame(sBodyDefs** bodyDefs, sFrameOfData** frameOfData)
{
	*bodyDefs = GetBodyDefinitions();
	*frameOfData = GetCurrentFrame();
}

// Send a skeleton base pose to EVaRT
bool EvartSdk2Interface::SendSkeletonBasePose(sHierarchy* hierarchy, tSegmentData* basePose)
{
	if (!hierarchy || !basePose || !m_initialized) return false;

	// it turns out that PushBasePosition is not actually implemented yet
	int rc = RC_ApiError; //EVaRT2_PushBasePosition(hierarchy, basePose);

	return rc == RC_Okay;
}

// Free a body definitions structure.
void EvartSdk2Interface::FreeBodyDefinitions(sBodyDefs* bodyDefs)
{
	if (bodyDefs)
	{
		EVaRT2_FreeBodyDefs(bodyDefs);
	}
}

// Copy a frame of data
sFrameOfData* EvartSdk2Interface::CopyFrame(const sFrameOfData* src)
{
	sFrameOfData* ptr = new sFrameOfData;
	if (ptr)
	{
		memset(ptr, 0, sizeof(*ptr));
		EVaRT2_CopyFrame(src, ptr);
	}

	return ptr;
}


// Free a frame of data
// This should only be used to free frames that were created using CopyFrame()
void EvartSdk2Interface::FreeFrame(sFrameOfData* frame)
{
	if (frame)
	{
		EVaRT2_FreeFrame(frame);
		delete frame;
	}
}

// Find a tracking object by name
int EvartSdk2Interface::FindObjectByName(sBodyDefs* bodyDefs, char* name)
{
	if (!bodyDefs)	return -1;
	if (!name)		return -1;

	// try to find the given object by name
	// note, search is NOT case sensitive
	int idx = -1;
	int i = 0;
	int count = bodyDefs->nBodyDefs;

	while (i < count && idx == -1)
	{
		if (bodyDefs->BodyDefs[i].szName &&
				strcmp(bodyDefs->BodyDefs[i].szName, name) == 0)
		{
			idx = i;
		}

		i++;
	}

	return idx;
}

// Handle a message from the SDK2.
void EvartSdk2Interface::SdkMessageHasArrived(int level, char* msg)
{
	if (m_logFile)
	{
		const char* type;
		struct tm* tm_ptr;
		time_t timer;
		int hh = 0, mm = 0, ss = 0;

		// get the time
		time(&timer);
		tm_ptr = localtime(&timer);
		if (tm_ptr)
		{
			hh = tm_ptr->tm_hour;
			mm = tm_ptr->tm_min;
			ss = tm_ptr->tm_sec;
		}

		switch (level)
		{
			case VL_Error:		type = "Error";		break;
			case VL_Warning:	type = "Warning";	break;
			case VL_Info:		type = "Info";		break;
			case VL_Debug:		type = "Debug";		break;
			default:			type = "Unknown";	break;
		}

		m_msgCount++;
		fprintf(m_logFile, "[%06d](%02d:%02d:%02d) [%s] %s\n", m_msgCount, hh, mm, ss, type, msg);
	}
}


// The function called by the SDK2 when a message is sent.
void EvartSdk2Interface::MessageHandler(int msgType, char* msg)
{
	LOG4CXX_DEBUG(logger, "EVaRT message arrived: " << msg);
	if (c_messageLogger)
	{
		c_messageLogger->SdkMessageHasArrived(msgType, msg);
	}
}

// The function called by the SDK2 when a frame of data arrives.
void EvartSdk2Interface::DataHandler(sFrameOfData* data)
{
	LOG4CXX_DEBUG(logger, "Frame " << data->iFrame << " arrived.");

	if (c_framesUntilForward <= 0)
	{
		c_framesUntilForward = c_frameReduction;

		LOG4CXX_DEBUG(logger, "c_framesUntilForward <= 0");

		if (c_dataListener && c_dataListener->AcceptingSdkData())
		{
			LOG4CXX_DEBUG(logger, "Calling SdkDataHasArrived of data listener.");
			c_dataListener->SdkDataHasArrived(data);
		}
	}

	c_framesUntilForward--;
}
