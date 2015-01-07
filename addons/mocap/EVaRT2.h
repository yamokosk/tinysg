/*=========================================================
//
// File: EVaRT2.h  v200
//
// Created by Ned Phipps, Oct-2004
//
// This file defines the interface to EVaRT ethernet communication.
//
//----------------------------------------------------------
Modification History:

Date      By          Comment
------------------------------------------------------------
Oct 2004  np          First version

=============================================================================*/

/*! \file EVaRT2.h
This file defines the structures and the API for ethernet communication of data
between EVaRT and multiple client programs.
*/

#ifndef EVaRT2_H
#define EVaRT2_H

#ifdef _WINDOWS
    #define DLL __declspec(dllexport)
#else
    #define DLL
#endif


/** Return codes
*/
typedef enum maReturnCode
{
    RC_Okay=0,             //!< Okay
    RC_GeneralError,       //!< General Error
    RC_ApiError,           //!< Invalid use of the API
    RC_NetworkError,       //!< Network Error
    RC_TimeOut,            //!< No response from EVaRT
    RC_MemoryError,        //!< Memory allocation failed
    RC_Unrecognized        //!< Request string not recognized
}
maReturnCode;


/** Verbosity setting for internal messages
*/
typedef enum maVerbosityLevel
{
    VL_None=0,   //!< No Messages
    VL_Error,    //!< Error Message
    VL_Warning,  //!< Warning Message [DEFAULT VALUE]
    VL_Info,     //!< Informational Message
    VL_Debug,    //!< Debug Message
}
maVerbosityLevel;


// Array dimensions

#define MAX_N_BODIES      20


#define XEMPTY 9999999.0f


/** Data for one segment
*/
typedef double tSegmentData[7]; //!<  X,Y,Z, aX,aY,aZ, Length

/** Data for one marker
*/
typedef float  tMarkerData[3];  //!<  X,Y,Z

/** Data for one forceplate
*/
typedef float  tForceData[7];   //!<  X,Y,Z, fX,fY,fZ, mZ

/** Data for one degree of freedom
*/
typedef double tDofData;        //!<  Usually an angle value in degrees




//==================================================================
//==================================================================
//
//   S T R U C T U R E S
//
//==================================================================
//==================================================================


//==================================================================

/** The description of the connection to EVaRT.
*/
typedef struct sHostInfo
{
    int           bFoundHost;              //!< True = have talked to EVaRT
    int           LatestConfirmationTime;  //!< Time of last receipt from EVaRT
    char          szHostMachineName[128];  //!< Name of machine EVaRT is running on
    unsigned char HostMachineAddress[4];   //!< IP Address of that machine 
    char          szHostProgramName[128];  //!< Name of module communicating with
    unsigned char HostProgramVersion[4];   //!< Version number of that module

} sHostInfo;


//==================================================================

/** The rudimentary description of a skeleton's bones and hierarchy.
*/
typedef struct sHierarchy
{
    int            nSegments;         //!< Number of segments
    char**         szSegmentNames;    //!< Array of segment names
    int*           iParents;          //!< Array of segment parents (defines the hierarchy)

} sHierarchy;


//==================================================================

/** The description of a single tracking object that will have streaming data.
*/
typedef struct sBodyDef
{
    char*          szName;            //!< Name of the object

    int            nMarkers;          //!< Number of markers
    char**         szMarkerNames;     //!< Array of marker names

    sHierarchy     Hierarchy;         //!< The Skeleton description for HTR data

    int            nDofs;             //!< Number of degrees of freedom
    char**         szDofNames;        //!< Array of degrees of freedom names

} sBodyDef;


//==================================================================

/** The description of all the data that will stream from EVaRT
*/
typedef struct sBodyDefs
{
    int            nBodyDefs;               //!< Number of bodies being tracked
    sBodyDef       BodyDefs[MAX_N_BODIES];  //!< The definition of each body

    int            nAnalogChannels;         //!< The number of active analog channels
    char**         szAnalogChannelNames;    //!< The names given to each channel

    int            nForcePlates;            //!< The number of active forcesplates

    void*          AllocatedSpace;          //!< Private space (DON'T TOUCH)

} sBodyDefs;


//==================================================================

