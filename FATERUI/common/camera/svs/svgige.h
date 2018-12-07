/*******************************************************************************
 * SVS GigE API   Declaration of GigE camera access functions
 *******************************************************************************
 *
 *
 * Version:     2.4.1 / 2017 March
 *
 * Copyright:   SVS VISTEK GmbH
 *
 * Function categories:
 * ---------------------------------------------------------
 *    1 - Camera: Discovery and bookkeeping
 *    2 - Camera: Connection
 *    3 - Camera: Information
 *    4 - Stream: Channel creation and control
 *    5 - Stream: Channel statistics
 *    6 - Stream: Channel info
 *    7 - Stream: Transfer parameters
 *    8 - Stream: Image access
 *    9 - Stream: Image conversion
 *   10 - Stream: Image characteristics
 *   11 - Stream: Image statistics
 *   12 - Stream: Messaging channel
 *   13 - Controlling camera: Frame rate
 *   14 - Controlling camera: Exposure
 *   15 - Controlling camera: Gain and offset
 *   16 - Controlling camera: Auto gain/exposure
 *   17 - Controlling camera: Acquisition trigger
 *   18 - Controlling camera: Strobe
 *   19 - Controlling camera: Tap balance
 *   20 - Controlling camera: Image parameter
 *   21 - Controlling camera: Image appearance
 *   22 - Special control: IOMux configuration
 *   23 - Special control: IO control
 *   24 - Special control: Serial communication
 *   25 - Special control: Direct register and memory access
 *   26 - Special control: Persistent settings and recovery
 *   27 - General functions
 *   28 - Diagnostics
 * ---------------------------------------------------------
 *   99 - Deprecated functions
 *
 *******************************************************************************
 * Revision history:
 *
 * Version 2.4.1
 * Bugfix Release:
 *  rewrite the “gec” example to fix the issue from version 2.3.1.
 * 	
 *
 * Version 2.4.0
 * new feature: support Multicast 
 * function added:
 * Camera_setMulticastGroup()
 * 
 * breaking changes:
 * openCamera() / openCameraExt(), add one more parameter to support multicast mode
 * 
 * Version 2.3.1
 * function added:
 *  Camera_loadSequenceParameters();
 *
 *
 * Version 2.3.0
 * Bugfixed for Camera_setAcquisitionControl()/Camera_setAcquisitionMode() 
 * Bugfixed for discoverCameras()/discoverCamerasExt()
 *
 * function added:
 *  + Camera_setFlippingMode()
 *  + Camera_getFlippingMode()
 *  + Camera_setShutterMode()
 *  + Camera_getShutterMode()
 *
 *  + Camera_isLensAvailable()
 *  + Camera_getLensName()
 *  + Camera_setLensFocalLenght()
 *  + Camera_getLensFocalLenght()
 *  + Camera_getLensFocalLenghtMin()
 *  + Camera_getLensFocalLenghtMax()
 *  + Camera_setLensFocusUnit()
 *  + Camera_getLensFocusUnit()
 *  + Camera_setLensFocus()
 *  + Camera_getLensFocus()
 *  + Camera_getLensFocusMin()
 *  + Camera_getLensFocusMax()
 *  + Camera_setLensAperture()
 *  + Camera_getLensAperture()
 *  + Camera_getLensApertureMin()
 *  + Camera_getLensApertureMax()
 *
 * Verion 2.2.1
 * function added:
 *  + Camera_setTapConfigurationEx()
 *  + Camera_getTapConfigurationEx()
 * function deprecated:
 *  Camera_setTapConfiguration() // deprecated because of compatible reason
 *  Camera_getTapConfiguration() // deprecated because of compatible reason

 * Version 2.2.0
 * function added:
 *  + Camera_getMaxIOMuxIN()
 *  + Camera_getMaxIOMuxOUT()
 *  + Camera_getIOInputStatus()
 *  + Camera_getIOOutputStatus()
 *  + Camera_setIOConfig()
 *  + Camera_getIOConfig()
 *  + Camera_setIOAssignmentExt()
 *
 * bug-fixed:
 *  - remove the member variable camera with the type Camera_handle in SVGigE_IMAGE structure

 * Version 2.1.2 
 *  Bugfixed for discoverCameras()/discoverCamerasExt()
 *
 * Version 2.1.1
 *  correct the comments in svgige.h file
 *  Bugfixed for Camera_getOffsetMax()
 *  Bugfixed error on commode_update
 *  
 * Version 2.1.0
 * function added:
 *  + StreamingChannel_getTotalPacketResend()
 *  + StreamingChannel_getTotalPacketCount()
 *  + Image_getPacketCount()
 *  + Image_getPacketResend()
 *
 * Version: 2.0.0
 * function added:
 *  + SVGigE_getVersionInfo()
 *
 * Version 1.4.19.51-7 Beta /2013-07
 * bug-fixed
 *
 * Version 1.4.19.51-6 Beta /2013-07
 *
 * bug-fixed for Camera_closeCamera()
 *
 * Version 1.4.19.51-5 Beta /2013-04
 *
 *  bug-fixed
 *
 *  function removed:
 *  - forceIP(), please use forceIP_MAC() instead
 *
 *
 * Version 1.4.19.51-4 Beta /2012-11-22
 *
 * functions added:
 *  + Camera_getTimestampTickFrequency()
 *
 * functions removed:
 *  - Camera_getFrameRateRange()
 *  - Camera_getExposureTimeRange()
 *  - Camera_getGainMaxExtended()
 *  - Camera_setPrescalerDevisor()
 *  - Camera_getPrescalerDevisor()
 *  - Camera_setStrobePolarityExtended()
 *  - Camera_getStrobePolarityExtended()
 *  - Camera_setStrobePositionExtended()
 *  - Camera_getStrobePositionExtended()
 *  - Camera_setStrobeDurationExtended()
 *  - Camera_getStrobeDurationExtended()
 *  - Camera_getMaxAreaOfInterest()
 *  - Camera_setGammaCorrection()
 *  - StreamingChannel_getBufferData()
 *  - Image_getImageGray()
 *
 * Version 1.4.19.51-3 Beta /2012-10-30
 *
 * new functions:
 *  + StreamingChannel_getPixelType()
 *  + StreamingChannel_getBufferSize()
 *  + StreamingChannel_getImagePitch()
 *  + StreamingChannel_getImageSizeX()
 *  + StreamingChannel_getImageSizeY()
 *  + StreamingChannel_setStreamingPacketSize()
 *  + StreamingChannel_getStreamingPacketSize()
 *  + Image_getPixelType()
 *  + Image_getImageSize()
 *  + Image_getPitch()
 *  + Image_getSizeX()
 *  + Image_getSizeY()
 *  + Image_getImageId()
 *  + Image_getTimestamp()
 *
 * functions removed:
 *  - Image_getBufferIndex()
 *
 * Version 1.4.19.51-2 Beta /2012-10-17

 * new functions:
 *  + Camera_getPixelClock()
 *  + Camera_getSensorTemperature()
 *  + Camera_getFrameRateMin()
 *  + Camera_getFrameRateRange()
 *  + Camera_getFrameRateIncrement()
 *  + Camera_getExposureTimeMin()
 *  + Camera_getExposureTimeRange()
 *  + Camera_getExposureTimeIncrement()
 *  + Camera_getExposureDelayMax()
 *  + Camera_getExposureDelayIncrement()
 *  + Camera_getGainMaxExtended()
 *  + Camera_getGainIncrement()
 *  + Camera_getOffsetMax()
 *  + Camera_setAutoGainDynamics()
 *  + Camera_getAutoGainDynamics()
 *  + Camera_setTriggerPolarity()
 *  + Camera_getTriggerPolarity()
 *  + Camera_setTapConfiguration()
 *  + Camera_getTapConfiguration()
 *  + Camera_setAutoTapBalanceMode()
 *  + Camera_getAutoTapBalanceMode()
 *  + Camera_setTapBalance()
 *  + Camera_getTapBalance()
 *  + Camera_getTapGain()
 *  + Camera_getBufferSize()
 *  + Camera_getPitch()
 *  + Camera_getMaxAreaOfInterest()
 *  + Camera_getAreaOfInterestRange()
 *  + Camera_getAreaOfInterestIncrement()
 *  + Camera_resetTimestampCounter()
 *  + Camera_getTimestampCounter()
 *  + Camera_getWhiteBalanceMax()
 *  + Camera_getMaxIOMuxIN()
 *  + Camera_getMaxIOMuxOUT()
 *  + Camera_setIOAssignment()
 *  + Camera_getIOAssignment()
 *  + Camera_setIOMuxIN()
 *  + Camera_getIOMuxIN()
 *  + Camera_setIO()
 *  + Camera_getIO()
 *  + Camera_setAcqLEDOverride()
 *  + Camera_getAcqLEDOverride()
 *  + Camera_setLEDIntensity()
 *  + Camera_getLEDIntensity()
 *  + Camera_setUARTBuffer()
 *  + Camera_getUARTBuffer()
 *  + Camera_setUARTBaud()
 *  + Camera_getUARTBaud()

 * functions removed:
 *  - Camera_readXMLFile()
 *  - Camera_setLowpassFilter()
 *  - StreamingChannel_setSynchronous()
 *  - StreamingChannel_getSynchronous()

 *
 * Version 1.4.19.51-1 Beta /2012-10-09
 *------------------------------
 * Camera_getAreaOfInterestIncrement()
 *
 *
 * Version 1.4.19.51 Beta / 2012-09-27
 *------------------------------
   new functions:
 *  + Camera_isCameraFeature()
 *  + Camera_readEEPROM()
 *  + Camera_writeEEPROM()
 *  + getErrorMessage()
 *  + Camera_setStrobePolarityExtended()
 *  + Camera_getStrobePolarityExtended()
 *  + Camera_setStrobePositionExtended()
 *  + Camera_getStrobePositionExtended()
 *  + Camera_setStrobeDurationExtended()
 *  + Camera_getStrobeDurationExtended()
 *  + Camera_setPrescalerDevisor()
 *  + Camera_getPrescalerDevisor()
 *  + Camera_setPivMode()
 *  + Camera_getPivMode()
 *  + Camera_setDebouncerDuration()
 *  + Camera_getDebouncerDuration()
 *  + Camera_setTapGain()
 *  + Camera_restoreFactoryDefaults()

 *Version 1.4.19.50 Beta / 2012-08-16
 *------------------------------
 * new streaming technology
 * bug-fixed
 *
 *Version 1.4.19.49 / 2011-11-28
 *------------------------------
 * Camera_setIOAssignment() and
 * Camera_getIOAssignment() are called in the
 * sample program "gec.cpp"
 * force_IP function is available for the following
 * Serial number: AC:4F:FC:00:xx:xx
 *                 10:20:30:40:xx:xx
 *                xxxxx as integer for ex. the cameras
 *                from type 8050
 * Version 1.4.19.48 / 2011-03-04
 *------------------------------ * many new functions

 * Version 1.4.19.47 / 2010-11-09

 * 'force IP' function extended for new range of MAC: AC:4F:FC:00:xx:xx
 * Version 1.4.19.46 / 2010-06-17
 * ------------------------------
 * new functions:
 *  + setIPAddress()
 *  + getIPAddress()
 *  + setIPConfig()
 *  + getIPConfig()
 *
 * Version 1.4.19.44 / 2010-05-26
 * ------------------------------
 * new functions:
 *  + Camera_setStrobePolarity()
 *  + Camera_getStrobePolarity()
 *  + Camera_setStrobePosition()
 *  + Camera_getStrobePosition()
 *  + Camera_getStrobePositionMax()
 *  + Camera_getStrobePositionIncrement()
 *  + Camera_setStrobeDuration()
 *  + Camera_getStrobeDuration()
 *  + Camera_getStrobeDurationMax()
 *  + Camera_getStrobeDurationIncrement()
 *
 * Version 1.4.19.43 / 2010-01-29
 * ------------------------------
 *  - hidden thread start-up problem solved which prevented from receiving images (found on Fedora-12)
 *
 * new functions:
 *  + Camera_setInterPacketDelay()
 *  + Camera_getInterPacketDelay()
 *
 * Version 1.4.18.42 / 2009-12-07
 * ------------------------------
 *
 * changed functions:
 * NOTE: for all functions: Camera handle changed from reference (pointer)
 *       to value for robustness reasons (prevent segmentation fault)
 *
 *    openCamera(),discoverCameras(): GigE Vision assigned control port has been
 *        removed from parameter list because it will not change for GigE cameras,
 *        however, extended functions still accept a port assignment
 *
 * new functions:
 *  + openCameraExt()
 *  + discoverCamerasExt()
 *  + Camera_getPixelType()
 *  + Camera_setPixelDepth()
 *  + Camera_getPixelDepth()
 *  + Image_getImage12bitAs8bit()
 *  + Image_getImage12bitAs16bit()
 *  + Image_getImage16bitAs8bit()
 *  + Camera_softwareTrigger()
 *  + Camera_getFrameRateMax()
 *  + Camera_getExposureTimeMax()
 *  + Camera_getGainMax()
 *
 * deprecated functions:
 *	- Camera_getMaxFrameRate()
 *	- Camera_getMaxExposureTime()
 *	- Camera_getMaxGain()
 *	- Camera_startAcquisitionCycle()
 *
 * Version 1.4.16.35 / 2009-04-28
 * ------------------------------
 * functions added:
 *	+ StreamingChannel_setSynchronous()
 *	+ StreamingChannel_getSynchronous()
 *  + SVGigE_estimateWhiteBalance()
 *
 * Version 1.4.16.34 / 2009-04-15
 * ------------------------------
 *
 * Version 1.4.16.32
 * -----------------
 *  - Linux SDK adjusted to version 1.4.16.32 (Win)
 *
 *
 * Version 1.4.14.27
 * -----------------
 *  - derived from SVGigE SDK (Win) 1.4.14.26
 *
 *
 *******************************************************************************
 * Detailed function listing
 *******************************************************************************
 *
 * 1 - Camera: Discovery and bookkeeping
 * -------------------------------------
 * findNetworkAdapters()
 * discoverCameras()
 * discoverCamerasExt()
 * shutdownCameras()
 *
 * 2 - Camera: Connection
 * ----------------------
 * openCameraExt()
 * openCamera()
 * closeCamera()
 * setIPAddress()
 * getIPAddress()
 * setIPConfig()
 * getIPConfig()
 * forceIP_MAC()
 *
 * 3 - Camera: Information
 * -----------------------
 * Camera_getManufacturerName()
 * Camera_getModelName()
 * Camera_getDeviceVersion()
 * Camera_getManufacturerSpecificInformation()
 * Camera_getSerialNumber()
 * Camera_setUserDefinedName()
 * Camera_getUserDefinedName()
 * Camera_getIPAddress()
 * Camera_getSubnetMask()
 * Camera_getPixelClock()
 * Camera_getMacAddress()
 * Camera_isCameraFeature()
 * Camera_getSensorTemperature()
 *
 * 4 - Stream: Channel creation and control
 * ----------------------------------------
 * addStream()
 * addStreamExt()
 * enableStream()
 * closeStream()
 *
 * 5 - Stream: Channel statistics
 * ------------------------------------------
 * StreamingChannel_getFrameLoss()
 * StreamingChannel_getActualFrameRate()
 * StreamingChannel_getActualDataRate()
 *
 * 6 - Stream: Channel info
 * ------------------------
 * StreamingChannel_getPixelType()
 * StreamingChannel_getBufferSize()
 * StreamingChannel_getImagePitch()
 * StreamingChannel_getImageSizeX()
 * StreamingChannel_getImageSizeY()
 * StreamingChannel_getTotalPacketResend()
 * StreamingChannel_getTotalPacketResend()

 * 7 - Stream: Transfer parameters
 * -------------------------------
 * StreamingChannel_setStreamingPacketSize()
 * StreamingChannel_getStreamingPacketSize()
 * Camera_setInterPacketDelay()
 * Camera_getInterPacketDelay()
 *
 * 8 - Stream: Image access
 * ------------------------
 *  n.a.
 *
 * 9 - Stream: Image conversion
 * ----------------------------
 * Image_getImage12bitAs8bit()
 * Image_getImage12bitAs16bit()
 * Image_getImage16bitAs8bit()
 *
 * 10 - Stream: Image characteristics
 * ---------------------------------
 * Image_getPixelType()
 * Image_getImageSize()
 * Image_getPitch()
 * Image_getSizeX()
 * Image_getSizeY()
 *
 * 11 - Stream: Image statistics
 * ----------------------------
 * Image_getImageId()
 * Image_getTimestamp()
 * Image_getPacketCount()
 * Image_getPacketResend()
 *
 * 12 - Stream: Messaging channel
 * ------------------------------
 *  n.a.
 *
 * 13 - Controlling camera: Frame rate
 * -----------------------------------
 * Camera_setFrameRate()
 * Camera_getFrameRate()
 * Camera_getFrameRateMin()
 * Camera_getFrameRateMax()
 * Camera_getFrameRateIncrement()
 *
 * 14 - Controlling camera: Exposure
 * ---------------------------------
 * Camera_setExposureTime()
 * Camera_getExposureTime()
 * Camera_getExposureTimeMin()
 * Camera_getExposureTimeMax()
 * Camera_getExposureTimeIncrement()
 * Camera_setExposureDelay()
 * Camera_getExposureDelay()
 * Camera_getExposureDelayMax()
 * Camera_getExposureDelayIncrement()
 *
 * 15 - Controlling camera: Gain and offset
 * ----------------------------------------
 * Camera_setGain()
 * Camera_getGain()
 * Camera_getGainMax()
 * Camera_getGainIncrement()
 * Camera_setOffset()
 * Camera_getOffset()
 * Camera_getOffsetMax()
 *
 * 16 - Controlling camera: Auto gain/exposure
 * -------------------------------------------
 * Camera_setAutoGainEnabled()
 * Camera_getAutoGainEnabled()
 * Camera_setAutoGainBrightness()
 * Camera_getAutoGainBrightness()
 * Camera_setAutoGainDynamics()
 * Camera_getAutoGainDynamics()
 * Camera_setAutoGainLimits()
 * Camera_getAutoGainLimits()
 * Camera_setAutoExposureLimits()
 * Camera_getAutoExposureLimits()
 *
 * 17 - Controlling camera: Acquisition trigger
 * --------------------------------------------
 * Camera_setAcquisitionControl()
 * Camera_getAcquisitionControl()
 * Camera_setAcquisitionMode()
 * Camera_getAcquisitionMode()
 * Camera_softwareTrigger()
 * Camera_setTriggerPolarity()
 * Camera_getTriggerPolarity()
 * Camera_setPivMode()
 * Camera_getPivMode()
 * Camera_setDebouncerDuration()
 * Camera_getDebouncerDuration()
 *
 * 18 - Controlling camera: Strobe
 * -------------------------------
 * Camera_setStrobePolarity()
 * Camera_getStrobePolarity()
 * Camera_setStrobePosition()
 * Camera_getStrobePosition()
 * Camera_getStrobePositionMax()
 * Camera_getStrobePositionIncrement()
 * Camera_setStrobeDuration()
 * Camera_getStrobeDuration()
 * Camera_getStrobeDurationMax()
 * Camera_getStrobeDurationIncrement()
 *
 * 19 - Controlling camera: Tap balance
 * ------------------------------------
 * Camera_setTapConfigurationEx()
 * Camera_getTapConfigurationEx()
 * Camera_setAutoTapBalanceMode()
 * Camera_getAutoTapBalanceMode()
 * Camera_setTapBalance()
 * Camera_getTapBalance()
 * Camera_setTapGain()
 * Camera_getTapGain()
 *
 * 20 - Controlling camera: Image parameter
 * ---------------------------------------
 * Camera_getImagerWidth()
 * Camera_getImagerHeight()
 * Camera_getBufferSize()
 * Camera_getPitch()
 * Camera_getSizeX()
 * Camera_getSizeY()
 * Camera_setBinningMode()
 * Camera_getBinningMode()
 * Camera_setAreaOfInterest()
 * Camera_getAreaOfInterest()
 * Camera_getAreaOfInterestRange()
 * Camera_getAreaOfInterestIncrement()
 * Camera_resetTimestampCounter()
 * Camera_getTimestampCounter()
 * Camera_getTimestampTickFrequency()
 *
 * 21 - Controlling camera: Image appearance
 * -----------------------------------------
 * Camera_getPixelType()
 * Camera_setPixelDepth()
 * Camera_getPixelDepth()
 * Camera_setWhiteBalance()
 * Camera_getWhiteBalance()
 * Camera_getWhiteBalanceMax()
 * Camera_setLookupTableMode()
 * Camera_getLookupTableMode()
 * Camera_setLookupTable()
 * Camera_getLookupTable()
 *
 * 22 - Special control: IOMux configuration
 * -------------------------------------------------------
 * Camera_getMaxIOMuxIN()
 * Camera_getMaxIOMuxOUT()
 * Camera_setIOAssignment()
 * Camera_getIOAssignment()
 *
 * 23 - Special control: IO control
 * -------------------------------------------------------
 * Camera_setIOMuxIN()
 * Camera_getIOMuxIN()
 * Camera_setIO()
 * Camera_getIO()
 * Camera_setAcqLEDOverride()
 * Camera_getAcqLEDOverride()
 * Camera_setLEDIntensity()
 * Camera_getLEDIntensity()
 *
 * 24 - Special control: Serial communication
 * -------------------------------------------------------
 * Camera_setUARTBuffer()
 * Camera_getUARTBuffer()
 * Camera_setUARTBaud()
 * Camera_getUARTBaud()
 *
 * 25 - Special control: Direct register and memory access
 * -------------------------------------------------------
 * Camera_setGigECameraRegister()
 * Camera_getGigECameraRegister()
 * Camera_writeGigECameraMemory()
 * Camera_readGigECameraMemory()
 *
 * 26 - Special control: Persistent settings and recovery
 * ------------------------------------------------------
 * Camera_readEEPROM()
 * Camera_writeEEPROM()
 * Camera_restoreFactoryDefaults()
 *
 * 27 - General functions
 * ----------------------
 * SVGigE_estimateWhiteBalance()
 *
 * 28 - Diagnostics
 * ----------------
 * SVGigE_getVersionInfo()
 * getErrorMessage()
 *
 * ---------------------------------------------------------
 * 99 - Deprecated functions
 * ---------------------------------------------------------
 *	Camera_getMaxFrameRate()
 *	Camera_getMaxExposureTime()
 *	Camera_getMaxGain()
 *	Camera_startAcquisitionCycle()
 * Camera_setTapConfiguration()
 * Camera_getTapConfiguration()
 *
 *******************************************************************************
 */


