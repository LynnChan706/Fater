// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvPixelType.h>
#include <ImagingBuffer.h>
#include <ImagingContrastFilter.h>
#define V_GUI_NOT_AVAILABLE
#ifdef PV_GUI_NOT_AVAILABLE
#include <PvSystem.h>
#else
#include <PvDeviceFinderWnd.h>
#endif // PV_GUI_NOT_AVAILABLE
#include "PvBufferConverter.h"

PvBuffer* gPvBuffers;
SimpleImagingLib::ImagingBuffer* gImagingBuffers;
int32_t gStride;

int64_t gFrameWidthVal;
int64_t gFrameHeightVal;
uint32_t gBufferCount;

// ImageProcessing.cpp : Defines the entry point for the console application.
//
PV_INIT_SIGNAL_HANDLER();

#define BUFFER_COUNT ( 16 )

bool GetFrameDimensions( PvGenParameterArray * aDeviceParams, PvDevice * aDevice ) 
{
    PvGenInteger* lFrameWidth = dynamic_cast<PvGenInteger*>( aDeviceParams->Get( "Width") );
    PvGenInteger* lFrameHeight = dynamic_cast<PvGenInteger*>( aDeviceParams->Get( "Height") );
    
    if ( !lFrameWidth )
    {
        cerr << "Could not get width" << endl;
        PvDevice::Free( aDevice );
        return false;
    }

    if ( !lFrameHeight )
    {
        cerr << "Could not get height" << endl;
        PvDevice::Free( aDevice );
        return false;
    }

    lFrameWidth->GetValue( gFrameWidthVal );
    lFrameHeight->GetValue( gFrameHeightVal );

    cout << "Frame dimensions are " << gFrameWidthVal << "x" << gFrameHeightVal << endl;

    return true;
}

void CreateBuffers( PvDevice* aDevice, PvStream* aStream ) 
{
    gPvBuffers = NULL;
    PvGenParameterArray *lDeviceParams = aDevice->GetParameters();

    // Set device in RGB8 to match what our imaging library expects
    lDeviceParams->SetEnumValue( "PixelFormat", PvPixelRGB8 );

    // Get width, height from device
    int64_t lWidth = 0, lHeight = 0;
    lDeviceParams->GetIntegerValue( "Width", lWidth );
    lDeviceParams->GetIntegerValue( "Height", lHeight );

    // Use min of BUFFER_COUNT and how many buffers can be queued in PvStream.
    gBufferCount = ( aStream->GetQueuedBufferMaximum() < BUFFER_COUNT ) ? 
        aStream->GetQueuedBufferMaximum() : 
        BUFFER_COUNT;
    
    // Create our image buffers which are holding the real memory buffers
    gImagingBuffers = new SimpleImagingLib::ImagingBuffer[ gBufferCount ];
    for ( uint32_t i = 0; i < gBufferCount; i++ )
    {
        gImagingBuffers[ i ].AllocateImage( static_cast<uint32_t>( lWidth ), static_cast<uint32_t>( lHeight ), 3 );
    }

    // Creates, eBUS SDK buffers, attach out image buffer memory
    gPvBuffers = new PvBuffer[ gBufferCount ];
    for ( uint32_t i = 0; i < gBufferCount; i++ )
    {
        // Attach the memory of our imaging buffer to a PvBuffer. The PvBuffer is used as a shell
        // that allows directly acquiring image data into the memory owned by our imaging buffer
        gPvBuffers[ i ].GetImage()->Attach( gImagingBuffers[ i ].GetTopPtr(), 
            static_cast<uint32_t>( lWidth ), static_cast<uint32_t>( lHeight ), PvPixelRGB8 );

        // Set eBUS SDK buffer ID to the buffer/image index
        gPvBuffers[ i ].SetID( i );
    }

    // Queue all buffers in the stream
    for ( uint32_t i = 0; i < gBufferCount; i++ )
    {
        aStream->QueueBuffer( gPvBuffers + i );
    }
}

void ReleaseBuffers() 
{   
    cout << "Releasing buffers" << endl;

    if ( gPvBuffers != NULL )
    {
        delete [] gPvBuffers;
        gPvBuffers = NULL;
    }

    if ( gImagingBuffers != NULL )
    {
        delete []gImagingBuffers;
        gImagingBuffers = NULL;
    }
}

