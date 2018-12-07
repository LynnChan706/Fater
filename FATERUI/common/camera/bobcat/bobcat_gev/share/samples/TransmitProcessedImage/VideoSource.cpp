// *****************************************************************************
//
// Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvDeviceFinderWnd.h>
#include <PvTransmitterGEV.h>
#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>
#include <PvVersion.h>

#include <sstream>

#include <QtGui/QPainter>

#include "VideoSource.h"


VideoSource::VideoSource( const char *aDeviceAddress )
    : mDeviceAddress( aDeviceAddress )
    , mDevice( NULL )
    , mStream( NULL )
    , mPipeline( NULL )
{
}

VideoSource::~VideoSource()
{
    // Free images
	gImageMap::iterator lIt = mMap.begin();
    while ( lIt != mMap.end() )
    {
        QImage *lImage = ( lIt++ )->second;
        delete lImage;
		lImage = NULL;
    }
}

void VideoSource::Connect()
{
    if ( mDeviceAddress.size() <= 0 )
    {
        // No device address, prompts user
        PvDeviceFinderWnd lWnd;
        if ( lWnd.ShowModal().IsOK() )
        {
            mDeviceAddress = lWnd.GetSelected()->GetConnectionID();
        }
    }

    // Connect device
    PvResult lResult;
    mDevice = PvDevice::CreateAndConnect( mDeviceAddress.c_str(), &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to device." << endl;
        exit( 1 );
    }

    // Open stream
    mStream = PvStream::CreateAndOpen( mDeviceAddress.c_str(), &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to open stream." << endl;
        exit( 1 );
    }

    // Create acquistion pipeline
    mPipeline = new PvPipeline( mStream );
}

void VideoSource::StartAcquisition()
{
    // Start pipeline
    mPipeline->Start();

    PvDeviceGEV *lDeviceGEV = dynamic_cast<PvDeviceGEV *>( mDevice );
    PvStreamGEV *lStreamGEV = dynamic_cast<PvStreamGEV *>( mStream );
    if ( ( lDeviceGEV != NULL ) && ( lStreamGEV != NULL ) )
    { 
        // Set streaming destination
        lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
    }

    // Prepare controller for streaming
    mDevice->StreamEnable();

    // Start acquisition
    mDevice->GetParameters()->ExecuteCommand( "AcquisitionStart" );
}

void VideoSource::StopAcquisition()
{
    // Stop acquisition
    mDevice->GetParameters()->ExecuteCommand( "AcquisitionStop" );

    // Get controller out of streaming
    mDevice->StreamDisable();

    // Stop pipeline
    mPipeline->Stop();
}

void VideoSource::Disconnect()
{
    if ( mPipeline != NULL )
    {
        delete mPipeline;
        mPipeline = NULL;
    }

    if ( mStream != NULL )
    {
        PvStream::Free( mStream );
        mStream = NULL;
    }

    if ( mDevice != NULL )
    {
        PvDevice::Free( mDevice ); 
        mDevice = NULL;
    }
}

bool VideoSource::FillBuffer( PvBuffer *aBuffer, PvTransmitterGEV *aTransmitter )
{
    // Get input buffer from pipeline (stream in)
    PvBuffer *lInBuffer = NULL;
    if ( !mPipeline->RetrieveNextBuffer( &lInBuffer, 100 ).IsOK() )
    {
        return false;
    }

    // We got a buffer, but is it good?
    if ( lInBuffer->GetOperationResult().IsOK() )
    {
		// Get image by ID (one for every buffer, attached for the buffer)
		QImage *lImage = GetImageForBuffer( aBuffer );

		uint32_t lWidth = GetWidth();
		uint32_t lHeight = GetHeight();
		uint32_t lPaddingX = lImage->bytesPerLine() - ( lWidth * 3 );

		// Attach image to output buffer with conversion-happy pixel type
		aBuffer->GetImage()->Attach( lImage->bits(),
			lWidth, lHeight, PvPixelRGB8, lPaddingX );

		// Copy in buffer into image attached to out buffer using eBUS SDK pixel converters
		mConverter.Convert( lInBuffer, aBuffer, false );

		// Draw (transform part of the sample) on image attached to out buffer
		Draw( aTransmitter, lImage );
    }

	// Release buffer back to pipeline
	mPipeline->ReleaseBuffer( lInBuffer );

	return lInBuffer->GetOperationResult().IsOK();
}

uint32_t VideoSource::GetWidth()
{
    int64_t lWidth;
    mDevice->GetParameters()->GetIntegerValue( "Width", lWidth );

    return static_cast<uint32_t>( lWidth );
}

uint32_t VideoSource::GetHeight()
{
    int64_t lHeight;
    mDevice->GetParameters()->GetIntegerValue( "Height", lHeight );

    return static_cast<uint32_t>( lHeight );
}

PvPixelType VideoSource::GetPixelFormat()
{
    int64_t lFormat;
    mDevice->GetParameters()->GetEnumValue( "PixelFormat", lFormat );

    return static_cast<PvPixelType>( lFormat );
}

QImage *VideoSource::GetImageForBuffer( PvBuffer *aBuffer )
{
	static int sNextID = 1;
    uint32_t lID = static_cast<uint32_t>( aBuffer->GetID() );
    if ( lID == 0 )
    {
    	// Make sure all buffers have unique IDs
    	aBuffer->SetID( sNextID++ );
    }

    QImage *lImage = NULL;

    uint32_t lWidth = GetWidth();
    uint32_t lHeight = GetHeight();

    if ( mMap.find( lID ) == mMap.end() )
    {
        // No buffer found at map position
    }
    else
    {
        lImage = mMap[ lID ];
        if ( ( lImage->size().width() != lWidth ) ||
             ( lImage->size().height() != lHeight ) )
        {
            // Detach from PvBuffer
            aBuffer->Detach();

            // Image found, but not the right size
            delete lImage;
            lImage = NULL;
        }
    }

    if ( lImage == NULL )
    {
        // Create a new image of the right size
        lImage = new QImage( lWidth, lHeight, QImage::Format_RGB888 );
        mMap[ lID ] = lImage;
    }

    return lImage;
}

void VideoSource::Draw( PvTransmitterGEV *aTransmitter, QImage *aImage )
{
    stringstream lSS;

    lSS << "TransmitProcessedImage" << endl;
    lSS << PRODUCT_NAME << " " << NVERSION_STRING << endl << endl;

    lSS << fixed << setprecision( 1 );
    lSS << "Transmitted " << aTransmitter->GetBlocksTransmitted() << " blocks" << endl;

    lSS << fixed << setprecision( 1 );
    lSS << aTransmitter->GetAverageTransmissionRate() << " (" << aTransmitter->GetInstantaneousTransmissionRate() << ") FPS" << endl;

    lSS << fixed << setprecision( 1 );
    lSS << aTransmitter->GetAveragePayloadThroughput() / 1000000.0f <<
        " (" << aTransmitter->GetInstantaneousPayloadThroughput() / 1000000.0f << ") Mb/s" << endl;

    QRect lRectangle( QPoint( 25, 25 ) , QPoint( aImage->width(), aImage->height() ) );

    QFont lFont( "Arial", 16 );

    QPainter lPainter( aImage );
	lPainter.setPen( Qt::cyan );
	lPainter.setFont( lFont );

	lPainter.drawText( lRectangle, Qt::AlignTop | Qt::AlignLeft, lSS.str().c_str() );
}


