// *****************************************************************************
//
// Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvBuffer.h>

#include "VideoSource.h"


VideoSource::VideoSource()
    : mSeed( 0 )
{
}

VideoSource::~VideoSource()
{
}

void VideoSource::CopyPattern( PvBuffer *aBuffer )
{
    uint32_t lWidth = aBuffer->GetImage()->GetWidth();
    uint32_t lHeight = aBuffer->GetImage()->GetHeight();

    unsigned char *lPtr = aBuffer->GetDataPointer();
    for ( uint32_t y = 0; y < lHeight; y++ )
    {
        unsigned char lValue = mSeed + y;
        for ( uint32_t x = 0; x < lWidth; x++ )
        {
            *( lPtr++ ) = lValue++;
        }
    }

    // Make sure we get a different pattern next time by
    mSeed++;
}