#ifndef SVGigEH
#define SVGigEH
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// Structures and enumerations
//------------------------------------------------------------------------------

/** Version information
    *  The particular components of the version information will be represented
    *  in the following way
    */
#ifndef SVGigE_VERSION_DEFINED
#define SVGigE_VERSION_DEFINED
typedef struct
{ 
   unsigned char Major;
   unsigned char Minor;
   unsigned char SubMinor;
} SVGigE_VERSION;
#endif


/** Camera handle.
    *  A camera handle serves as a reference for access functions to camera
    *  functionality. A value of SVGigE_NO_CAMERA serves as an indicator for an
    *  invalid camera handle before obtaining a camera from a camera container.
    */
typedef size_t Camera_handle;
#define SVGigE_NO_CAMERA	(Camera_handle)-1

/** Streaming channel handle.
    *  A streaming channel handle serves as a reference for image acquisition
    *  functions
    */
typedef size_t Stream_handle;
#define SVGigE_NO_STREAMING_CHANNEL	(Stream_handle)-1

/** Image handle.
    *  An image handle serves as a reference for picture access and
    *  processing functions
    */
typedef int Image_handle;  

/** Function returns.
    *  API Functions may return success or error codes by this data type unless
    *  they return specific values
    */
typedef enum 
{
   SVGigE_SUCCESS                                      =  0,
   SVGigE_ERROR                                        = -1,
   SVGigE_DLL_NOT_LOADED                               = -2,
   SVGigE_DLL_VERSION_MISMATCH                         = -3,
   SVGigE_DRIVER_VERSION_MISMATCH                      = -4,
   SVGigE_WINSOCK_INITIALIZATION_FAILED                = -5,
   SVGigE_CAMERA_CONTAINER_NOT_AVAILABLE               = -6,
   SVGigE_NO_CAMERAS_DETECTED                          = -7,
   SVGigE_CAMERA_NOT_FOUND                             = -8,
   SVGigE_CAMERA_OPEN_FAILED                           = -9,
   SVGigE_CAMERA_COMMUNICATION_FAILED                  = -10,
   SVGigE_CAMERA_REGISTER_ACCESS_DENIED                = -11,
   SVGigE_UNKNOWN_LUT_MODE                             = -12,
   SVGigE_STREAMING_FUNCTION_ALREADY_REGISTERED        = -13,
   SVGigE_STREAMING_NOT_INITIALIZED                    = -14,
   SVGigE_OUT_OF_MEMORY                                = -15,
   SVGigE_INVALID_CALLBACK_INITIALIZATION              = -16,
   SVGigE_INVALID_CALLBACK_FUNCTION_POINTER            = -17,
   SVGigE_IMAGE_NOT_AVAILABLE                          = -18,
   SVGigE_INSUFFICIENT_RGB_BUFFER_PROVIDED             = -19,
   SVGigE_STREAMING_CHANNEL_NOT_AVAILABLE              = -20,
   SVGigE_INVALID_PARAMETERS                           = -21,
   SVGigE_PIXEL_TYPE_NOT_SUPPORTED                     = -22,
   SVGigE_FILE_COULD_NOT_BE_OPENED                     = -23,
   SVGigE_TRANSPORT_LAYER_NOT_AVAILABLE                = -24,
   SVGigE_XML_FILE_NOT_AVAILABLE                       = -25,
   SVGigE_WHITE_BALANCE_FAILED                         = -26,
   SVGigE_DEPRECATED_FUNCTION                          = -27,
   SVGigE_WRONG_DESTINATION_BUFFER_SIZE                = -28,
   SVGigE_INSUFFICIENT_DESTINATION_BUFFER_SIZE         = -29,
   SVGigE_CAMERA_NOT_IN_CURRENT_SUBNET                 = -30,
   SVGigE_CAMERA_MOVED_TO_FOREIGN_SUBNET               = -31,

   // Mapped camera return codes
   SVGigE_SVCAM_STATUS_ERROR                           = -101,
   SVGigE_SVCAM_STATUS_SOCKET_ERROR                    = -102,
   SVGigE_SVCAM_STATUS_ALREADY_CONNECTED               = -103,
   SVGigE_SVCAM_STATUS_INVALID_MAC                     = -104,
   SVGigE_SVCAM_STATUS_UNREACHABLE                     = -105,
   SVGigE_SVCAM_STATUS_ACCESS_DENIED                   = -106,
   SVGigE_SVCAM_STATUS_BUSY                            = -107,
   SVGigE_SVCAM_STATUS_LOCAL_PROBLEM                   = -108,
   SVGigE_SVCAM_STATUS_MSG_MISMATCH                    = -109,
   SVGigE_SVCAM_STATUS_PROTOCOL_ID_MISMATCH            = -110,
   SVGigE_SVCAM_STATUS_NOT_ACKNOWLEDGED                = -111,
   SVGigE_SVCAM_STATUS_RECEIVE_LENGTH_MISMATCH         = -112,
   SVGigE_SVCAM_STATUS_ACKNOWLEDGED_LENGTH_MISMATCH    = -113,
   SVGigE_SVCAM_STATUS_NO_ACK_BUFFER_PROVIDED          = -114,
   SVGigE_SVCAM_STATUS_CONNECTION_LOST                 = -115,
   SVGigE_SVCAM_STATUS_TL_NOT_AVAILABLE                = -116,
   SVGigE_SVCAM_STATUS_DRIVER_VERSION_MISMATCH         = -117,
   SVGigE_SVCAM_STATUS_FEATURE_NOT_SUPPORTED           = -118,
   SVGigE_SVCAM_STATUS_PENDING_CHANNEL_DETECTED        = -119,
   SVGigE_SVCAM_STATUS_LUT_NOT_AVAILABLE               = -120,
   SVGigE_SVCAM_STATUS_LUT_SIZE_MISMATCH               = -121,
   SVGigE_SVCAM_STATUS_NO_MATCHING_IP_ADDRESS          = -122,
   SVGigE_SVCAM_STATUS_DISCOVERY_INFO_CHANGED          = -123,
   SVGigE_SVCAM_STATUS_FIRMWARE_UPGRADE_REQUIRED       = -124,
   SVGigE_SVCAM_STATUS_MULTICAST_NOT_SUPPORTED         = -125,
   SVGigE_SVCAM_STATUS_PIXELDEPTH_NOT_SUPPORTED        = -126,
   SVGigE_SVCAM_STATUS_IO_CONFIG_NOT_SUPPORTED         = -127,
   SVGigE_SVCAM_STATUS_USER_DEFINED_NAME_TOO_LONG      = -128,
   SVGigE_SVCAM_STATUS_INVALID_IP                      = -129,
   SVGigE_SVCAM_STATUS_CAMERA_COMMUNICATION_ERROR      = -199,

   // Mapped transport layer return codes
   SVGigE_TL_SUCCESS                                   =    0,
   SVGigE_TL_DLL_NOT_LOADED                            = -201,
   SVGigE_TL_DLL_VERSION_MISMATCH                      = -202,
   SVGigE_TL_DLL_ALIGNMENT_PROBLEM                     = -203,
   SVGigE_TL_OPERATING_SYSTEM_NOT_SUPPORTED            = -204,
   SVGigE_TL_WINSOCK_INITIALIZATION_FAILED             = -205,
   SVGigE_TL_CAMERA_NOT_FOUND                          = -206,
   SVGigE_TL_CAMERA_OPEN_FAILED                        = -207,
   SVGigE_TL_CAMERA_NOT_OPEN                           = -208,
   SVGigE_TL_CAMERA_UNKNWON_COMMAND                    = -209,
   SVGigE_TL_CAMERA_PAYLOAD_LENGTH_EXCEEDED            = -210,
   SVGigE_TL_CAMERA_PAYLOAD_LENGTH_INVALID             = -211,
   SVGigE_TL_CAMERA_COMMUNICATION_TIMEOUT              = -212,
   SVGigE_TL_CAMERA_ACCESS_DENIED                      = -213,
   SVGigE_TL_CAMERA_CONNECTION_LOST                    = -214,
   SVGigE_TL_STREAMING_FUNCTION_ALREADY_REGISTERED     = -215,
   SVGigE_TL_NO_STREAMING_PORT_FOUND                   = -216,
   SVGigE_TL_OUT_OF_MEMORY                             = -217,
   SVGigE_TL_INVALID_CALLBACK_FUNCTION_POINTER         = -218,
   SVGigE_TL_STREAMING_CHANNEL_NOT_AVAILABLE           = -219,
   SVGigE_TL_STREAMING_CHANNEL_VERSION_MISMATCH        = -220,
   SVGigE_TL_CALLBACK_INVALID_PARAMETER                = -221,
   SVGigE_TL_CALLBACK_IMAGE_DATA_MISSING               = -222,
   SVGigE_TL_OPENING_STREAMING_CHANNEL_FAILED          = -223,
   SVGigE_TL_CHANNEL_CREATION_FAILED                   = -224,
   SVGigE_TL_DRIVER_NOT_INSTALLED                      = -225,
   SVGigE_TL_PENDING_CHANNEL_DETECTED                  = -226,
   SVGigE_TL_UNSUPPORTED_PACKET_FORMAT                 = -227,
   SVGigE_TL_INCORRECT_BLOCK_ID                        = -228,
   SVGigE_TL_INVALID_PARAMETER                         = -229,
   SVGigE_TL_STREAMING_CHANNEL_LOOSING_FRAMES          = -230,

   // Mapped GEV return codes
   SVGigE_GEV_STATUS_NOT_IMPLEMENTED                   = -301,
   SVGigE_GEV_STATUS_INVALID_PARAMETER                 = -302,
   SVGigE_GEV_STATUS_INVALID_ADDRESS                   = -303,
   SVGigE_GEV_STATUS_WRITE_PROTECT                     = -304,
   SVGigE_GEV_STATUS_BAD_ALIGNMENT                     = -305,
   SVGigE_GEV_STATUS_ACCESS_DENIED                     = -306,
   SVGigE_GEV_STATUS_BUSY                              = -307,
   SVGigE_GEV_STATUS_LOCAL_PROBLEM                     = -308,
   SVGigE_GEV_STATUS_MSG_MISMATCH                      = -309,
   SVGigE_GEV_STATUS_INVALID_PROTOCOL                  = -310,
   SVGigE_GEV_STATUS_NO_MSG                            = -311,
   SVGigE_GEV_STATUS_PACKET_UNAVAILABLE                = -312,
   SVGigE_GEV_STATUS_DATA_OVERRUN                      = -313,
   SVGigE_GEV_STATUS_INVALID_HEADER                    = -314,
   SVGigE_GEV_STATUS_ERROR                             = -399

} SVGigE_RETURN;

/** GigE transport layer type.
 *  The SVGigE functionality is exposed through a transport layer which
 *  connects to cameras in the network and which delivers images as well
 *  as signals.
 *  There are different types of transport layers, e.g. a NDIS filter
 *  driver which assembles images from particular network packets in an
 *  efficient way. A transport layer based on Winsock is another kind of
 *  transport layer which has a higher CPU load footprint but does not have
 *  a need for a driver being installed.
 *  It is recommended to use a driver for performance and reliability reasons.
 */
typedef enum
{
   SVGigETL_TypeNone             = 0,
   SVGigETL_TypeFilter           = 1,    // SVGigE filter driver (must be installed)
   SVGigETL_TypeWinsock          = 2,    // sockets available on Windows platforms
   SVGigETL_TypeLinuxKmod        = 3,    // loadable kernel module on Linux platforms
   SVGigETL_TypeLinuxSocket      = 4,    // sockets available on Linux platforms

}SVGigETL_Type;


/** Camera feature information.
    *  A camera can support several items from the following set of camera features.
    */