/** A structure containing ALL the data to drive one markerset.
*/
typedef struct sBodyData
{
    char           szName[128];          //!< For dynamic matching of objects.

    int            nMarkers;             //!< Number of markers defined
    tMarkerData*   Markers;              //!< [nMarkers][3] array.  Markers[iMarker][0] == XEMPTY means no data.
    float          fAvgMarkerResidual;   //!< Average residual of the marker triangulations

    int            nSegments;            //!< Number of segments
    tSegmentData*  Segments;             //!< [nSegments][7] array

    int            nDofs;                //!< Number of degrees-of-freedom
    tDofData*      Dofs;                 //!< Array of degree-of-freedom angles
    float          fAvgDofResidual;      //!< Average residual from the solve
    int            nIterations;          //!< Number of iterations to solve

} sBodyData;


//==================================================================

/** All the analog data for one frame's worth of time.
*/
typedef struct sAnalogData
{
    int            nAnalogChannels;  //!< Total number of active channels
    int            nAnalogSamples;   //!< The number of samples in the current frame
    short*         AnalogSamples;    //!< The data: nChannels * nSamples of these

    int            nForcePlates;     //!< Total number of active forceplates
    int            nForceSamples;    //!< The number of samples in the current frame
    tForceData*    Forces;           //!< The forces: nForcePlates * nForceSamples of these

} sAnalogData;


//==================================================================

/** ALL the data for one frame streamed from EVaRT.
*/
typedef struct sFrameOfData
{
    int            iFrame;                  //!< EVaRT's frame number
    float          fDelay;                  //!< Total time (seconds) from Camera to the Host sending the data

    int            nBodies;                 //!< The bodies should match the descriptions
    sBodyData      BodyData[MAX_N_BODIES];  //!< The data for each body.

    int            nUnidentifiedMarkers;    //!< Number of unrecognized markers
    tMarkerData*   UnidentifiedMarkers;     //!< The unrecognized markers.

    sAnalogData    AnalogData;              //!< The analog data packaged.

} sFrameOfData;



#ifdef  __cplusplus
extern "C" {
#endif

DLL int EVaRT2_GetSdkVersion(unsigned char Version[4]);
DLL int EVaRT2_SetVerbosityLevel(int iLevel);
DLL int EVaRT2_SetErrorMsgHandlerFunc(void (*MyFunction)(int iLogLevel, char* szLogMessage));
DLL int EVaRT2_SetDataHandlerFunc(void (*MyFunction)(sFrameOfData* pFrameOfData));
DLL int EVaRT2_Initialize(char* szMyNicCardAddress, char* szEVaNicCardAddress);  // Hostname or IP Address or NULL (auto find)
DLL int EVaRT2_Request(char* szCommand, void** ppResponse, int *pnBytes);  // Friendly extendable command function.
DLL int EVaRT2_FreeBodyDefs(sBodyDefs* pBodyDefs);
DLL int EVaRT2_PushBasePosition(sHierarchy* pHierarchy, tSegmentData* pPosition);
DLL int EVaRT2_Exit();
DLL int EVaRT2_GetHostInfo(sHostInfo *pHostInfo);

DLL sBodyDefs*     EVaRT2_GetBodyDefs();      // The description of what we are tracking.
DLL sFrameOfData*  EVaRT2_GetCurrentFrame();  // Can POLL for the current frame.

DLL int EVaRT2_CopyFrame(const sFrameOfData* pSrc, sFrameOfData* pDst);  // Allocates or reallocate pointers
DLL int EVaRT2_FreeFrame(sFrameOfData* pFrame);

DLL int EVaRT2_SendHtr(sHierarchy *pHierarchy, tSegmentData *pFrame);    // Push a skeleton definition to EVaRT


//==================================================================
// Euler angle utility functions
//==================================================================

#define ZYX_ORDER 1
#define XYZ_ORDER 2
#define YXZ_ORDER 3
#define YZX_ORDER 4
#define ZXY_ORDER 5
#define XZY_ORDER 6

DLL void  EVaRT2_ConstructRotationMatrix(
        double angles[3],
        int iRotationOrder,
        double matrix[3][3]);

DLL void  EVaRT2_ExtractEulerAngles(
        double matrix[3][3],
        int    iRotationOrder,
        double angles[3]);



#ifdef  __cplusplus
}
#endif

#endif
