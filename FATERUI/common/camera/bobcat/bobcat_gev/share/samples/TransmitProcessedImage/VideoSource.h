// *****************************************************************************
//
// Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __VIDEO_SOURCE_H__
#define __VIDEO_SOURCE_H__

#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvBufferConverter.h>

#include <QtGui/QImage>

#include <map>


using namespace std;


typedef map<uint32_t, QImage *> gImageMap;
class PvTransmitterGEV;


// Video source
class VideoSource
{
public:

    VideoSource( const char *aDeviceAddress );
    ~VideoSource();

    uint32_t GetWidth();
    uint32_t GetHeight();
    PvPixelType GetPixelFormat();

    void Connect();
    void StartAcquisition();
    void StopAcquisition();
    void Disconnect();

    bool FillBuffer( PvBuffer *aBuffer, PvTransmitterGEV *aTransmitter );

private:

    QImage *GetImageForBuffer( PvBuffer *aBuffer );
    void Draw( PvTransmitterGEV *aTransmitter, QImage *aImage );

    std::string mDeviceAddress;

    PvDevice *mDevice;
    PvStream *mStream;
    PvPipeline *mPipeline;

    PvBufferConverter mConverter;

    gImageMap mMap;
};


#endif // __VIDEO_SOURCE_H__