typedef enum
{
    CAMERA_FEATURE_SOFTWARE_TRIGGER                    = 0,  ///< camera can be triggered by software
    CAMERA_FEATURE_HARDWARE_TRIGGER                    = 1,  ///< hardware trigger supported as well as trigger polarity
    CAMERA_FEATURE_HARDWARE_TRIGGER_EXTERNAL_EXPOSURE  = 2,  ///< hardware trigger with internal exposure supported as well as trigger polarity
    CAMERA_FEATURE_FRAMERATE_IN_FREERUNNING_MODE       = 3,  ///< framerate can be adjusted (in free-running mode)
    CAMERA_FEATURE_EXPOSURE_TIME                       = 4,  ///< exposure time can be adjusted
    CAMERA_FEATURE_EXPOSURE_DELAY                      = 5,  ///< exposure delay can be adjusted
    CAMERA_FEATURE_STROBE                              = 6,  ///< strobe is supported (polarity, duration and delay)
    CAMERA_FEATURE_AUTOGAIN                            = 7,  ///< autogain is supported
    CAMERA_FEATURE_ADCGAIN                             = 8,  ///< 2009-05-15/deprecated
    CAMERA_FEATURE_GAIN                                = 8,  ///< the ADC's gain can be adjusted
    CAMERA_FEATURE_AOI                                 = 9,  ///< image acquisition can be done for an AOI (area of interest)
    CAMERA_FEATURE_BINNING                             = 10, ///< binning is supported
    CAMERA_FEATURE_UPDATE_REGISTER                     = 11, ///< streaming channel related registers can be pre-set and then updated at once (e.g. for changing an AOI)
    CAMERA_FEATURE_NOT_USED                            = 12, ///< not in use
    CAMERA_FEATURE_COLORDEPTH_8BPP                     = 13, ///< a pixel depth of 8bit is supported
    CAMERA_FEATURE_COLORDEPTH_10BPP                    = 14, ///< a pixel depth of 10bit is supported
    CAMERA_FEATURE_COLORDEPTH_12BPP                    = 15, ///< a pixel depth of 12bit is supported
    CAMERA_FEATURE_COLORDEPTH_16BPP                    = 16, ///< a pixel depth of 16bit is supported
    CAMERA_FEATURE_ADCOFFSET                           = 17, ///< the ADC's offset can be adjusted
    CAMERA_FEATURE_SENSORDATA                          = 18, ///< the camera's sensor/ADC settings can be adjusted (for dual tap cameras)
    CAMERA_FEATURE_WHITEBALANCE                        = 19, ///< a LUT for whitebalancing is available
    CAMERA_FEATURE_LUT_10TO8                           = 20, ///< a LUT from 10 bit to 8 bit is available
    CAMERA_FEATURE_LUT_12TO8                           = 21, ///< a LUT from 12 bit to 8 bit is available
    CAMERA_FEATURES_FLAGS                              = 22, ///< streaming state and image availability can be queried from camera
    CAMERA_FEATURES_READOUT_CONTROL                    = 23, ///< time of image read out from camera can be controlled by application
    CAMERA_FEATURES_TAP_CONFIG                         = 24, ///< the tap configuration can be changed (switching between one and two taps)
    CAMERA_FEATURES_ACQUISITION                        = 25, ///< acquisition can be controlled by start/stop
    CAMERA_FEATURES_TAPBALANCE                         = 26, ///< camera is capable of running auto tap balance
    CAMERA_FEATURES_BINNING_V2                         = 27, ///< camera offers extended binning modes
    CAMERA_FEATURES_ROTATE_180                         = 28, ///< image is rotated by 180
    CAMERA_FEATURES_CAMMODE_NG                         = 29, ///< camera has a next-generation register mapping
    CAMERA_FEATURES_CAMMODE_CLASSIC                    = 30, ///< camera has a classic register mapping
    CAMERA_FEATURES_NEXT_FEATUREVECTOR                 = 31, ///< a subsequent feature vector is available
    // Extended feature vector
    CAMERA_FEATURES2_START                             = 32, ///< first extended camera feature
    CAMERA_FEATURES2_1_TAP                             = 32, ///< camera supports a single tap sensor
    CAMERA_FEATURES2_2_TAP                             = 33, ///< camera supports a dual tap sensor
    CAMERA_FEATURES2_3_TAP                             = 34, ///< camera supports a triple tap sensor
    CAMERA_FEATURES2_4_TAP                             = 35, ///< camera supports a quadruple tap sensor
    CAMERA_FEATURES2_REBOOT                            = 36, ///< a remote reboot command is supported
    CAMERA_FEATURES2_IOMUX                             = 37, ///< IO multiplexer functionality is available
    CAMERA_FEATURES2_SOFTWARE_TRIGGER_ID               = 38, ///< writing a software trigger ID into images is supported
    CAMERA_FEATURES2_KNEE_POINTS                       = 39, ///< knee points available
    CAMERA_FEATURES2_NOISEFILTER                       = 40, ///< noise filter available
    CAMERA_FEATURES2_TRIGGERDEBOUNCE                   = 41, ///< trigger bounce can be activated
    CAMERA_FEATURES2_TEMPERATURE_SENSOR                = 42, ///< temperature sensor available
    CAMERA_FEATURES2_IOMUX_PWM                         = 43, ///< PWM A and B signals are available in IO multiplexer
    CAMERA_FEATURES2_IOMUX_STROBE2                     = 44, ///< STROBE0 and STROBE1 signals are available in IO multiplexer
    CAMERA_FEATURES2_PIV                               = 45, ///< PIV Mode
    CAMERA_FEATURES2_IOMUX_EXPOSE                      = 46, ///< EXPOSE signal is available in IO multiplexer
    CAMERA_FEATURES2_IOMUX_READOUT                     = 47, ///< READOUT signal is available in IO multiplexer
    CAMERA_FEATURES2_FLATFIELDCORRECTION               = 48, ///< FLATFIELDCORRECTION is available
    CAMERA_FEATURES2_SHADINGCORRECTION                 = 49, ///< SHADINGCORRECTION is available
    CAMERA_FEATURES2_DEFECTPIXEL                       = 50, ///< DEFECTPIXEL treatment is available
    CAMERA_FEATURES2_TRIGGERBOTHEDGES                  = 51, ///< TRIGGERBOTHEDGES is available
    CAMERA_FEATURES2_USERGAIN                          = 52, ///< USERGAIN is available
    CAMERA_FEATURES2_USEROFFSET                        = 53, ///< USEROFFSET is available
    CAMERA_FEATURES2_BINNING_X2                        = 54, ///< BINNING_X2 is available
    CAMERA_FEATURES2_BINNING_X3                        = 55, ///< BINNING_X3 is available
    CAMERA_FEATURES2_BINNING_X4                        = 56, ///< BINNING_X4 is available
    CAMERA_FEATURES2_IOMUX_LOGIC                       = 57, ///< IOMUX_LOGIC is available
    CAMERA_FEATURES2_IOMUX_STROBE4                     = 58, ///< IOMUX_STROBE4 is available
    CAMERA_FEATURES2_LENSCONTROL                       = 59, ///< LENSCONTROL is supported
    CAMERA_FEATURES2_1_TAP_1X_1Y                       = 60, ///< camera supports a single tap sensor
    CAMERA_FEATURES2_2_TAP_2XE_1Y                      = 61, ///< camera supports a dual tap left/right sensor
    CAMERA_FEATURES2_2_TAP_1X_2YE                      = 62, ///< camera supports a dual tap top/bottom sensor
    CAMERA_FEATURES2_4_TAP_2XE_2YE                     = 63, ///< camera supports a quad tap sensor
    // Extended feature vector
    CAMERA_FEATURES3_START                             = 64, ///< second extended camera feature
    CAMERA_FEATURES3_REVERSE_X	                       = 64, ///< camera supports horizontal flipping
    CAMERA_FEATURES3_REVERSE_Y	                       = 65, ///< camera supports vertical flipping
    CAMERA_FEATURES3_GLOBAL_SHUTTER                    = 66, ///< camera supports GLOBAL SHUTTER  Mode
    CAMERA_FEATURES3_ROLLING_SHUTTER                   = 67, ///< camera supports ROLLING SHUTTER Mode
    CAMERA_FEATURES3_MFT_FOCUS_UNIT                    = 68 ///< MFT focus unit can be changed
} CAMERA_FEATURE;


/** Look-up table mode.
    *  A camera can be instructed to apply a look-up table. Usually this will
    *  be used for running a gamma correction. However, other goals can also
    *  be implemented by a look-up table, like converting a gray-scale picture
    *  into a binary black/white picture.
    */
typedef enum  
{
   LUT_MODE_DISABLED               = 0,
   LUT_MODE_WHITE_BALANCE          = 1,    // 2006-12-20: deactivated, use "Camera_setWhiteBalance()" instead
   LUT_MODE_ENABLED                = 2,

} LUT_MODE;

/** Binning mode.
    *  A camera can be set to one of the following pre-defined binning modes
    */
typedef enum
{
   BINNING_MODE_OFF =0,
   BINNING_MODE_HORIZONTAL,
   BINNING_MODE_VERTICAL,
   BINNING_MODE_2x2,
   BINNING_MODE_3x3,
   BINNING_MODE_4x4,
} BINNING_MODE;

/** Low-pass filter.
 *  A low-pass filter can be activated/deactivated according to the
 *  following options.
 */
typedef enum
{
   LOWPASS_FILTER_NONE             = 0,
   LOWPASS_FILTER_3X3              = 1,
} LOWPASS_FILTER;

/** Multicast mode
 *  An application can receive images from a camera as a multicast controller,
 *  multicast listener or by non-multicast (default, unicast).
 */
typedef enum
{
   MULTICAST_MODE_NONE             = 0,
   MULTICAST_MODE_LISTENER         = 1,
   MULTICAST_MODE_CONTROLLER       = 2,
} MULTICAST_MODE;

/** PIV mode
 *  A  camera can be set to enabled or disabled PIV mode
 */
typedef enum
{
   PIV_MODE_DISABLED               = 0,
   PIV_MODE_ENABLED                = 1,
} PIV_MODE ;

/** Acquisition mode.
    *  A camera can be set to one of the following acquisition modes
    */
typedef enum
{
   ACQUISITION_MODE_NO_ACQUISITION             = 0,
   ACQUISITION_MODE_FREE_RUNNING               = 1,  // 2008-05-06: deprecated, replaced by "fixed frequency"
   ACQUISITION_MODE_FIXED_FREQUENCY            = 1,
   ACQUISITION_MODE_INT_TRIGGER                = 2,  // 2008-05-06: deprecated, replaced by "software trigger"
   ACQUISITION_MODE_SOFTWARE_TRIGGER           = 2,
   ACQUISITION_MODE_EXT_TRIGGER_INT_EXPOSURE   = 3,
   ACQUISITION_MODE_EXT_TRIGGER_EXT_EXPOSURE   = 4,

} ACQUISITION_MODE;

/** Acquisition control
    *  A camera can be set to the following acquisition control modes
    */
typedef enum
{
   ACQUISITION_CONTROL_STOP            = 0,
   ACQUISITION_CONTROL_START           = 1,
} ACQUISITION_CONTROL;

/** Trigger polarity.
    *  A camera can be set to positive or negative trigger polarity
    */
typedef enum
{
   TRIGGER_POLARITY_POSITIVE = 0,
   TRIGGER_POLARITY_NEGATIVE = 1,
} TRIGGER_POLARITY;

/** Strobe polarity.
    *  A camera can be set to positive or negative strobe polarity
    */
typedef enum
{
   STROBE_POLARITY_POSITIVE = 0,
   STROBE_POLARITY_NEGATIVE = 1,
} STROBE_POLARITY;

/** Bayer conversion method
    */
typedef enum
{
   BAYER_METHOD_NONE         = -1,
   // BAYER_METHOD_NEAREST      = 0,  NOTE Not supported
   BAYER_METHOD_SIMPLE       = 1,
   // BAYER_METHOD_BILINEAR     = 2, Not supported
   //BAYER_METHOD_HQLINEAR     = 3, Not supported
   //BAYER_METHOD_EDGESENSE    = 4, Not supported
   //BAYER_METHOD_GRAY         = 5, Not supported

} BAYER_METHOD;

/** Pixel depth defines.
    *  The following pixel depths can be supported by a camera
    */
typedef enum
{
   SVGIGE_PIXEL_DEPTH_UNKNOWN   =0,
   SVGIGE_PIXEL_DEPTH_8         =0,
   //SVGIGE_PIXEL_DEPTH_10        =1, // not supported
   SVGIGE_PIXEL_DEPTH_12        =2,
   SVGIGE_PIXEL_DEPTH_16        =3,
}SVGIGE_PIXEL_DEPTH;


/** Correction step.
 *  While performing image correction, a sequence of
 *  particular steps is needed as they are defined below .
 */
typedef enum
{
    IMAGE_CORRECTION_IDLE                   = 0,
    IMAGE_CORRECTION_ACQUIRE_BLACK_IMAGE    = 1,
    IMAGE_CORRECTION_ACQUIRE_WHITE_IMAGE    = 2,
    IMAGE_CORRECTION_SAVE_TO_EEPROM         = 3,
} IMAGE_CORRECTION_STEP;

/** Correction mode.
 *  After a successful image correction run, one of
 *  the following modes can be activated in order to
 *  control what image correction method is used.
 */
typedef enum
{
    IMAGE_CORRECTION_NONE         = 0,
    IMAGE_CORRECTION_OFFSET_ONLY  = 1,
    IMAGE_CORRECTION_ENABLED      = 2,
} IMAGE_CORRECTION_MODE;


/** Tap selection defines.
 *  Each tap of a 2-tap or 4-tap camera can be selected
 *  by using one of the following defines.
 */
typedef enum
{
   SVGIGE_TAP_SELECT_TAP0    = 1,
   SVGIGE_TAP_SELECT_TAP1    = 2,
   SVGIGE_TAP_SELECT_TAP2    = 3,
   SVGIGE_TAP_SELECT_TAP3    = 4,
}SVGIGE_TAP_SELECT;


/** Tap configuration	selection defines.
 *  Each configuration  of a 1-tap, 2-tap (horizontal and vertical) or 4-tap can be selected
 *  by using one of the following defines.
 */
typedef enum
{
    SVGIGE_SELECT_SINGLE_TAP	= 0,
    SVGIGE_SELECT_DUAL_TAP_H	= 1,
    SVGIGE_SELECT_DUAL_TAP_V	= 2,
    SVGIGE_SELECT_QUAD		 	= 3,
}SVGIGE_TAP_CONFIGURATION_SELECT;


/** flipping mode selection defines.
 *  the following modes of flipping are available
 */
typedef enum
{
    SVGIGE_REVERSE_OFF	= 0,
    SVGIGE_REVERSE_X	= 1,
    SVGIGE_REVERSE_Y	= 2,
    SVGIGE_REVERSE_X_Y	= 3,
}SVGIGE_FLIPPING_MODE ;


/** Shutter mode selection defines.
 *  the following modes of Shutter are available
 */
typedef enum
{
    SVGIGE_GLOBAL_SHUTTER  = 0,
    SVGIGE_ROLLING_SHUTTER = 1,
}SVGIGE_SHUTTER_MODE ;


/** Auto tap balance modes.
 *  The following modes of auto tap balancing are available
 */
typedef enum
{
   SVGIGE_AUTO_TAP_BALANCE_MODE_OFF          =0,
   SVGIGE_AUTO_TAP_BALANCE_MODE_ONCE         =1,
   SVGIGE_AUTO_TAP_BALANCE_MODE_CONTINUOUS   =2,
   SVGIGE_AUTO_TAP_BALANCE_MODE_RESET        =3,
}SVGIGE_AUTO_TAP_BALANCE_MODE;

/** Whitebalance Arts.
 *  The following Art of Whitebalance are available
 */
typedef enum
{
    Histogramm_Based_WB =0,
    Gray_Based_WB       =1,
}SVGIGE_Whitebalance_SELECT;


 /** The following maps for pixels correction are available
 */
typedef enum
{
    Factory_Map =0,
    SVS_Map     =1,
    Custom_Map  =2,
} PIXELS_CORRECTION_MAP_SELECT ;

/** following focus units are available
*/
typedef enum
    {
     One_mm_Unit = 0,   //  focus  unit: 1 mm
     Dec_mm_Unit = 1    //  focus  unit: 1/10 mm
    } FOCUS_UNIT;


/** Pixel type defines.
    *  The pixel type as it is defined in the GigE Vision specification
    */

// Indicate that pixel is monochrome
#define GVSP_PIX_MONO                       0x01000000
#define GVSP_PIX_RGB                        0x02000000

// Indicate effective number of bits occupied by the pixel (including padding).
// This can be used to compute amount of memory required to store an image.
#define GVSP_PIX_OCCUPY8BIT                 0x00080000
#define GVSP_PIX_OCCUPY12BIT                0x000C0000
#define GVSP_PIX_OCCUPY16BIT                0x00100000
#define GVSP_PIX_OCCUPY24BIT                0x00180000

// Bit masks
#define GVSP_PIX_COLOR_MASK                 0xFF000000
#define GVSP_PIX_EFFECTIVE_PIXELSIZE_MASK   0x00FF0000
#define GVSP_PIX_ID_MASK                    0x0000FFFF  

// Bit shift value
#define GVSP_PIX_EFFECTIVE_PIXELSIZE_SHIFT  16

typedef enum  
{
   // Unknown pixel format
   GVSP_PIX_UNKNOWN          = 0x0000,

   // Mono buffer format defines
#ifndef GVSP_PIX_MONO8
   GVSP_PIX_MONO8            = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0001),
   GVSP_PIX_MONO10           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0003),
   GVSP_PIX_MONO10_PACKED    = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT | 0x0004),
   GVSP_PIX_MONO12           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0005),
   GVSP_PIX_MONO12_PACKED    = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT | 0x0006),
   GVSP_PIX_MONO16           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0007),

   // Bayer buffer format defines
   GVSP_PIX_BAYGR8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0008),
   GVSP_PIX_BAYRG8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0009),
   GVSP_PIX_BAYGB8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x000A),
   GVSP_PIX_BAYBG8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x000B),
   GVSP_PIX_BAYGR10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000C),
   GVSP_PIX_BAYRG10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000D),
   GVSP_PIX_BAYGB10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000E),
   GVSP_PIX_BAYBG10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000F),
   GVSP_PIX_BAYGR12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0010),
   GVSP_PIX_BAYRG12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0011),
   GVSP_PIX_BAYGB12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0012),
   GVSP_PIX_BAYBG12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0013),
#endif

    GVSP_PIX_BAYGR12_PACKED = (GVSP_PIX_MONO  | GVSP_PIX_OCCUPY12BIT | 0x002A),
    GVSP_PIX_BAYRG12_PACKED = (GVSP_PIX_MONO  | GVSP_PIX_OCCUPY12BIT | 0x002B),
    GVSP_PIX_BAYGB12_PACKED = (GVSP_PIX_MONO  | GVSP_PIX_OCCUPY12BIT | 0x002C),
    GVSP_PIX_BAYBG12_PACKED = (GVSP_PIX_MONO  | GVSP_PIX_OCCUPY12BIT | 0x002D),

   // Color buffer format defines
   GVSP_PIX_RGB24            = (GVSP_PIX_RGB  | GVSP_PIX_OCCUPY24BIT),

   // Define for a gray image that was converted from a bayer coded image
   GVSP_PIX_GRAY8            = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT)

} GVSP_PIXEL_TYPE;


/** Signal types
    *  Each image that is delivered to an application by a callback will have a related signal
    *  which informs about the circumstances why a callback was triggered.
    *
    *  Usually a complete image will be delivered with the SVGigE_SIGNAL_FRAME_COMPLETED signal.
    */
