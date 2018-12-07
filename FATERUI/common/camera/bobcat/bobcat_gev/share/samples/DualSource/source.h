// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <PvAcquisitionStateManager.h>
#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>
#include <PvPipeline.h>

#include <vector>


class PvDisplayWnd;
class SourceWidget;
class DisplayThread;


class Source : protected PvAcquisitionStateEventSink, protected PvPipelineEventSink, protected PvGenEventSink
{
public:

    Source( uint16_t aSourceIndex );
    virtual ~Source();

    PvResult Open( PvDeviceGEV *aDevice, const PvString &aIPAddress, const PvString &aLocalIPAddress, uint16_t aChannel );
    PvResult Close();

    PvResult StartStreaming();
    PvResult StopStreaming();

    PvResult StartAcquisition();
    PvResult StopAcquisition();

	bool IsOpened() { return mStream->IsOpen(); }
	PvGenParameterArray *GetParameters() { return mStream->GetParameters(); }
	PvAcquisitionState GetAcquisitionState();

	void GetAcquisitionMode( bool aSelectSourceIfNeeded, QString &aAcquisitionMode );
	void GetAcquisitionModes( std::vector<QString> &aVector );
	PvResult SetAcquisitionMode( const QString &aAcquisitionMode );

	void SetWidget( SourceWidget *aWidget );

	void GetStreamInfo( QString &aInfo );

protected:

    uint32_t GetPayloadSize();

     // PvPipelineEventSink events
    void OnBufferTooSmall( PvPipeline *aPipeline, bool *aReallocAll, bool *aResetStats );

    // PvAcquisitionStateEventSink events
    void OnAcquisitionStateChanged( PvDevice* aDevice, PvStream* aStream, uint32_t aSource, PvAcquisitionState aState );

    // PvGenParameterEventSink events
	void OnParameterUpdate( PvGenParameter *aParameter );

private:

    // Owned by the source object
    PvStreamGEV *mStream;
    PvPipeline *mPipeline;
    PvAcquisitionStateManager *mAcquisitionManager;

    // Referenced from outside
    PvDeviceGEV *mDevice;
    SourceWidget *mWidget;

    DisplayThread *mDisplayThread;

    uint16_t mSourceIndex;

    bool mMultiSource;
    bool mBuffersReallocated;
};


#endif // __SOURCE_H__