bool ProcessImages()
{
    const PvDeviceInfo *lDeviceInfo = NULL;
    PvResult lResult;

#ifdef PV_GUI_NOT_AVAILABLE
    PvSystem lSystem;

    //Get the selected device information.
    lDeviceInfo = PvSelectDevice( lSystem );
#else
    PvDeviceFinderWnd lDeviceFinderWnd;
    // Shows all devices
    lResult = lDeviceFinderWnd.ShowModal();
    if ( !lResult.IsOK() )
    {
        cout << "No device selected." << endl;
        return false;
    }

    //Get the selected device information.
    lDeviceInfo = lDeviceFinderWnd.GetSelected();
#endif // PV_GUI_NOT_AVAILABLE
    if ( lDeviceInfo == NULL )
    {
        cout << "No device selected." << endl;
        return false;
    }

    // Connect to the GigE Vision or USB3 Vision device
    cout << "Connecting to " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
    PvDevice *lDevice = PvDevice::CreateAndConnect( lDeviceInfo, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
        PvDevice::Free( lDevice );
        return false;
    }
        
    // Get device parameters need to control streaming
    PvGenParameterArray *lDeviceParams = lDevice->GetParameters();
    
    PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStart" ) );
    PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStop" ) );
    
    if ( !GetFrameDimensions( lDeviceParams, lDevice ) )
    {
        return false;
    }

    // Creates stream object
    cout << "Opening stream to device" << endl;
    PvStream* lStream = PvStream::CreateAndOpen( lDeviceInfo->GetConnectionID(), &lResult );
    if ( ( lStream == NULL ) || !lResult.IsOK() )
    {
        cout << "Error creating and opening stream object" << endl;
        PvStream::Free( lStream );
        PvDevice::Free( lDevice );
        return false;
    }

    // Configure streaming for GigE Vision devices
    const PvDeviceInfoGEV* lDeviceInfoGEV = dynamic_cast<const PvDeviceInfoGEV *>( lDeviceInfo );
    if ( lDeviceInfoGEV != NULL )
    {
        PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>( lStream );
        PvDeviceGEV *lDeviceGEV = static_cast<PvDeviceGEV *>( lDevice );

        // Negotiate packet size
        lDeviceGEV->NegotiatePacketSize();

        // Configure device streaming destination
        lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
    }

    // Reading payload size from device.
    CreateBuffers( lDevice, lStream );
    
    // Get stream parameters/stats.
    PvGenParameterArray *lStreamParams = lStream->GetParameters();
    PvGenInteger *lBlockCount = dynamic_cast<PvGenInteger *>( lStreamParams->Get( "BlockCount" ) );
    PvGenFloat *lFrameRate = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "AcquisitionRate" ) );
    PvGenFloat *lBandwidth = dynamic_cast<PvGenFloat *>( lStreamParams->Get( "Bandwidth" ) );
    
    // Enables stream before sending the AcquisitionStart command.
    cout << "Enable streaming on the controller." << endl;
    lDevice->StreamEnable();

    // The buffers are queued in the stream, we just have to tell the device
    // to start sending us images.
    cout << "Sending StartAcquisition command to device" << endl;
    lStart->Execute();

    char lDoodle[] = "|\\-|-/";
    int lDoodleIndex = 0;
    int64_t lBlockCountVal = 0;
    double lFrameRateVal = 0.0;
    double lBandwidthVal = 0.0;

    cout << endl;
    // Acquire images until the user instructs us to stop.
    cout << "<press a key to stop streaming>" << endl;

    // allocate image
    //AlignedImageBufferAllocator
    while ( !PvKbHit() )
    {
        PvBuffer *lBuffer = NULL;
        PvResult lOperationResult;
        int lConvertedBufferIndex = 0;
        PvBufferConverter lBufferConverter;
        SimpleImagingLib::ImagingContrastFilter lContrastFilter;

        // Retrieve next buffer
        PvResult lResult = lStream->RetrieveBuffer( &lBuffer, &lOperationResult, 1000 );
        if ( lResult.IsOK() )
        {
            if (lOperationResult.IsOK())
            {
                //
                // We now have a valid buffer. This is where you would typically process the buffer.
                // -----------------------------------------------------------------------------------------
                // ...

                lBlockCount->GetValue( lBlockCountVal );
                lFrameRate->GetValue( lFrameRateVal );
                lBandwidth->GetValue( lBandwidthVal );

                // Retrieve the imaging buffer based on the buffer's custom ID
                SimpleImagingLib::ImagingBuffer *lImagingBuffer = gImagingBuffers + lBuffer->GetID();

                // Retrieve our image based on buffer ID - which has been set to the index of the array
                lContrastFilter.Apply( lImagingBuffer );

                uint32_t lHeight = lImagingBuffer->GetHeight();
                uint32_t lWidth = lImagingBuffer->GetWidth();

                cout << fixed << setprecision( 1 );
                cout << lDoodle[ lDoodleIndex ];
                cout << " BlockID: " << uppercase << hex << setfill('0') << setw(16) << lBuffer->GetBlockID() << " W: " << dec << lWidth << " H: " 
                    << lHeight << " " << lFrameRateVal << " FPS " << ( lBandwidthVal / 1000000.0 ) << " Mb/s  \r";
            }

            // Re-queue the buffer in the stream object.
            lStream->QueueBuffer( lBuffer );

        }
        else
        {
            // Timeout
            cout << lDoodle[ lDoodleIndex ] << " Timeout\r";
        }

        ++lDoodleIndex %= 6;
    }

    PvGetChar(); // Flush key buffer for next stop.
    cout << endl << endl;

    // Tell the device to stop sending images.
    cout << "Sending AcquisitionStop command to the device" << endl;
    lStop->Execute();

    // Disable stream after sending the AcquisitionStop command.
    cout << "Disable streaming on the controller." << endl;
    lDevice->StreamDisable();

    // Abort all buffers from the stream, dequeue.
    cout << "Aborting buffers still in stream" << endl;
    lStream->AbortQueuedBuffers();
    while ( lStream->GetQueuedBufferCount() > 0 )
    {
        PvBuffer *lBuffer = NULL;
        PvResult lOperationResult;

        lStream->RetrieveBuffer( &lBuffer, &lOperationResult );
    }

    ReleaseBuffers();

    // Now close the stream. Also optional but nice to have.
    cout << "Closing stream" << endl;
    lStream->Close();

    // Finally disconnect the device. Optional, still nice to have.
    cout << "Disconnecting device" << endl;
    lDevice->Disconnect();

    // Free the objects allocated by PvDevice and PvStream factory methods
    PvStream::Free( lStream );
    PvDevice::Free( lDevice );

    return true;
}

int main()
{
    PV_SAMPLE_INIT();

    cout << "ImageProcessing Sample" << endl << endl;
    // PvPipeline used to acquire images from a device.
    ProcessImages();

    cout << endl;
    cout << "<press a key to exit>" << endl;
    PvWaitForKeyPress();

    PV_SAMPLE_TERMINATE();

    return 0;
}