typedef enum
{
    SVGigE_SIGNAL_NONE                              = 0,
    SVGigE_SIGNAL_FRAME_COMPLETED                   = 1,   // new image available, transfer was successful
    SVGigE_SIGNAL_FRAME_ABANDONED                   = 2,   // an image could not be completed in time and was therefore abandoned
    SVGigE_SIGNAL_START_OF_TRANSFER                 = 3,   // transfer of a new image has started
    SVGigE_SIGNAL_BANDWIDTH_EXCEEDED                = 4,   // available network bandwidth has been exceeded
    SVGigE_SIGNAL_OLD_STYLE_DATA_PACKETS            = 5,   // driver problem due to old-style driver behaviour (prior to 2003, not WDM driver)
    SVGigE_SIGNAL_TEST_PACKET                       = 6,   // a test packet arrived
    SVGigE_SIGNAL_CAMERA_IMAGE_TRANSFER_DONE        = 7,   // the camera has finished an image transfer
    SVGigE_SIGNAL_CAMERA_CONNECTION_LOST            = 8,   // connection to camera got lost
    SVGigE_SIGNAL_MULTICAST_MESSAGE                 = 9,   // an exceptional situation occurred during a multicast transmission
    SVGigE_SIGNAL_FRAME_INCOMPLETE                  = 10,  // a frame could not be properly completed
    SVGigE_SIGNAL_MESSAGE_FIFO_OVERRUN              = 11,  // a next entry was put into the message FIFO before the old one was released
    SVGigE_SIGNAL_CAMERA_SEQ_DONE                   = 12,  // the camera has finished a shutter sequence
    SVGigE_SIGNAL_CAMERA_TRIGGER_VIOLATION          = 13,  // the camera detected a trigger violation
    SVGigE_SIGNAL_CAMERA_FOUND
} SVGigE_SIGNAL_TYPE;

/** IO multiplexer IN signals (signal sources).
 *  A camera provides for a flexible IO signal handling where one or
 * multiple IN signals (signal sources) can be assigned to an OUT
 * signal (signal sink). The following IN signals are defined.
 */

typedef enum 
{
   SVGigE_IOMUX_IN0  =  (1 <<  0),
   SVGigE_IOMUX_IN1  =  (1 <<  1),
   SVGigE_IOMUX_IN2  =  (1 <<  2),
   SVGigE_IOMUX_IN3  =  (1 <<  3),
   SVGigE_IOMUX_IN4  =  (1 <<  4),
   SVGigE_IOMUX_IN5  =  (1 <<  5),
   SVGigE_IOMUX_IN6  =  (1 <<  6),
   SVGigE_IOMUX_IN7  =  (1 <<  7),
   SVGigE_IOMUX_IN8  =  (1 <<  8),
   SVGigE_IOMUX_IN9  =  (1 <<  9),
   SVGigE_IOMUX_IN10  = (1 << 10),
   SVGigE_IOMUX_IN11  = (1 << 11),
   SVGigE_IOMUX_IN12  = (1 << 12),
   SVGigE_IOMUX_IN13  = (1 << 13),
   SVGigE_IOMUX_IN14  = (1 << 14),
   SVGigE_IOMUX_IN15  = (1 << 15),
   SVGigE_IOMUX_IN16  = (1 << 16),
   SVGigE_IOMUX_IN17  = (1 << 17),
   SVGigE_IOMUX_IN18  = (1 << 18),
   SVGigE_IOMUX_IN19  = (1 << 19),
   SVGigE_IOMUX_IN20  = (1 << 20),
   SVGigE_IOMUX_IN21  = (1 << 21),
   SVGigE_IOMUX_IN22  = (1 << 22),
   SVGigE_IOMUX_IN23  = (1 << 23),
   SVGigE_IOMUX_IN24  = (1 << 24),
   SVGigE_IOMUX_IN25  = (1 << 25),
   SVGigE_IOMUX_IN26  = (1 << 26),
   SVGigE_IOMUX_IN27  = (1 << 27),
   SVGigE_IOMUX_IN28  = (1 << 28),
   SVGigE_IOMUX_IN29  = (1 << 29),
   SVGigE_IOMUX_IN30  = (1 << 30),
   SVGigE_IOMUX_IN31  = (1 << 31)
} SVGigE_IOMux_IN;

/** Some multiplexer's IN signals (signal sources)  have a
  *  pre-defined usage:
  */

#define SVGigE_IOMux_IN_UART2_OUT         SVGigE_IOMUX_IN24 // output of internal camera module UART2 (TXD)
#define SVGigE_IOMux_IN_TRIGGERFEEDBACK   SVGigE_IOMUX_IN23 // trigger feedback signal from camera control module
#define SVGigE_IOMux_IN_MFT_RXD           SVGigE_IOMUX_IN22 // RXD of the serial interface (RS232) of the MFT adapter
#define SVGigE_IOMux_IN_STROBE3           SVGigE_IOMUX_IN21 // STROBE3 signal from the camera (same as STROBE)
#define SVGigE_IOMux_IN_STROBE2           SVGigE_IOMUX_IN20 // STROBE2 signal from the camera (same as STROBE)
#define SVGigE_IOMUX_IN_LOGIC             SVGigE_IOMUX_IN19 // for logic trigger
#define SVGigE_IOMUX_IN_PWMMASK           SVGigE_IOMUX_IN18 // for PWMMASK
#define SVGigE_IOMUX_IN_PRESCALE          SVGigE_IOMUX_IN17 // pre-scaler for trigger purposes
#define SVGigE_IOMUX_IN_DEBOUNCE          SVGigE_IOMUX_IN16 // Reject short pulses (debounce)
#define SVGigE_IOMux_IN_SEQUENCE          SVGigE_IOMUX_IN15 // for SEQUENCE
#define SVGigE_IOMux_IN_PWMD              SVGigE_IOMUX_IN14 // PWMA signal (pulse width modulator)
#define SVGigE_IOMux_IN_PWMC              SVGigE_IOMUX_IN13 // PWMA signal (pulse width modulator)
#define SVGigE_IOMUX_IN_PULSE             SVGigE_IOMUX_IN12 // Pulse signal from camera
#define SVGigE_IOMux_IN_READOUT           SVGigE_IOMUX_IN11 // READOUT signal from camera
#define SVGigE_IOMux_IN_EXPOSE            SVGigE_IOMUX_IN10 // EXPOSE signal from camera
#define SVGigE_IOMux_IN_PWMB              SVGigE_IOMUX_IN9  // PWMB signal (pulse width modulator)
#define SVGigE_IOMux_IN_PWMA              SVGigE_IOMUX_IN8  // PWMA signal (pulse width modulator)
#define SVGigE_IOMux_IN_STROBE1           SVGigE_IOMUX_IN7  // STROBE1 signal from the camera
#define SVGigE_IOMux_IN_STROBE0           SVGigE_IOMUX_IN6  // STROBE0 signal from the camera (same as STROBE)
#define SVGigE_IOMux_IN_STROBE            SVGigE_IOMUX_IN6  // STROBE signal from the camera
#define SVGigE_IOMux_IN_UART_OUT          SVGigE_IOMUX_IN5  // Transmitter output from UART
#define SVGigE_IOMux_IN_IO_RXD            SVGigE_IOMUX_IN4  // Receiver IO line from camera connector
#define SVGigE_IOMux_IN_IN3               SVGigE_IOMUX_IN3  // IO_3
#define SVGigE_IOMux_IN_IN2               SVGigE_IOMUX_IN2  // IO_2
#define SVGigE_IOMux_IN_IN1               SVGigE_IOMUX_IN1  // IO_1
#define SVGigE_IOMux_IN_IN0               SVGigE_IOMUX_IN0  // IO_0
#define SVGigE_IOMux_IN_NOT_CONNECTED     0                 // not connected


/** Signal values for a particular signal
  */
typedef enum 
{
   IO_SIGNAL_OFF = 0,
   IO_SIGNAL_ON  = 1
}IO_SIGNAL;


/** IO multiplexer OUT signals (signals sinks).
 *  A camera provides for a flexible IO signal handling where one or
 *  multiple IN signals (signal sources) can be assigned to an OUT
 *  signal (signal sink). The following OUT signals are defined
 */

typedef enum 
{
   SVGigE_IOMUX_OUT0   =  0,
   SVGigE_IOMUX_OUT1   =  1,
   SVGigE_IOMUX_OUT2   =  2,
   SVGigE_IOMUX_OUT3   =  3,
   SVGigE_IOMUX_OUT4   =  4,
   SVGigE_IOMUX_OUT5   =  5,
   SVGigE_IOMUX_OUT6   =  6,
   SVGigE_IOMUX_OUT7   =  7,
   SVGigE_IOMUX_OUT8   =  8,
   SVGigE_IOMUX_OUT9   =  9,
   SVGigE_IOMUX_OUT10  = 10,
   SVGigE_IOMUX_OUT11  = 11,
   SVGigE_IOMUX_OUT12  = 12,
   SVGigE_IOMUX_OUT13  = 13,
   SVGigE_IOMUX_OUT14  = 14,
   SVGigE_IOMUX_OUT15  = 15,
   SVGigE_IOMUX_OUT16  = 16,
   SVGigE_IOMUX_OUT17  = 17,
   SVGigE_IOMUX_OUT18  = 18,
   SVGigE_IOMUX_OUT19  = 19,
   SVGigE_IOMUX_OUT20  = 20,
   SVGigE_IOMUX_OUT21  = 21,
   SVGigE_IOMUX_OUT22  = 22,
   SVGigE_IOMUX_OUT23  = 23,
   SVGigE_IOMUX_OUT24  = 24,
   SVGigE_IOMUX_OUT25  = 25,
   SVGigE_IOMUX_OUT26  = 26,
   SVGigE_IOMUX_OUT27  = 27,
   SVGigE_IOMUX_OUT28  = 28,
   SVGigE_IOMUX_OUT29  = 29,
   SVGigE_IOMUX_OUT30  = 30,
   SVGigE_IOMUX_OUT31  = 31
}SVGigE_IOMux_OUT;

typedef enum
{
    SVGigE_IO_MODE_NOT_INV = 0,  // original signal
    SVGigE_IO_MODE_INV     = 1,  // inverted signal
    SVGigE_IO_MODE_HIGH    = 2,  // static 1
    SVGigE_IO_MODE_LOW     = 3   // static 0
}SVGigE_IO_MODE;




/** Some of the multiplexer's OUT signals (signal sinks) have a
  *  pre-defined usage:
  */


#define SVGigE_IOMux_OUT_UART2_IN         SVGigE_IOMUX_OUT17  // UART2 (RXD)
#define SVGigE_IOMux_OUT_PULSE3           SVGigE_IOMUX_OUT16  // Pulse generator 3
#define SVGigE_IOMux_OUT_PULSE2           SVGigE_IOMUX_OUT15  // Pulse generator 2
#define SVGigE_IOMux_OUT_PULSE1           SVGigE_IOMUX_OUT14  // Pulse generator 1
#define SVGigE_IOMux_OUT_PULSE0           SVGigE_IOMUX_OUT13  // Pulse generator 0
#define SVGigE_IOMux_OUT_MFT_TXD          SVGigE_IOMUX_OUT12  // TXD of the serial interface (RS232) of the MFT adapter
#define SVGigE_IOMux_OUT_LOGICB           SVGigE_IOMUX_OUT11  // input B of internal camera module logic
#define SVGigE_IOMux_OUT_LOGICA           SVGigE_IOMUX_OUT10  // input A of internal camera module logic
#define SVGigE_IOMux_OUT_PRESCALE         SVGigE_IOMUX_OUT9   // Prescaler
#define SVGigE_IOMux_OUT_DEBOUNCE         SVGigE_IOMUX_OUT8   // Debouncer
#define SVGigE_IOMux_OUT_SEQUENZER        SVGigE_IOMUX_OUT7   // Sequenzer
#define SVGigE_IOMux_OUT_TRIGGER          SVGigE_IOMUX_OUT6   // Trigger signal to camera
#define SVGigE_IOMux_OUT_UART_IN          SVGigE_IOMUX_OUT5   // Receiver input to UART
#define SVGigE_IOMux_OUT_IO_TXD           SVGigE_IOMUX_OUT4   // Transmitter IO line from camera connector
#define SVGigE_IOMux_OUT_OUT3             SVGigE_IOMUX_OUT3   // IO_3
#define SVGigE_IOMux_OUT_OUT2             SVGigE_IOMUX_OUT2   // IO_2
#define SVGigE_IOMux_OUT_OUT1             SVGigE_IOMUX_OUT1   // IO_1
#define SVGigE_IOMux_OUT_OUT0             SVGigE_IOMUX_OUT0   // IO_0


/** Baud rate for a camera's UART
   *  A camera's UART can be set to the following pre-defined baud rates.
   */
typedef enum
{
   SVGigE_BaudRate_110        =    110,
   SVGigE_BaudRate_300        =    300,
   SVGigE_BaudRate_1200       =   1200,
   SVGigE_BaudRate_2400       =   2400,
   SVGigE_BaudRate_4800       =   4800,
   SVGigE_BaudRate_9600       =   9600,
   SVGigE_BaudRate_19200      =  19200,
   SVGigE_BaudRate_38400      =  38400,
   SVGigE_BaudRate_57600      =  57600,
   SVGigE_BaudRate_115200     = 115200
} SVGigE_BaudRate;


/** Camera buffer structure
    *  Camera information will be stored and transferred to the application
    *  using the following structure
    */
typedef struct {
   unsigned int controlIP;
   unsigned int controlSubnet;
   unsigned int controlGateway;
   unsigned long macHigh;
   unsigned long macLow;
   char ip[16];
   char subnet[16];
   char mac[18];
   char manufacturer[32];
   char model[32];
   char specificInformation[48];
   char deviceVersion[32];
   char serialNumber[16];
   char userName[16];
   unsigned short versionMajor;
   unsigned short versionMinor;
   unsigned int localIP;
   GVSP_PIXEL_TYPE PixelType;
   SVGIGE_PIXEL_DEPTH PixelDepth;
} SVGigE_CAMERA;

/** Image buffer structure
    *  An image data pointer along with accompanying data will be stored and
    *  transferred to the application using the following structure
    */
typedef struct {
   unsigned char *ImageData;///< pointer to image data
   int ImageSize;           ///< image buffer size
   int ImageWidth;          ///< image width
   int ImageHeight;         ///< image height
   int ImagePitch;          ///< image pitch
   unsigned int ImageID;    ///< block ID assigned by camera (1..65535,wraps around)
   // 2012-08-10/GR entry removed (cleanup)
   //int BufferIndex;       // buffer index of current image
   int ImageCount;          ///< total number of images
   int FrameLoss;           ///< lost images in streaming channel instance
   int PacketCount;	        ///< packet count for one image
   int PacketResend;        ///< total number of resend packets in streaming channel instance
   int TransferTime;        ///< Total packet transfer time from receiving first packet until finishing image and sending to user space [microseconds]

   // 2012-08-10/GR entry removed (cleanup)
   //unsigned char Header[576];           // header data of current image

   SVGigE_SIGNAL_TYPE SignalType;         ///< type of signal

   // 2012-08-10/GR entry removed (cleanup)
   //  SVGIGE_PIXEL_DEPTH PixelDepth;     // bit count of a pixel

   GVSP_PIXEL_TYPE PixelType;             ///< pixel type

   // 2012-08-10/GR entry removed (cleanup)
   //bool ImageLocked;                    // flag for locking image while doing image processing

   unsigned long long Timestamp;          ///< image timestamp
  // unsigned long long ClockFrequency;   // camera's time counter (clock) frequency

  // 2014-02-01 removed (cleanup)
  //Camera_handle camera;                  ///< camerahandle

} SVGigE_IMAGE; 

/** Signal structure.
 * A camera signal conveys information about an event that took place
 * during image acquisition. In case of the end-of-transfer signal
 * an image pointer along with accompanying image data will be delivered
 * to the application contained in a data transport structure.
 */
typedef struct {
   SVGigE_SIGNAL_TYPE SignalType;
   void *Data;
   int DataLength;
} SVGigE_SIGNAL;


/** Signal callback function
    * An application needs to register a callback function of the following type
    * in order to receive any signals from the SVGigE transport layer.
    */
typedef SVGigE_RETURN (*ApplicationCallback)(SVGigE_SIGNAL *Signal, void* Context);


//------------------------------------------------------------------------------
// 1 - Camera: Discovery and bookkeeping
//------------------------------------------------------------------------------

/** Find network adapters.
   *  Fill a list with the IPs of all network adapters the system has
   *
   *  @param Adapters pointer to an array where the IPs will be stored
   *  @param Size number of entrys the array can accommodate
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */

SVGigE_RETURN 
findNetworkAdapters(unsigned int *Adapters, unsigned char Size);

/** Discover cameras (previously used with ControlPort, now without that port).
   *  Search for Cameras in the given network and call a given function for each
   *
   *  @param SourceIP the IP of the network card to which the camera is connected
   *  @param DiscoveryTimeout time that the function waits for discovery replies
   *  @param Callback a callback function that will be called for each camera
   *  @param Context a context pointer that will be returned in each camera callback
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
discoverCameras(unsigned int SourceIP, unsigned long DiscoveryTimeout, ApplicationCallback Callback, void *Context);

/** Discover camerasExt (previous: discoverCameras).
   *  Search for Cameras in the given network and call a given function for each
   *
   *  @param ControlPort port to which the discover packets will be sent (GigE Vision default: 3956)
   *  @param SourceIP the IP of the network card to which the camera is connected
   *  @param DiscoveryTimeout time that the function waits for discovery replies
   *  @param Callback a callback function that will be called for each camera
   *  @param Context a context pointer that will be returned in each camera callback
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN 
discoverCamerasExt(unsigned int ControlPort, unsigned int SourceIP, unsigned long DiscoveryTimeout, ApplicationCallback Callback, void *Context);

/** Shutdown cameras.
   *  Release system resources from discovering cameras (e.g. camera infos)
   *
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
shutdownCameras();

//------------------------------------------------------------------------------
// 2 - Camera: Connection
//------------------------------------------------------------------------------

/** Open cameraExt (previous: openCamera).
   *  A GVCP connection to a GigE camera will be established
   *
   *  @param Camera a handle for camera communication will be returned
   *  @param CameraControlIP the camera's IP address
   *  @param CameraControlPort the camera's control port
   *  @param SourceIP the IP of the interface used to communicate with the camera  
   *  @param Heartbeat a heartbeat rate [ms] for keeping connection open
   *  @param MulticastMode in which mode camera should be opened, if there is no need to use multicast, please set it to MULTICAST_MODE_NONE
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
openCameraExt(Camera_handle *Camera, unsigned int CameraControlIP, unsigned int CameraControlPort, unsigned int SourceIP, unsigned int Heartbeat, MULTICAST_MODE MulticastMode);

/** Open camera (previously used  with CameraControlPort, now without that port).
   *  A GVCP connection to a GigE camera will be established
   *
   *  @param Camera a handle for camera communication will be returned
   *  @param CameraControlIP the camera's IP address
   *  @param SourceIP the IP of the interface used to communicate with the camera
   *  @param Heartbeat a heartbeat rate [ms] for keeping connection open
   *  @param MulticastMode in which mode camera should be opened, if there is no need to use multicast, please set it to MULTICAST_MODE_NONE
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN 
openCamera(Camera_handle *Camera, unsigned int CameraControlIP, unsigned int SourceIP, unsigned int Heartbeat, MULTICAST_MODE multicastMode);

/** closeCamera.
   *  The communication channel to a GigE camera will be closed
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
closeCamera(Camera_handle Camera);


/** Set MulticastGroup
   *  set the camera's multicast group(ip) and port for multicast participant,
   *  this function should be use after openCamera() / openCameraExt(), if multicasting is needed to apply,  
   *  thus there is no need to use this function
   * 
   *  @param Camera a handle from a camera that has been opened before
   *  @param MulticastGroup the multicast group ip address
   *  @param MulticastPort the multicast port
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setMulticastGroup(Camera_handle Camera, unsigned int MulticastGroup, unsigned short MulticastPort);

/** Set IP address.
   *  The camera will get a new persistent IP address assigned. The new persistent IP
   *  address will apply after camera's next reboot.
   *
   *  HINT:
   *  If an IP address is set that is not inside the subnet where the camera
   *  is currently connected to, then the camera becomes unavailable after next
   *  reboot.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param IPAddress a valid IP address
   *  @param NetMask a matching net mask
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
setIPAddress(Camera_handle Camera,unsigned int IPAddress,unsigned int NetMask);

/** Get IP address.
   *  The camera's persistent IP address will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param IPAddress persistent IP address
   *  @param NetMask persistent net mask
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
getIPAddress(Camera_handle Camera,unsigned int *IPAddress,unsigned int *NetMask);

/** Set IP config.
   *  The camera's IP configuration scheme will be determined.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param DHCP whether camera will try to get an IP from DHCP
   *  @param Persistent whether camera will reboot with a fixed IP
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
setIPConfig(Camera_handle Camera, bool DHCP, bool Persistent);

/** Get IP config.
   *  The camera's IP configuration scheme will be queried.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param DHCP whether camera will try to get an IP from DHCP
   *  @param Persistent whether camera will reboot with a fixed IP
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
getIPConfig(Camera_handle Camera, bool *DHCP, bool *Persistent);

/** Force IP.
   *  Change the camera's IP Address
   *
   *  @param MAC camera's mac address (last four hex digits without delimiter) or the complete mac Address with colons ":" or hyphens "-" delimiter
   *  @param IP address to set camera to
   *  @param Subnet subnet to set camera to
   *  @param SourceIP adapter's local IP address
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
// this function is removed, use forceIP_MAC() instead
//SVGigE_RETURN
//forceIP(unsigned int SN, unsigned int IP, unsigned int Subnet, unsigned int SourceIP);

SVGigE_RETURN
forceIP_MAC(char const* MAC, unsigned int IP, unsigned int Subnet, unsigned int SourceIP);


//* Force valid network settings.
//   *  @param  Camera a handle from a camera that has been opened before
//   *  @param  IPAddress the new IP address that has been selected and forced
//   *  @param  NetMask the new subnet mask has been selected and forced
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code

//TODO:SVGigE_RETURN
//forceValidNetworkSettings(Camera_handle Camera, unsigned int *IPAddress,unsigned int *NetMask);
/* forceValidNetworkSettings */

//* Restart IP configuration
//   *  A camera's IP configuration process will be restarted. Usually this
//   *  function will be used when a new DHCP address configuration should be
//   *  performed. If no DHCP server is available then a fallback to LLA will
//   *  take place. If, however, the camera has already got a persistent IP
//   *  assigned then the result of the IP configuration process will be the
//   *  already assigned fixed IP address.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_restartIPConfiguration(Camera_handle Camera);
/* Camera_restartIPConfiguration */

//------------------------------------------------------------------------------
// 3 - Camera: Information
//------------------------------------------------------------------------------

/** Get manufacturer name.
   *  The manufacturer name that is obtained from the camera firmware will be
   *  returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'manufacturer name' string
   */
char const*
Camera_getManufacturerName(Camera_handle Camera);

/** Get model name.
   *  The model name that is obtained from the camera firmware will be
   *  returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'model name' string
   */
char const*
Camera_getModelName(Camera_handle Camera);

/** Get device version.
   *  The device version that is obtained from the camera firmware will be
   *  returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'device version' string
   */
char const*
Camera_getDeviceVersion(Camera_handle Camera);

/** Get manufacturer specific information.
   *  The manufacturer specific information that is obtained from the camera
   *  firmware will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'manufacturer specific information' string
   */
char const*
Camera_getManufacturerSpecificInformation(Camera_handle Camera);

/** Get serial number.
   *  The (manufacturer-assigned) serial number will be obtained from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'serial number' string
   */
char const*
Camera_getSerialNumber(Camera_handle Camera);

/** Set user-defined name.
   *  A user-defined name will be uploaded to a camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param UserDefinedName the name to be transferred to the camera
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setUserDefinedName(Camera_handle Camera,
                          char const* UserDefinedName);

/** Get user-defined name.
   *  A name that has been assigned to a camera by the user will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'user-defined name' string
   */
char const*
Camera_getUserDefinedName(Camera_handle Camera);

/** Get IP address.
   *  The IP address that the camera is currently working on will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return an 'IP address' string
   */
char const*
Camera_getIPAddress(Camera_handle Camera);

/** Get subnet mask.
   *  The subnet mask that the camera is currently working with will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'subnet mask' string
   */
char const*
Camera_getSubnetMask(Camera_handle Camera);

/** Get MAC address.
   *  The MAC address that is obtained from the camera firmware will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return a 'MAC address' string
   */
char const*
Camera_getMacAddress(Camera_handle Camera);

/** Get pixel clock.
   *  The camera's pixel clock will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PixelClock a reference to the pixel clock value
   *  @return return success or error code
   */
SVGigE_RETURN
Camera_getPixelClock(Camera_handle Camera, int  *PixelClock);

/** Is camera feature.
   *  The camera will be queried whether a particular feature is available or not.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Feature a camera feature to be queried whether it is available
   *  @return a boolean value indicating whether the queried feature is available or not
   */
bool
Camera_isCameraFeature(Camera_handle Camera,
                       CAMERA_FEATURE Feature);


/**  Get sensor temperature.
   * The current camera's Sensor temperature  will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SensorTemperature the actual sensor temperature of the camera [°C]
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getSensorTemperature(Camera_handle Camera, unsigned int *SensorTemperature);

//------------------------------------------------------------------------------
// 4 - Stream: Channel creation and control
//------------------------------------------------------------------------------

/** Add stream.
   *  A streaming channel will be added to an open camera
   *
   *  @param Camera the camera handle that was received from openCamera()
   *  @param Stream a handle for the stream will be returned
   *  @param LocalIP the IP of the local streaming endpoint will be returned
   *  @param LocalPort the Port of the local streaming endpoint will be returned
   *  @param BufferSize the size of an image buffer
   *  @param BufferCount the number of image buffers
   *  @param PacketSize the network packet size to be used
   *  @param PacketResendTimeout the time from image transfer start when packet
   *         bookkeeping will be started and a packet resend may follow
   *  @param Callback a callback function that will be called for each image
   *  @param Context a context pointer that will be returned in each image callback
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
addStream(Camera_handle Camera,
          Stream_handle *Stream,
          unsigned int *LocalIP,
          unsigned short *LocalPort,
          int BufferSize,
          int BufferCount,
          int PacketSize,
          int PacketResendTimeout,
          ApplicationCallback Callback,
          void *Context);

/** Add streamExt.
   *  A streaming channel will be added to an open camera
   *
   *  @param Camera the camera handle that was received from openCamera()
   *  @param Stream a handle for the stream will be returned
   *  @param LocalIP the IP of the local streaming endpoint will be returned
   *  @param LocalPort the Port of the local streaming endpoint will be returned
   *  @param MaxPort the highest port number to be selected
   *  @param BufferSize the size of an image buffer
   *  @param BufferCount the number of image buffers
   *  @param PacketSize the network packet size to be used
   *  @param PacketResendTimeout the time from image transfer start when packet
   *         bookkeeping will be started and a packet resend may follow
   *  @param Callback a callback function that will be called for each image
   *  @param Context a context pointer that will be returned in each image callback
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
addStreamExt(Camera_handle Camera,
             Stream_handle *Stream,
             unsigned int *LocalIP,
             unsigned short *LocalPort,
             unsigned short MaxPort,
             int BufferSize,
             int BufferCount,
             int PacketSize,
             int PacketResendTimeout,
             ApplicationCallback Callback,
             void *Context);

/** Enable stream.
   *  The status of an opened stream will be set to enabled respectively
   *  to not enabled dependent on the "Enabled" argument. This function
   *  is intended to provide for a clean handling of acquisition mode
   *  changes, e.g. switching from free-running to triggered modes.
   *
   *  @param Stream a stream handle that was received from addStream()
   *  @param Enabled whether the stream is supposed to be enabled
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
enableStream(Stream_handle Stream, bool Enabled);

/** Close stream.
   *  A previously opened stream will be closed
   *
   *  @param Stream a stream handle that was received from addStream()
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
closeStream(Stream_handle Stream);

//------------------------------------------------------------------------------
// 5 - Stream: Channel statistics
//------------------------------------------------------------------------------

/** Get frame loss.
   *  The number of lost frames in a streaming channel will be returned
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param FrameLoss the number of frames that have been lost since the streaming channel was opened
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
StreamingChannel_getFrameLoss(Stream_handle Stream,int *FrameLoss);

/** Get actual frame rate.
   *  The actual frame rate calculated from received images will be returned
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param ActualFrameRate the actual frame rate measured based on received images
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
StreamingChannel_getActualFrameRate(Stream_handle Stream,float *ActualFrameRate);

/** Get actual data rate.
   *  The actual data rate calculated from received image data will be returned
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param ActualDataRate the actual data rate measured based on received image data
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
StreamingChannel_getActualDataRate(Stream_handle Stream,float *ActualDataRate);


//* Get peak data rate.
//   *  The peak data rate will be returned. It is determined for a single image
//   *  transfer by measuring the transfer time from first to last network packet
//   *  which belong to a single image. The peak data rate is received by dividing
//   *  the amount of data of one image by that transfer time. It can be used for
//   *  evaluating the bandwidth situation when operating multiple GigE cameras
//   *  over a single GigE line.
//   *
//   *  @param Stream a handle from a camera that has been opened before
//   *  @param PeakDataRate the actual frame rate measured based on received image data
//   *  @return success or error code

//TODO SVGigE_RETURN
//StreamingChannel_getPeakDataRate(StreamingChannel_handle hStreamingChannel,float *PeakDataRate);
/* StreamingChannel_getPeakDataRate */
//------------------------------------------------------------------------------
// 6 - Stream: Channel info
//------------------------------------------------------------------------------
/** Get pixel type.
   *  The pixel type will be returned that applies to the output image (or output
   *  view in case of a MultiStream channel).
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param PixelType the pixel type that has been set for the output image/view
   *  @return success or error code
   */
SVGigE_RETURN
StreamingChannel_getPixelType(Stream_handle Stream,GVSP_PIXEL_TYPE *PixelType);


/** Get buffer size.
   *  The buffer size will be returned that applies to the output image (or output
   *  view in case of a MultiStream channel).
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param BufferSize the buffer size for the output image/view
   *  @return success or error code
   */
SVGigE_RETURN
StreamingChannel_getBufferSize(Stream_handle Stream,int *BufferSize);

/** Get image pitch.
   *  The image pitch will be returned that applies to the output image (or output
   *  view in case of a MultiStream channel).
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param ImagePitch the image pitch for the output image/view
   *  @return success or error code
   */
SVGigE_RETURN
StreamingChannel_getImagePitch(Stream_handle Stream, int *ImagePitch);

/** Get image size X.
   *  The image size X will be returned that applies to the output image (or output
   *  view in case of a MultiStream channel).
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param ImageSizeX the image size X for the output image/view
   *  @return success or error code
   */
SVGigE_RETURN
StreamingChannel_getImageSizeX(Stream_handle Stream, int *ImageSizeX);

/** Get image size Y.
   *  The image size Y will be returned that applies to the output image (or output
   *  view in case of a MultiStream channel).
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param ImageSizeY the image size Y for the output image/view
   *  @return success or error code
   */
SVGigE_RETURN
StreamingChannel_getImageSizeY(Stream_handle Stream, int *ImageSizeY);


/** Get total packet resend.
   *  the total number of resent packets in streaming channel
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param TotalPacketResend the total number of resent packets since the streaming channel is created
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
StreamingChannel_getTotalPacketResend(Stream_handle Stream, int *TotalPacketResend);


/** Get total packet count.
   *  the total number of packets in streaming channel
   *
   *  @param Stream a stream handle from a camera that has been opened before
   *  @param TotalPacketCount the total number of packets since the streaming channel is created
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
StreamingChannel_getTotalPacketCount(Stream_handle Stream, int *TotalPacketCount);
//------------------------------------------------------------------------------
// 7 - Stream: Transfer parameters
//------------------------------------------------------------------------------

//* Set streaming packet size.
//   *  The packet size is set which will be generated by the camera for streaming
//   *  image data as payload packets to the host
//   *
//   *  WARNING! Explicitly setting network packet size to values above 1500 bytes
//   *           may provide to unpredictable results and also to a system crash.
//   *           Please use "Camera_evaluateMaximalPacketSize" for a save adjustment
//   *           to jumbo packets.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param StreamingPacketSize the packet size used by the camera for image packets
//   *  @return success or error code
//   */
////TODO:SVGigE_RETURN
////Camera_setStreamingPacketSize(Camera_handle Camera, int StreamingPacketSize);
///* Camera_setStreamingPacketSize

/** Set streaming packet size.
   *  The packet size is set which will be generated by the camera for streaming
   *  image data as payload packets to the host
   *
   *  WARNING! Explicitly setting network packet size to values above 1500 bytes
   *           may provide to unpredictable results and also to a system crash.
   *           Please use "Camera_evaluateMaximalPacketSize" for a save adjustment
   *           to jumbo packets.
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param StreamingPacketSize the new packet size setting which will be used by the camera for image packets
   *  @return success or error code
     */

SVGigE_RETURN
StreamingChannel_setStreamingPacketSize(Stream_handle Stream, int StreamingPacketSize);

 /** Get streaming packet size.
   *  The packet size which used for streaming image data as payload packets to the host
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param StreamingPacketSize the current packet size used by the camera for image packets
   *  @return success or error code
   * */
SVGigE_RETURN
StreamingChannel_getStreamingPacketSize(Stream_handle Stream, int *StreamingPacketSize);

/** Set inter-packet delay.
   *  A delay between network packets will be introduced and set to a specified
   *  number of ticks.
   *
   *  NOTE: The resulting total image transfer time must not exceed 250 ms.
   *  Otherwise a time-out condition will be reached in the SVGigE driver.
   *  The dependency between inter-packet delay and total image transfer time
   *  depends on pixel clock, image size and has to be determine case by case.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param InterPacketDelay the new value for inter-packet delay
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setInterPacketDelay(Camera_handle Camera,
                           float InterPacketDelay);

/** Get inter-packet delay.
   *  The delay between network packets will be returned as a relative number
   *  of ticks.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param InterPacketDelay the currently programmed value to an inter-packet delay
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getInterPacketDelay(Camera_handle Camera,
                           float *InterPacketDelay);
//* Set multicast mode
//   *  The multicast mode will be set to none (default), listener or controller.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param MulticastMode the intended new multicast mode
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_setMulticastMode(Camera_handle Camera, MULTICAST_MODE MulticastMode);
/* Camera_setMulticastMode */

//* Get multicast mode
//   *  Current multicast mode will be retrieved from the camera.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param MulticastMode current multicast mode will be returned
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_getMulticastMode(Camera_handle Camera,MULTICAST_MODE *MulticastMode);
/* Camera_getMulticastMode */

//* Get multicast group
//   *  Current multicast group (IP) and port will be retrieved from the camera.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param MulticastGroup current multicast group (IP) will be returned
//   *  @param MulticastPort current multicast port will be returned
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_getMulticastGroup(Camera_handle Camera, unsigned int *MulticastGroup, unsigned int *MulticastPort);
/* Camera_getMulticastGroup */
//------------------------------------------------------------------------------
// 8 - Stream: Image access
//------------------------------------------------------------------------------
//* Get image pointer.
//   *  A pointer to the image data will be returned
//   *
//   *  @param Image an image buffer that was received from the callback function
//   *  @param ImageData a pointer to the image data
//   *  @return success or error code

//SVGigE_RETURN	Image_getDataPointer(SVGigE_IMAGE Image, unsigned char **ImageData);
//TODO:unsigned char*	Image_getDataPointer(SVGigE_IMAGE Image);

//* Get buffer index.
//   *  The index of the current image buffer will be returned
//   *
//   *  @param Image an image buffer that was received from the callback function
//   *  @param BufferIndex a pointer to the image data
//   *  @return success or error code

//TODO deprecated: SVGigE_RETURN	Image_getBufferIndex(SVGigE_IMAGE Image, int *BufferIndex);

//* Get signal type
//   *  The signal type that is related to a callback will be returned.
//   *
//   *  HINT: If the camera cable is disconnected while a connection to that camera
//   *        is open, the image callback will return a NULL pointer. When the
//   *        Image_getSignalType() function is called in that situation it will
//   *        return a value: SVGigE_SIGNAL_CAMERA_CONNECTION_LOST. That signal
//   *        value may be used for taking further actions.
//   *
//   *  @param Image an image buffer that was received from the callback function
//   *  @param SignalType the signal type which informs why a callback was triggered
//   *  @return  success or error code

//TODO:SVGigE_RETURN	Image_getSignalType(SVGigE_IMAGE Image, SVGigE_SIGNAL_TYPE *SignalType);

//* Get camera handle
//   *  A handle of the camera that captured the image will be returned
//   *
//   *  @param Image an image buffer that was received from the callback function
//   *  @param a camera handle
//   *  @return success or error code

//TODO:Camera_handle	Image_getCamera(SVGigE_IMAGE Image);

//* Release image.
//   *  An image which availability was indicated by a StreamCallback function needs
//   *  to be released after processing it by a user application in order to free
//   *  the occupied buffer space for a subsequent image acquisition.
//   *
//   *  After releasing an image, none image access function must be called anymore
//   *  which would use the released image handle.
//   *
//   *  NOTE: The Image_release() function is called automatically when leaving the
//   *        callback function. Therefore, afterwards a buffer will stay available
//   *        with image content only as long as a buffer overrun occurs. A buffer
//   *        overrun can be prevented by using a sufficient number of buffers.
//   *
//   *  HINT: An application is responsible for providing a sufficient number of
//   *        buffers to the driver in order to prevent from buffer overrun. It is
//   *        recommended to assign e.g. at least 60 MByte total buffer space
//   *        (number of buffers times the size of a particular buffer). That amount
//   *        of buffer space would make sure that images are available also in worst
//   *        case for at least 500 ms because of a limited bandwidth on a GigE line
//   *        (< 120 MB/s).
//   *
//   *  @param Image an image buffer that was received from the callback function
//   *  @param Release s or error code //TODO
//   *  @return success or error code

//TODO:SVGigE_RETURN	Image_release(SVGigE_IMAGE Image, char **Release);
//------------------------------------------------------------------------------
// 9 - Stream: Image conversion
//------------------------------------------------------------------------------

/** Get image 12-bit as 8-bit.
   *  A 12-bit image will be converted into a 8-bit image. The caller needs to
   *  provide for a sufficient buffer for the 8-bit image.
   *
   *  @param ImageData a pointer to image data
   *  @param ImageWidth the image width
   *  @param ImageHeight the image height
   *  @param PixelType the pixel type
   *  @param Buffer8bit a buffer for 8-bit image data
   *  @param BufferLength the length of the image buffer
   *  @return success or error code
   */
SVGigE_RETURN
Image_getImage12bitAs8bit(unsigned char const * ImageData,
                          int ImageWidth,
                          int ImageHeight,
                          GVSP_PIXEL_TYPE PixelType,
                          unsigned char *Buffer8bit,
                          int BufferLength);

/** Get image 12-bit as 16-bit.
   *  A 12-bit image will be converted into a 16-bit image. The caller needs to
   *  provide for a sufficiently large buffer (2 x width x height bytes) for the
   *  16-bit image.
   *
   *  @param ImageData a pointer to image data
   *  @param ImageWidth the image width
   *  @param ImageHeight the image height
   *  @param PixelType the pixel type
   *  @param Buffer16bit a buffer for 16-bit image data
   *  @param BufferLength the length of the image buffer
   *  @return success or error code
   */
SVGigE_RETURN
Image_getImage12bitAs16bit(unsigned char *ImageData,
                           int ImageWidth,
                           int ImageHeight,
                           GVSP_PIXEL_TYPE PixelType,
                           unsigned char *Buffer16bit,
                           int BufferLength);

/** Get image 16-bit as 8-bit.
   *  A 16-bit image will be converted into a 8-bit image. The caller needs to
   *  provide for a sufficient buffer for the 8-bit image.
   *
   *  @param ImageData a pointer to image data
   *  @param ImageWidth the image width
   *  @param ImageHeight the image height
   *  @param PixelType the pixel type
   *  @param Buffer8bit a buffer for 8-bit image data
   *  @param BufferLength the length of the image buffer
   *  @return success or error code
   */
SVGigE_RETURN
Image_getImage16bitAs8bit(unsigned char const* ImageData, int ImageWidth,int ImageHeight,GVSP_PIXEL_TYPE PixelType,unsigned char *Buffer8bit,int BufferLength);

//------------------------------------------------------------------------------
// 10 - Stream: Image characteristics
//------------------------------------------------------------------------------

/** Get pixel type.
   *  The pixel type as indicated by the camera will be returned
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param PixelType the pixel type as indicated by the camera
   *  @return success or error code
   */
SVGigE_RETURN Image_getPixelType(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, GVSP_PIXEL_TYPE *PixelType);

/** Get image size.
   *  The number of bytes in the image data field will be returned.
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param ImageSize number of bytes in the image data field
   *  @return success or error code
   */
SVGigE_RETURN Image_getImageSize(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *ImageSize);

/** Get image pitch.
   *  The number of bytes in a row (pitch) will be returned as received from the camera
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param ImagePitch image's pitch as received from the camera
   *  @return success or error code
   */
SVGigE_RETURN Image_getPitch(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *ImagePitch);

/** Get image size X.
   *  The horizontal pixel number will be returned as received from the camera
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param SizeX image's size X as received from the camera
   *  @return success or error code
   */
SVGigE_RETURN Image_getSizeX(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *SizeX);

/** Get image size Y.
   *  The vertical pixel number will be returned as received from the camera
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param SizeY image's size Y as received from the camera
   *  @return success or error code
   */
SVGigE_RETURN Image_getSizeY(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *SizeY);


//------------------------------------------------------------------------------
// 11 - Stream: Image statistics
//------------------------------------------------------------------------------

/** Get image identifier.
   *
   *  An image identifier will be returned as it was assigned by the camera.
   *  Usually the camera will assign an increasing sequence of integer numbers
   *  to subsequent images which will wrap at some point and jump back to 1.
   *  The 0 identifier will not be used as it is defined in the GigE Vision
   *  specification
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image info that was received from the callback function
   *  @param ImageID an integer number that is unique inside a certain sequence of numbers
   *  @return success or error code
   */
SVGigE_RETURN Image_getImageId(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *ImageID);

/** Get image timestamp.
   *  The timestamp that was assigned to an image by the camera on image
   *  acquisition will be returned
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image info that was received from the callback function
   *  @param Timestamp as it was received from the camera in ticks elapsed after last camera reboot
   *  @return success or error code
   */
SVGigE_RETURN Image_getTimestamp(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *Timestamp);

///** Get image transfer time.
//   *  The time that elapsed from image's first network packet arriving on PC side
//   *  until image completion will be determined, including possible packet resends.
//   *
//   *  @param Stream a handle from a camera that has been opened before
//   *  @param Image an image buffer that was received from the callback function
//   *  @param TRANSFERTIME image's transfer time as it was explained above
//   *  @return success or error code
//   */

//SVGigE_RETURN Image_getTransferTime(Stream_handle Stream, SVGigE_IMAGE *ImageInfo , unsigned *TRANSFERTIME );
/* Image_getTransferTime */

/** Get packet count.
   *  The number of packets that have been sent will be reported
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param PacketCount the number of all sent packets per image
   *  @return success or error code
   */
SVGigE_RETURN Image_getPacketCount(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *PacketCount);


/** Get packet resend.
   *  The number of packets that have been resent will be reported
   *
   *  @param Stream a handle from a camera that has been opened before
   *  @param ImageInfo an image buffer that was received from the callback function
   *  @param PacketResend the number of resent packets per image
   *  @return success or error code
   */
SVGigE_RETURN Image_getPacketResend(Stream_handle Stream, SVGigE_IMAGE *ImageInfo, unsigned *PacketResend);

//------------------------------------------------------------------------------
// 12 - Stream: Messaging channel
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 13 - Controlling camera: Frame rate
//------------------------------------------------------------------------------

/** Set framerate.
   *  The camera will be adjusted to a new framerate
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Framerate new framerate in 1/s
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setFrameRate(Camera_handle Camera,
                    float Framerate);

/** Get framerate.
   *  The currently programmed framerate will be retrieved
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Framerate currently programmed frame rate in 1/s
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getFrameRate(Camera_handle Camera,
                    float *Framerate);

/** Get framerate min.
   *  The minimal available framerate will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinFramerate the minimal framerate in 1/s
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getFrameRateMin(Camera_handle Camera,float *MinFramerate);


/** Get frame rate max.
   *  The currently available maximal framerate will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxFramerate currently available maximal framerate in 1/s
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getFrameRateMax(Camera_handle Camera,float *MaxFramerate);

/** Get framerate increment.
   *  The framerate increment will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param FramerateIncrement the framerate increment in 1/s
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getFrameRateIncrement(Camera_handle Camera,float *FramerateIncrement);

//------------------------------------------------------------------------------
// 14 - Controlling camera: Exposure
//------------------------------------------------------------------------------

/** Set exposure time.
   *  The camera will be adjusted to a new exposure time
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureTime new exposure time in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setExposureTime(Camera_handle Camera,
                       float ExposureTime);

/** Get exposure time.
   *  The currently programmed exposure time will be retrieved
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureTime currently programmed exposure time in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getExposureTime(Camera_handle Camera,
                       float *ExposureTime);

/** Get exposure time min.
   *  The minimal setting for exposure time will be returned.
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinExposureTime the minimal exposure time setting in microseconds
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getExposureTimeMin(Camera_handle Camera,
                          float *MinExposureTime);

/** Get exposure time max.
   *  The currently available maximal exposure time will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxExposureTime currently available maximal exposure time in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getExposureTimeMax(Camera_handle Camera,
                          float *MaxExposureTime);

/** Get exposure time increment.
   *  The increment for exposure time will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureTimeIncrement the exposure time increment in microseconds
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getExposureTimeIncrement(Camera_handle Camera, float *ExposureTimeIncrement);

/** Set exposure delay.
   *  The camera's exposure delay in micro seconds relative to the trigger
   *  pulse will be set to the provided value. The delay will become active
   *  each time an active edge of an internal or external trigger pulse arrives
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureDelay the new value for exposure delay
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setExposureDelay(Camera_handle Camera,
                        float ExposureDelay);

/** Get exposure delay.
   *  The camera's current exposure delay will be returned in micro seconds
   *  relative to the trigger pulse
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureDelay the currently programmed value for exposure delay
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getExposureDelay(Camera_handle Camera,float *ExposureDelay);

/** Get exposure delay max.
   *  The camera's maximal exposure delay will be returned in micro seconds
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxExposureDelay the maximal value for exposure delay in microseconds
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getExposureDelayMax(Camera_handle Camera, float *MaxExposureDelay);

/** Get exposure delay increment.
   *  The camera's exposure delay increment will be returned in micro seconds
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ExposureDelayIncrement the exposure delay increment in microseconds
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getExposureDelayIncrement(Camera_handle Camera,float *ExposureDelayIncrement);

//------------------------------------------------------------------------------
// 15 - Controlling camera: Gain and offset
//------------------------------------------------------------------------------

/** Set gain.
   *  The camera will be adjusted to a new analog gain
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Gain new analog gain (0..18 dB)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setGain(Camera_handle Camera,
               float Gain);

/** Get gain.
   *  The currently programmed analog gain will be retrieved
   *  Note: A gain of 1.0 is represented as integer 128 in the appropriate camera
   *  register. Thus the gain can be adjusted only in discrete steps.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Gain currently programmed analog gain (0..18 dB)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getGain(Camera_handle Camera,
               float *Gain);

/** Get maximal gain.
   *  The currently available maximal analog gain will be retrieved
   *  Note: A gain of 1.0 is represented as integer 128 in the appropriate camera
   *  register. Thus the gain can be adjusted only in discrete steps.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxGain currently available maximal analog gain (0..18 dB)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getGainMax(Camera_handle Camera,
                  float *MaxGain);

/** Get gain increment.
   *  The analog gain increment will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param GainIncrement the analog gain increment
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getGainIncrement(Camera_handle Camera,float *GainIncrement);

/** Set offset.
   *  The offset value will be set to the provided value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Offset the new value for pixel offset (0..255)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setOffset(Camera_handle Camera,float Offset);

/** Get offset.
   *  The current offset value will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Offset the currently programmed value for pixel offset (0..255)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getOffset(Camera_handle Camera,float *Offset);

/** Get maximal offset.
   *  The maximal offset value will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxOffset the maximal value for pixel offset
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getOffsetMax(Camera_handle Camera,float *MaxOffset);

//------------------------------------------------------------------------------
// 16 - Controlling camera: Auto gain/exposure
//------------------------------------------------------------------------------

/** Set auto gain enabled.
   *  The auto gain status will be switched on or off.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param isAutoGainEnabled whether auto gain has to be enabled or disabled
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAutoGainEnabled(Camera_handle Camera,bool isAutoGainEnabled);

/** Get auto gain enabled.
   *  Current auto gain status will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param isAutoGainEnabled whether auto gain is enabled or disabled
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAutoGainEnabled(Camera_handle Camera, bool *isAutoGainEnabled);

/** Set auto gain brightness.
   *  The target brightness will be set that the camera tries to reach
   *  automatically when auto gain/exposure is enabled.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Brightness the target brightness for auto gain enabled
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAutoGainBrightness(Camera_handle Camera,
                             float Brightness);

/** Get auto gain brightness.
   *  The target brightness will be returned that the camera tries to reach
   *  automatically when auto gain/exposure is enabled.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Brightness currently programmed value for target brightness
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAutoGainBrightness(Camera_handle Camera,
                             float *Brightness);

/** Set auto gain dynamics.
   *  AutoGain PID regulator's time constants for the I (integration) and
   *  D (differentiation) components will be set to new values.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AutoGainParameterI the I parameter in a PID regulation loop
   *  @param AutoGainParameterD the D parameter in a PID regulation loop
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAutoGainDynamics(Camera_handle Camera,float AutoGainParameterI,float AutoGainParameterD);

/** Get auto gain dynamics.
   *  AutoGain PID regulator's time constants for the I (integration) and
   *  D (differentiation) components will be retrieved from the camera.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AutoGainParameterI the I parameter in a PID regulation loop
   *  @param AutoGainParameterD the D parameter in a PID regulation loop
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAutoGainDynamics(Camera_handle Camera,float *AutoGainParameterI,float *AutoGainParameterD);

/** Set auto gain limits.
   *  The minimal and maximal gain will be determined that the camera
   *  must not exceed in auto gain/exposure mode.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinGain the minimal allowed gain value
   *  @param MaxGain the maximal allowed gain value
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAutoGainLimits(Camera_handle Camera,
                         float MinGain,
                         float MaxGain);

/** Get auto gain limits.
   *  The minimal and maximal gain will be returned that the camera
   *  must not exceed in auto gain/exposure mode.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinGain the minimal allowed gain value
   *  @param MaxGain the maximal allowed gain value
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAutoGainLimits(Camera_handle Camera,
                         float *MinGain,
                         float *MaxGain);

/** Set auto exposure limits.
   *  The minimal and maximal exposure will be determined that the camera
   *  must not exceed in auto gain/exposure mode.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinExposure the minimal allowed exposure value
   *  @param MaxExposure the maximal allowed exposure value
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAutoExposureLimits(Camera_handle Camera,
                             float MinExposure,
                             float MaxExposure);

/** Set auto exposure limits.
   *  The minimal and maximal exposure will be determined that the camera
   *  must not exceed in auto gain/exposure mode.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinExposure the minimal allowed exposure value
   *  @param MaxExposure the maximal allowed exposure value
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAutoExposureLimits(Camera_handle Camera,
                             float *MinExposure,
                             float *MaxExposure);

//------------------------------------------------------------------------------
// 17 - Controlling camera: Acquisition trigger
//------------------------------------------------------------------------------

/** Set acquisition control.
   *  The camera's acquisition will be controlled (start/stop).
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AcquisitionControl the new setting for acquisition control
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAcquisitionControl(Camera_handle Camera,
                             ACQUISITION_CONTROL AcquisitionControl);

/** Get acquisition control.
   *  The camera's current acquisition control (start/stop) will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AcquisitionControl the currently programmed setting for acquisition control
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAcquisitionControl(Camera_handle Camera,
                             ACQUISITION_CONTROL *AcquisitionControl);

/** Set acquisition mode.
   *  The camera's acquisition mode will be set to the selected value.
   *  It can be determined whether camera control should switch to START immediately.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AcquisitionMode the new setting for acquisition mode
   *  @param AcquisitionStart whether camera control switches to START immediately
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAcquisitionMode(Camera_handle Camera,ACQUISITION_MODE AcquisitionMode,bool AcquisitionStart);

/** Get acquisition mode.
   *  The camera's current acquisition mode will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AcquisitionMode the currently programmed setting for acquisition mode
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAcquisitionMode(Camera_handle Camera, ACQUISITION_MODE *AcquisitionMode);

/** Start acquisition cycle.
   *  The camera will be triggered for starting a new acquisition cycle.
   *  A mandatory pre-requisition for successfully starting an acquisition
   *  cycle by software is to have the camera set to ACQUISITION_MODE_SOFTWARE_TIGGER
   *  before
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_softwareTrigger(Camera_handle Camera);

//* Software trigger ID. (defined but not yet available)
//   *  The camera will be triggered for starting a new acquisition cycle.
//   *  A mandatory pre-requisition for a successful software trigger is to have
//   *  the camera set to ACQUISITION_MODE_SOFTWARE_TIGGER before.
//   *  In addition to a usual software trigger, an ID will be accepted that
//   *  can be written into the image on demand, e.g. for maintaining a unique
//   *  trigger/image reference
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param TriggerID an ID to be transferred into first bytes of resulting image data
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_softwareTriggerID(Camera_handle Camera,TRIGGER_POLARITY TriggerID);
/* Camera_softwareTriggerID */

//* Software trigger ID enable. (defined but not yet available)
//   *  The "software trigger ID" mode will be enabled respectively disabled
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param TriggerIDEnable whether "trigger ID" will be enabled or disabled
//   *  @return success or error code

//TODO:SVGigE_RETURN
//Camera_softwareTriggerIDEnable(Camera_handle Camera, bool TriggerIDEnable);
/* Camera_softwareTriggerIDEnable */

/** Set trigger polarity.
   *  The camera's trigger polarity will be set to the selected value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TriggerPolarity the new setting for trigger polarity
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setTriggerPolarity(Camera_handle Camera, TRIGGER_POLARITY TriggerPolarity);

/** Get trigger polarity.
   *  The camera's current trigger polarity will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TriggerPolarity the currently programmed setting for trigger polarity
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getTriggerPolarity(Camera_handle Camera,TRIGGER_POLARITY *TriggerPolarity);

/** Set PIV mode.
   *  The camera's PIV mode will be enabled or disabled.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PivMode the new setting for PivMode
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setPivMode(Camera_handle Camera, PIV_MODE PivMode);

/** Get PIV mode
   *  Check if camera's PIV mode is enabled or disabled.
   *  The state of camera's current Piv mode will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PivMode the currently programmed setting for PivMode
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getPivMode(Camera_handle Camera,unsigned int *PivMode);

/** set debouncer duration.
   *  The camera's debouncer duration will be set to the selected value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param DebouncerDuration the new setting for debouncer duration
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setDebouncerDuration(Camera_handle Camera, float DebouncerDuration);

/** Get debouncer duration.
   *  The camera's debouncer duration will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Duration the currently programmed setting for debouncer duration
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getDebouncerDuration(Camera_handle Camera, float *Duration);


/** load Sequence parameters
 *  New sequence parameters will be loaded from a XML file into the camera
 *
 *  @see CameraContainer_getCamera()
 *  @param hCamera a camera handle received from CameraContainer_getCamera()
 *  @param Filename a complete path and file name where to load the settings from
 *  @return success or error code
 */
 SVGigE_RETURN
Camera_loadSequenceParameters(Camera_handle Camera,
                              char *Filename);

/** Start sequencer
   * Start acquisition using sequencer.
   * This will occur after loading the appropriate sequence parameters.
   *
   *  @param  Camera a handle from a camera that has been opened before
   *  @return success or error code
   */
SVGigE_RETURN Camera_startSequencer(Camera_handle Camera);

//------------------------------------------------------------------------------
// 18 - Controlling camera: Strobe
//------------------------------------------------------------------------------

/** Set strobe polarity.
   *  The camera's strobe polarity will be set to the selected value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePolarity the new setting for strobe polarity
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setStrobePolarity(Camera_handle Camera,
                         STROBE_POLARITY StrobePolarity);

/** Get strobe polarity.
   *  The camera's current strobe polarity will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePolarity the currently programmed setting for strobe polarity
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobePolarity(Camera_handle Camera,
                         STROBE_POLARITY *StrobePolarity);

/** Set strobe position.
   *  The camera's strobe position in micro seconds relative to the trigger
   *  pulse will be set to the selected value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePosition the new value for strobe position in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setStrobePosition(Camera_handle Camera,
                         float StrobePosition);

/** Get strobe position.
   *  The camera's current strobe position will be returned in micro seconds
   *  relative to the trigger pulse
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePosition the currently programmed value for strobe position in microseconds
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getStrobePosition(Camera_handle Camera,
                         float *StrobePosition);


/** Get maximal strobe position.
   *  The camera's maximal strobe position (delay) will be returned in micro seconds
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePositionMax the maximal value for strobe position in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobePositionMax(Camera_handle Camera,
                            float *StrobePositionMax);

/** Get strobe position increment.
   *  The camera's strobe position increment will be returned in micro seconds
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobePositionIncrement the strobe position increment in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobePositionIncrement(Camera_handle Camera,
                                  float *StrobePositionIncrement);

/** Set strobe duration.
   *  The camera's strobe duration in micro seconds will be set to the selected
   *  value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobeDuration the new value for strobe duration in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setStrobeDuration(Camera_handle Camera,
                         float StrobeDuration);

/** Get strobe duration.
   *  The camera's current strobe duration in micro seconds will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobeDuration the currently programmed value for strobe duration in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobeDuration(Camera_handle Camera,
                         float *StrobeDuration);

/** Get maximal strobe duration
   *  The camera's maximal strobe duration in micro seconds will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxStrobeDuration the maximal value for strobe duration in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobeDurationMax(Camera_handle Camera,
                            float *MaxStrobeDuration);

/** Get strobe duration increment
   *  The camera's strobe duration increment in micro seconds will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param StrobeDurationIncrement the strobe duration increment in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getStrobeDurationIncrement(Camera_handle Camera,
                                  float *StrobeDurationIncrement);

//------------------------------------------------------------------------------
// 19 - Controlling camera: Tap balance
//------------------------------------------------------------------------------

///** Set tap configuration
//   *  The camera will be controlled for working with one or two taps
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param TapCount the number of taps (1, 2) to be used by the camera
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setTapConfiguration(Camera_handle Camera, int TapCount);
/* Camera_setTapConfiguration */

///** Get tap configuration
//   *  The camera will be queried whether it is working with one or two taps
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param TapCount the number of taps (1, 2) currently used by the camera is returned
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getTapConfiguration(Camera_handle Camera, int *TapCount);
/* Camera_getTapConfiguration */



/**
 *	Set tap Configuration extended
 *  The camera will be controlled for working with one of the following tap configurations:
 *  SVGIGE_SELECT_SINGLE_TAP
 *	SVGIGE_SELECT_DUAL_TAP_H
 *	SVGIGE_SELECT_DUAL_TAP_V
 *	SVGIGE_SELECT_QUAD
 *
 *  @see CameraContainer_getCamera()
 *  @param hCamera a camera handle received from CameraContainer_getCamera()
 *  @param SelectedTapConfig the selected tap configuration to be used by the camera
 *  @return success or error code
 */
SVGigE_RETURN Camera_setTapConfigurationEx(Camera_handle Camera,
                                           SVGIGE_TAP_CONFIGURATION_SELECT SelectedTapConfig);

/**
 *  getTapConfiguration
 *  The camera will be queried whether it is working with one of the following configurations:
 *  SVGIGE_SELECT_SINGLE_TAP
 *	SVGIGE_SELECT_DUAL_TAP_H
 *	SVGIGE_SELECT_DUAL_TAP_V
 *	SVGIGE_SELECT_QUAD
 *
 *  @see CameraContainer_getCamera()
 *  @param hCamera a camera handle received from CameraContainer_getCamera()
 *  @param TapConfig the tap configuration currently used by the camera is returned
 *  @return success or error code
 */
SVGigE_RETURN Camera_getTapConfigurationEx(Camera_handle Camera,
                                           SVGIGE_TAP_CONFIGURATION_SELECT *TapConfig);


/** Set auto tap balance mode.
   *  One of the modes (OFF,ONCE,CONTINUOUS,RESET) will be applied for an auto
   *  tap balance procedure.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AutoTapBalanceMode the mode for auto tap balancing to be activated
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setAutoTapBalanceMode(Camera_handle Camera,
                             SVGIGE_AUTO_TAP_BALANCE_MODE AutoTapBalanceMode);

/** Get auto tap balance mode.
   *  Currently adjusted auto tap balance mode will be returned.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param AutoTapBalanceMode currently programmed value for auto tap balancing mode
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getAutoTapBalanceMode(Camera_handle Camera,
                             SVGIGE_AUTO_TAP_BALANCE_MODE *AutoTapBalanceMode);

/** Set tap balance.
   *  A provided tap balance in [dB] will be transferred to camera.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TapBalance the new value for tap balance to be activated
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setTapBalance(Camera_handle Camera,float TapBalance);


/** Get tap balance.
   *  Currently adjusted tap balance in [dB] will be retrieved from camera.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TapBalance currently programmed value for tap balance
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getTapBalance(Camera_handle Camera, float *TapBalance);

  /** Get tap gain.
   *  Currently adjusted tap gain in [dB] will be queried from camera.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TapGain the new value for tap gain 
   *  @param TapSelect one of the defined tap selectors 
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getTapGain(Camera_handle Camera, float *TapGain, SVGIGE_TAP_SELECT TapSelect);


/** Set Tap Gain.
   *  A provided tap gain in [dB] will be transferred to camera.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Tap Gain the new setting for Tap Gain
   *  @return success or error code
   */
SVGigE_RETURN
Camera_setTapGain(Camera_handle Camera, float TapGain, SVGIGE_TAP_SELECT TapSelect );


//* Set tap calibration
//   *  2009-03-10: DEPRECATED, please use Camera_setTapBalance()
//   *
//   *  The provided tab calibration values will be written to camera. The calibration
//   *  values apply to a particular tap. Thus adjusting all taps requires running this
//   *  function multiple, once for each tap.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param TapID an identification number (0, 1) of the tap
//   *  @param Gain the gain value for the ADC as integer
//   *  @param Offset the offset value for the ADC as integer
//   *  @return success or error code

/*
SVGigE_RETURN
    Camera_setTapCalibration(Camera_handle Camera,unsigned int TapID,unsigned int Gain, unsigned int Offset); */
/* Camera_setTapCalibration */

//------------------------------------------------------------------------------
// 20 - Controlling camera: Image parameter
//------------------------------------------------------------------------------

/** Get imager width.
   *  The imager width will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ImagerWidth a reference to the imager width value
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getImagerWidth(Camera_handle Camera,
                      unsigned int *ImagerWidth);

/** Get imager height.
   *  The imager height will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ImagerHeight a reference to the imager height value
   *  @return success or error code
   */
SVGigE_RETURN
Camera_getImagerHeight(Camera_handle Camera,
                       unsigned int *ImagerHeight);


/** Get camera buffersize.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param BufferSize
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getBufferSize(Camera_handle Camera, unsigned int *BufferSize);


/** Get image pitch.
   *  The image pitch will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ImagePitch a reference to the image pitch value
   *  @return success or error code
   */

SVGigE_RETURN
Camera_getPitch(Camera_handle Camera, unsigned int *ImagePitch);


/** Get size X.
   *  The currently used horizontal picture size X will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SizeX currently programmed value for size X
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getSizeX(Camera_handle Camera,
                unsigned int *SizeX);

/** Get size Y.
   *  The currently used vertical picture size Y will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SizeY currently programmed value for size Y
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getSizeY(Camera_handle Camera, unsigned int *SizeY);

/** Set binning mode.
   *  The camera's binning mode will be set to the selected value
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param BinningMode the new setting for binning mode
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setBinningMode(Camera_handle Camera,
                      BINNING_MODE BinningMode);

/** Get binning mode.
   *  The camera's current binning mode will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param BinningMode the currently programmed setting for binning mode
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getBinningMode(Camera_handle Camera,
                      BINNING_MODE *BinningMode);

/** Set area of interest (AOI)
   *  The camera will be switched to partial scan mode and an AOI will be set
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SizeX the number of pixels in one row
   *  @param SizeY the number of scan lines
   *  @param OffsetX a left side offset of the scanned area
   *  @param OffsetY an upper side offset of the scanned area
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setAreaOfInterest(Camera_handle Camera,
                         unsigned int SizeX,
                         unsigned int SizeY,
                         unsigned int OffsetX,
                         unsigned int OffsetY);

/** Get area of interest (AOI)
   *  The currently set parameters for partial scan will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SizeX the number of pixels in one row
   *  @param SizeY the number of scan lines
   *  @param OffsetX a left side offset of the scanned area
   *  @param OffsetY an upper side offset of the scanned area
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAreaOfInterest(Camera_handle Camera,
                         unsigned int *SizeX,
                         unsigned int *SizeY,
                         unsigned int *OffsetX,
                         unsigned int *OffsetY);


/** Get area of interest (AOIR)
   *  The currently set parameters for total scan  will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MinSizeX the minimal value available for AOI width
   *  @param MinSizeY the minimal value available for AOI height
   *  @param MaxSizeX the maximal value available for AOI width
   *  @param MaxSizeY the maximal value available for AOI height
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAreaOfInterestRange(Camera_handle Camera,
                              unsigned int *MinSizeX,
                              unsigned int *MinSizeY,
                              unsigned int *MaxSizeX,
                              unsigned int *MaxSizeY);


/** Get area of interest (AOI) increment
   *  The currently set parameters for partial scan will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param SizeXIncrement the minimal value available for AOI width
   *  @param SizeYIncrement the minimal value available for AOI height
   *  @param OffsetXIncrement the maximal value available for AOI width
   *  @param OffsetYIncrement the maximal value available for AOI height
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getAreaOfInterestIncrement(Camera_handle Camera,
                                  unsigned int *SizeXIncrement,
                                  unsigned int *SizeYIncrement,
                                  unsigned int *OffsetXIncrement,
                                  unsigned int *OffsetYIncrement);


/** Reset timestamp counter
   *  The camera's timestamp counter will be set to zero
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_resetTimestampCounter(Camera_handle Camera);


/** Get timestamp counter
   *  Current value of the camera's timestamp counter will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TimeStampCounter the current value of the timestamp counter
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getTimestampCounter(Camera_handle Camera, uint64_t *TimeStampCounter);


/** Get timestamp tick frequency
   *  Current value of the camera's timestamp tick frequency counter will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param TimestampTickFrequency the camera's timestamp tick frequency
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getTimestampTickFrequency(Camera_handle Camera, uint64_t *TimestampTickFrequency);


  /** Set Flipping mode.
   *   The camera will be controlled for working with the following flipping mode if selected:
   *   REVERSE_OFF (without flipping)
   *   REVERSE_X (vertical flipping)
   *   REVERSE_Y (horizontal flipping)
   *   REVERSE_X_Y( horizontal and vertical flipping) 
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param FlippingMode the new setting for flipping mode
   *  @return success or error code
   */
SVGigE_RETURN Camera_setFlippingMode(Camera_handle Camera, SVGIGE_FLIPPING_MODE FlippingMode);

  /** Get Flipping mode.
   *  The camera will be queried whether it is working with one of the following flipping mode:
   *   REVERSE_OFF (without flipping)
   *   REVERSE_X (vertical flipping)
   *   REVERSE_Y (horizontal flipping)
   *   REVERSE_X_Y( horizontal and vertical flipping)
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param FlippingMode the currently programmed setting for flipping mode
   *  @return success or error code
   */
SVGigE_RETURN Camera_getFlippingMode(Camera_handle Camera, SVGIGE_FLIPPING_MODE *FlippingMode);

  /** Set Shutter mode.
   *  The camera will be controlled for working with the following shutter mode if selected:
   *  GLOBAL_SHUTTER 
   *  ROLLING_SHUTTER
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ShutterMode the new setting for shutter mode
   *  @return success or error code
   */
SVGigE_RETURN Camera_setShutterMode(Camera_handle Camera, SVGIGE_SHUTTER_MODE ShutterMode);

  /** Get Shutter mode.
   *  The camera will be queried whether it is working with one of the following shutter mode:
   *  GLOBAL_SHUTTER 
   *  ROLLING_SHUTTER
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param ShutterMode the currently programmed setting for shutter mode
   *  @return success or error code
   */
SVGigE_RETURN Camera_getShutterMode(Camera_handle Camera, SVGIGE_SHUTTER_MODE *ShutterMode);




//------------------------------------------------------------------------------
// 21 - Controlling camera: Image appearance
//------------------------------------------------------------------------------

/** Get pixel type.
   *  The pixel type will be retrieved from the camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PixelType currently programmed value for PixelType
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getPixelType(Camera_handle Camera,
                    GVSP_PIXEL_TYPE *PixelType);

/** Set pixel depth.
   *  The number of bits for a pixel will be set to 8, 12 or 16 bits. Before this function
   *  is called the camera's feature vector should be queried whether the desired pixel depth
   *  is supported
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PixelDepth the new value for pixel depth
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setPixelDepth(Camera_handle Camera,
                     SVGIGE_PIXEL_DEPTH PixelDepth);

/** Get pixel depth.
   *  The camera's current setting for pixel depth will be queried.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param PixelDepth currently programmed value for PixelDepth
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getPixelDepth(Camera_handle Camera,
                     SVGIGE_PIXEL_DEPTH *PixelDepth);

/** setWhiteBalance.
   *  The provided values will be applied for white balance.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Red balanced value for red color
   *  @param Green balanced value for green color
   *  @param Blue balanced value for blue color
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setWhiteBalance(Camera_handle Camera,
                       float Red,
                       float Green ,
                       float Blue);


///** getWhiteBalance.
//   *  Currently set values for white balance will be returned.
//   *  Previously adjusted values will be returned either unchanged or adjusted
//   *  if necessary. The returned values will be 100 and above where the color
//   *  which got 100 assigned will be transferred unchanged, however two
//   *  other color components might be enhanced above 100 for each image.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param Red balanced value for red color
//   *  @param Green balanced value for green color
//   *  @param Blue balanced value for blue color
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_getWhiteBalance(Camera_handle Camera,
//                       float *Red,
//                       float *Green ,
//                       float *Blue);


///** getWhiteBalanceMax.
//   *  The maximal white-balance value for amplifying colors will be returned.
//   *  A value of 1.0 is the reference for a balanced situation.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param WhiteBalanceMax the maximal white-balance (e.g. 4.0 or 2.0)
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getWhiteBalanceMax(Camera_handle Camera,float *WhiteBalanceMax);


///** setLookupTableMode.
//   *  The look-up table mode will be switched on or off
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LUTMode whether LUT will be enabled or disabled
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_setLookupTableMode(Camera_handle Camera,
//                          LUT_MODE LUTMode);

///** Get look-up table mode.
//   *  The currently programmed look-up table mode will be retrieved
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LUTMode a pointer to a LUT_MODE value
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_getLookupTableMode(Camera_handle Camera,
//                          LUT_MODE *LUTMode);

///** setLookupTable.
//   *  A user-defined lookup table will be uploaded to the camera. The size has to match
//   *  the lookup table size that is supported by the camera (1024 for 10to8 or 4096 for 12to8).
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LookupTable an array of LUT values
//   *  @param LookupTableSize the size of the LUT array
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_setLookupTable(Camera_handle Camera,
//                      unsigned char *LookupTable,
//                      int LookupTableSize);

///** getLookupTable.
//   *  The currently installed lookup table will be downloaded from the camera. The size of the
//   *  reserved download space has to match the lookup table size (1024 for 10to8 or 4096 for 12to8).
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LookupTable an array for LUT values
//   *  @param LookupTableSize the size of the LUT array
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_getLookupTable(Camera_handle Camera,
//                      unsigned char *LookupTable,
//                      int LookupTableSize);



//------------------------------------------------------------------------------
// 22 - Special control: IOMux configuration
//------------------------------------------------------------------------------

/** Get Max IOMuxIN
   *  the maximal number of available IN signals (signal sources)
   
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxIOMuxINSignals the currently supported number of IN signals(signal sources)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getMaxIOMuxIN(Camera_handle Camera,int *MaxIOMuxINSignals);

/** Get Max IOMuxOUT
   *  the maximal number of available OUT signals (signal sinks) 
   
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxIOMuxOUTSignals the currently supported number of OUT signals(signal sinks)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getMaxIOMuxOUT(Camera_handle Camera,int *MaxIOMuxOUTSignals);

///** Set IOAssignment
//   *  An OUT signal (signal sink) will get one or multiple IN signal
//   *  (signal sources) assigned in a camera's multiplexer. In case of multiple
//   *  signal sources (IN signals) those signals will be or'd for combinig them to
//   *  one 32-bit value that will subsequently be assigned to an OUT signal
//   *  @param Camera a handle from a camera that has been opened before
//   *
//   *  @param IOMuxOUT the multiplexer's OUT signal (signal sink) to be configured
//   *  @param SignalIOMuxIN the IN signal vector (signal sources) to be activated
//   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
//   */
//SVGigE_RETURN
//Camera_setIOAssignment(Camera_handle Camera, SVGigE_IOMux_OUT IOMuxOUT, unsigned int SignalIOMuxIN);

///** getIOAssignment.
//   *  Current assignment of IN signals (signal sources) to an OUT signal
//   *  (signal sink) will be retrieved from a camera's multiplexer.
//   *
//   *  @see Camera_isCameraFeature()
//   *  @param Camera a handle from a camera that has been opened before
//   *
//   *  @param IOMuxOUT the multiplexer's OUT signal (signal sink) to be queried
//   *  @param IOMuxIN the IN signal vector (signal sources) connected to the OUT signal
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getIOAssignment(Camera_handle Camera,
//                       SVGigE_IOMux_OUT IOMuxOUT,
//                       unsigned int *IOMuxIN);



/** Get IOInputStatus
   *  Get the status of the current Source(signal source)
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param Source the multiplexer's IN signal (signal source)
   *  @param Status the boolean value which indicate the current Source's status
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getIOInputStatus(Camera_handle Camera, SVGigE_IOMux_IN Source, bool * Status);


/** Get IOOutputStatus
   *  Get the status of the current Sink(signal sink)
   * 
   *  @param Camera a handle from a camera that has been opened before
   *  @param Sink the multiplexer's OUT signal (signal sink)
   *  @param Status the boolean value which indicate the current Sink's status
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getIOOutputStatus(Camera_handle Camera, SVGigE_IOMux_OUT Sink, bool * Status);


/** Set IOConfig
   *  Set the boolean logic which will be attached to Sink (signal sink), to indicate the logic relation 
   *  between signal source and signal sink for the current Sink
   * 
   *  @param Camera a handle from a camera that has been opened before
   *  @param Sink the multiplexer's OUT signal (signal sink)
   *  @param IOMode the boolean logic element which will be applied to Sink
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setIOConfig(Camera_handle Camera, SVGigE_IOMux_OUT Sink, SVGigE_IO_MODE IOMode);


/** Get IOConfig
   *  Get the boolean logic which will be attached to Sink (signal sink), to indicate the logic relation 
   *  between signal source and signal sink for the current Sink
   *   
   *  @param Camera a handle from a camera that has been opened before
   *  @param Sink the multiplexer's OUT signal (signal sink) to be configured
   *  @param IOMode the boolean logic element which is applied to Sink, to indicate the logic relation 
   *         between signal source and signal sink
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getIOConfig(Camera_handle Camera, SVGigE_IOMux_OUT Sink, SVGigE_IO_MODE *IOMode);

/** Set IOAssignment
   *  An OUT signal (signal sink) will get one IN signal
   *  (signal source) assigned in a camera's multiplexer. 

   *  @param Camera a handle from a camera that has been opened before
   *  @param Sink the multiplexer's OUT signal (signal sink) to be configured
   *  @param Source the IN signal (signal source) to be activated
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setIOAssignmentExt(Camera_handle Camera, SVGigE_IOMux_OUT Sink, SVGigE_IOMux_IN Source);

//------------------------------------------------------------------------------
// 23 - Special control: IO control
//------------------------------------------------------------------------------

///** setIOMuxIN.
//   *  The complete vector of IN signals (source signals, max 32 bits) will be
//   *  set in a camera's multiplexer in one go.
//   *
//   *  @see Camera_isCameraFeature()
//   *  @param Camera a handle from a camera that has been opened before
//   *
//   *  @param VectorIOMuxIN the IN signal vector's new state to be assigned
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setIOMuxIN(Camera_handle Camera, unsigned int VectorIOMuxIN);

///** getIOMuxIN.
//   *  The complete vector of IN signals (source signals, max 32 bits) will be
//   *  read out from a camera's multiplexer in one go.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param VectorIOMuxIN the IN signal vector's current state
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getIOMuxIN(Camera_handle Camera, unsigned int *VectorIOMuxIN);

///** setIO.
//   *  A single IN signal (source signal, one out of max 32 bits) will be set.
//   *  @param Camera a handle from a camera that has been opened before
//   *
//   *  @param SignalIOMuxIN a particular signal from the IN signal vector
//   *  @param SignalValue the signal value to be assigned to the IN signal
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setIO(Camera_handle Camera,SVGigE_IOMux_IN SignalIOMuxIN,IO_SIGNAL SignalValue);

///** getIO.
//   *  A single IN signal (source signal, one out of max 32 bits) will be read.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param SignalIOMuxIN a particular signal from the IN signal vector
//   *  @param SignalValue the current value of the selected IN signal
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getIO(Camera_handle Camera,SVGigE_IOMux_IN SignalIOMuxIN,IO_SIGNAL *SignalValue);

///** setAcqLEDOverride.
//   *  Override default LED mode by an alternative behavior:
//   *  - blue:    waiting for trigger
//   *  - cyan:    exposure
//   *  - magenta: read-out
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param isOverrideActive whether LED override will be activated or deactivated
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setAcqLEDOverride(Camera_handle Camera, bool isOverrideActive);

///** getAcqLEDOverride.
//   *  Check whether default LED mode was overridden by an alternative behaviour:
//   *  - blue:    waiting for trigger
//   *  - cyan:    exposure
//   *  - magenta: read-out
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param isOverrideActive a flag indicating whether LED override is currently activated
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getAcqLEDOverride(Camera_handle Camera, bool *isOverrideActive);

///** setLEDIntensity.
//   *  The LED intensity will be controlled in the range 0..255 as follows:
//   *  0   - dark
//   *  255 - light
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LEDIntensity the new intensity (0..255=max) to be assigned to the LED
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setLEDIntensity(Camera_handle Camera, int LEDIntensity);

///** getLEDIntensity.
//   *  The LED intensity will be retrieved from camera with the following meaning:
//   *  0   - dark
//   *  255 - light
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param LEDIntensity currently assigned LED intensity
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getLEDIntensity(Camera_handle Camera, int *LEDIntensity);

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 24 - Special control: Serial communication
//------------------------------------------------------------------------------

///** setUARTBuffer.
//   *  A block of data (max 512 bytes) will be sent to the camera's UART for
//   *  transmitting it over the serial line to a receiver.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param Data a pointer to a block of data to be sent over the camera's UART
//   *  @param the length of the data block (1..512)
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setUARTBuffer(Camera_handle Camera, unsigned char *Data, int DataLength);

///** getUARTBuffer.
//   *  A block of data will be retrieved which has arrived in the camera's UART
//   *  receiver buffer. If this function returns the maximal possible byte count
//   *  of the serial buffer (=512 bytes) then there might be more data available
//   *  which should be retrieved by a subsequent call to this function.
//   *
//   *  NOTE: If DataLengthMax is set to less than the serial buffer size (512 bytes)
//   *        and if DataLengthMax is not sufficient for returning all buffered data
//   *        then data loss will occur.
//   *
//   *  HINT: If more data will be transferred than the serial buffer size (512 bytes),
//   *        then DataLengthMax has to match that serial buffer size. Only then a
//   *        seamless transfer of data chunks can be performed, each chunk being the
//   *        size of the serial buffer (512 bytes).
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param Data a pointer to a data buffer
//   *  @param DataLengthReceived a pointer to a value for returning actual data read
//   *  @param DataLengthMax the maximal data length to be read (1..512)
//   *  @param Timeout a time period [s] after which the function returns if no data was received
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getUARTBuffer(Camera_handle Camera, unsigned char *Data, int *DataLengthReceived, int DataLengthMax,
//                     float Timeout);

///** setUARTBaud.
//   *  The baud rate of the camera's UART will be set to one out of a set of
//   *  pre-defined baud rates. Alternatively, any baud rate can be provided
//   *  as integer which would not have to comply with any pre-defined value.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param BaudRate the baud rate to be assigned to the UART
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_setUARTBaud(Camera_handle Camera, SVGigE_BaudRate BaudRate);

///** getUARTBaud.
//   *  The currently set baud rate in the camera's UART will be returned.
//   *
//   *  @param Camera a handle from a camera that has been opened before
//   *  @param BaudRate the UART's currently assigned baud rate
//   *  @return success or error code
//   */
//SVGigE_RETURN
//Camera_getUARTBaud(Camera_handle Camera, SVGigE_BaudRate *BaudRate);


//------------------------------------------------------------------------------
// 25 - Special control: Direct register and memory access
//------------------------------------------------------------------------------

/** Set GigE camera register.
   *  A register of a SVS GigE camera will be directly written to
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param RegisterAddress the address of a camera register
   *  @param RegisterValue the value to be written into specified register
   *  @param GigECameraAccessKey an access key (please ask SVS VISTEK)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_setGigECameraRegister(Camera_handle Camera,
                             unsigned int RegisterAddress,
                             unsigned int RegisterValue,
                             unsigned int GigECameraAccessKey);

/** Get GigE camera register.
   *  A value from a SVS GigE camera register will be directly read out
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param RegisterAddress the address of a camera register
   *  @param RegisterValue currently programmed register value
   *  @param GigECameraAccessKey an access key (please ask SVS VISTEK)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getGigECameraRegister(Camera_handle Camera,
                             unsigned int RegisterAddress,
                             unsigned int *RegisterValue,
                             unsigned int GigECameraAccessKey);

/** Write GigE camera memory.
   *  A block of data will be written to the memory of a SVS GigE camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MemoryAddress the start address of a memory block
   *  @param DataBlock a pointer to the data block to be written into camera
   *  @param DataLength the length of the data block
   *  @param GigECameraAccessKey an access key (please ask SVS VISTEK)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_writeGigECameraMemory(Camera_handle Camera,
                             unsigned int  MemoryAddress,
                             unsigned char *DataBlock,
                             unsigned int  DataLength,
                             unsigned int  GigECameraAccessKey);

/** Read GigE camera memory.
   *  A block of data will be read from the memory of a SVS GigE camera
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MemoryAddress the start address of a memory block
   *  @param DataBlock a pointer to a data block where camera data will be written to
   *  @param DataLength the length of the data block
   *  @param GigECameraAccessKey an access key (please ask SVS VISTEK)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_readGigECameraMemory(Camera_handle Camera,
                            unsigned int  MemoryAddress,
                            unsigned char *DataBlock,
                            unsigned int  DataLength,
                            unsigned int  GigECameraAccessKey);

//------------------------------------------------------------------------------
// 26 - Special control: Persistent settings and recovery
//------------------------------------------------------------------------------
/** Write EEPROM defaults.
   *  The current settings will be made the EEPROM defaults that will be
   *  restored on each camera start-up
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return success or error code
   */
SVGigE_RETURN
Camera_writeEEPROM(Camera_handle Camera);


/** Read EEPROM defaults.
   *  The EEPROM content will be moved to the appropriate camera registers.
   *  This operation will restore the camera settings that were active when
   *  the EEPROM write function was performed
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return success or error code
   */
SVGigE_RETURN
Camera_readEEPROM(Camera_handle Camera);


/** Restore factory defaults.
   *  The camera's registers will be restored to the factory defaults and at
   *  the same time those settings will be written as default to EEPROM
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return success or error code
   */
SVGigE_RETURN
Camera_restoreFactoryDefaults(Camera_handle Camera);


//------------------------------------------------------------------------------
// 27 - General functions
//------------------------------------------------------------------------------

/** Estimate white balance.
   *  Current image will be investigated for a suitable white balance setting
   *
   *  @param BufferRGB a buffer with current RGB image
   *  @param BufferLength the length of the RGB buffer
   *  @param Red new value for red color
   *  @param Green new value for green color
   *  @param Blue new value for blue color
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
SVGigE_estimateWhiteBalance(unsigned char *BufferRGB,
                            int BufferLength,
                            float *Red,
                            float *Green ,
                            float *Blue);

//------------------------------------------------------------------------------
// 28 - Diagnostics
//------------------------------------------------------------------------------

/** Return the SDK's version information, which is defined at compile time.
   *
   *  @param Version Information about the SDK's version
   *  @return success or error code
   */
SVGigE_RETURN
SVGigE_getVersionInfo(SVGigE_VERSION *Version);


/** getErrorMessage.
   *
   *  @param ReturnCode a return code that has to be converted into a string
   *  @return a string that explains the provided ReturnCode
   */
const char *
getErrorMessage(SVGigE_RETURN ReturnCode);

//------------------------------------------------------------------------------
// 29 - Special control: Lens control
//------------------------------------------------------------------------------

/** Is Lens available.
   *The camera will be queried whether a MFT Lens is available or not.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param isAvailable a flag indicating whether Objective is available or not
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_isLensAvailable(Camera_handle Camera, bool *isAvailable);



/** Get lens name.
   *  The lens name that is obtained from the lens firmware will be returned
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @return The lens name that is obtained from the lens firmware will be returned
   */
char* Camera_getLensName(Camera_handle Camera);


 /** setLensFocalLenght.
   *
   *  A provided focal length  will be transferred to lens.

   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocalLenght the focal length to be assigned to the Lens (granularity: 1/10 mm, ex. 350 corresponds to 35 mm).
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setLensFocalLength(Camera_handle Camera, unsigned int FocalLength);


/** getLensFocalLenght.
   *  The currently set focal length  of the lens will be returned.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocalLenght the currently assigned focal length (granularity: 1/10 mm, ex. 350 corresponds to 35 mm).
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocalLength(Camera_handle Camera, unsigned int *FocalLength);

/** getLensFocalLenghtMin.
   *  Get the minimal focal length that can be used.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocalLenghtMin the minimal focal length setting(granularity: 1/10 mm, ex. 140 corresponds to 14 mm).
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocalLengthMin(Camera_handle Camera, unsigned int *FocalLengthMin);

  /** getLensFocalLenghtMax.
   *  Get the maximal focal length that can be used.

   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocalLenghtMax the maximal focal length setting (granularity: 1/10 mm, ex. 420 corresponds to 42 mm).
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocalLengthMax(Camera_handle Camera, unsigned int *FocalLengthMax);


  /** set focus unit
   *  A selected focus unit will be transferred to lens.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Selected_unit the focus unit( mm or 1/10 mm) to be assigned to Lens
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setLensFocusUnit(Camera_handle Camera, FOCUS_UNIT Selected_unit );


  /** get focus unit
   *  The currently focus unit will be returned.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Selected_unit the currently used focus unit.
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocusUnit(Camera_handle Camera, FOCUS_UNIT *Selected_unit );


/** setLensFocus.
   *  A provided focus will be transferred to lens.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Focus the focus in the selected unit to be assigned to lens (default unit is mm)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setLensFocus(Camera_handle Camera, unsigned int Focus );


/** getLensFocus.
   *  The currently set focus of the lens will be returned.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Focus the currently assigned focus in the selected unit(default unit is mm)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocus(Camera_handle Camera, unsigned int *Focus);

/** getLensFocusMin.
   *  Get the minimal focus that can be used.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocusMin the minimal focus setting in the selected unit(default unit is mm)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocusMin(Camera_handle Camera, unsigned int *FocusMin );


/** getLensFocusMax.
   *  Get the maximal focus that can be used.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param FocusMax the maximal focus setting in the selected unit(default unit is mm)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensFocusMax(Camera_handle Camera, unsigned int *FocusMax);



/** setLensAperture.
   *  A provided aperture will be transferred to lens.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Aperture the aperture to be assigned to the Lens (granularity: 1/10 , ex. 90 corresponds to 9)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_setLensAperture(Camera_handle Camera, unsigned int Aperture);


/** getLensAperture.
   *  The currently set aperture of the lens will be returned.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param Aperture the currently assigned aperture (granularity: 1/10 , ex. 90 corresponds to 9)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensAperture(Camera_handle Camera, unsigned int *Aperture);

/** getLensApertureMin.
   *  Get the minimal aperture that can be used.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param ApertureMin the minimal aperture setting (granularity: 1/10 , ex. 35 corresponds to 3.5)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensApertureMin(Camera_handle Camera, unsigned int *ApertureMin);

/** getLensApertureMax.
   *  Get the maximal aperture that can be used.
   *
   *  @see Camera_isCameraFeature()
   *  @param Camera a handle from a camera that has been opened before
   *  @param ApertureMax the maximal aperture setting (granularity: 1/10 , ex. 90 corresponds to 9)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN Camera_getLensApertureMax(Camera_handle Camera, unsigned int *ApertureMax);

//------------------------------------------------------------------------------
// 99 - Deprecated functions
//------------------------------------------------------------------------------

/** Get maximal framerate.
   *  The currently available maximal framerate will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxFramerate currently available maximal framerate in 1/s
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getMaxFrameRate(Camera_handle Camera,
                       float *MaxFramerate);

/** Get maximal exposure time.
   *  The currently available maximal exposure time will be returned
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxExposureTime currently available maximal exposure time in microseconds
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getMaxExposureTime(Camera_handle Camera,
                          float *MaxExposureTime);

/** Get maximal gain.
   *  The currently available maximal analog gain will be retrieved
   *  Note: A gain of 1.0 is represented as integer 128 in the appropriate camera
   *  register. Thus the gain can be adjusted only in discrete steps.
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @param MaxGain currently available maximal analog gain (0..18 dB)
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_getMaxGain(Camera_handle Camera,
                  float *MaxGain);

/** Start acquisition cycle.
   *  The camera will be triggered for starting a new acquisition cycle.
   *  A mandatory pre-requisition for successfully starting an acquisition
   *  cycle by software is to have the camera set to ACQUISITION_MODE_SOFTWARE_TIGGER
   *  before
   *
   *  @param Camera a handle from a camera that has been opened before
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
SVGigE_RETURN
Camera_startAcquisitionCycle(Camera_handle Camera);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
