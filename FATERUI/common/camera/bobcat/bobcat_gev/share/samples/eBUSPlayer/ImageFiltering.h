// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IMAGE_FILTERING_H__
#define __IMAGE_FILTERING_H__

#include "IPersistent.h"

#include <PvBufferConverter.h>
#include <PvBufferConverterRGBFilter.h>

#include <PvRangeFilter.h>


class DisplayThread;


class ImageFiltering : public IPersistent
{
#ifdef _AFXDLL
    DECLARE_DYNAMIC( ImageFiltering )
#endif // _AFXDLL

public:

    ImageFiltering();
    ~ImageFiltering();

    PvResult Save( PvConfigurationWriter *aWriter );
    PvResult Load( PvConfigurationReader *aReader );

    void ResetRGBFilter();
    void WhiteBalance( DisplayThread *aDisplayThread );
    void ConfigureConverter( PvBufferConverter &aConverter );

    bool GetRGBFilterEnabled() const { return mRGBFilterEnabled; }
    PvBufferConverterRGBFilter &GetRGBFilter() { return mRGBFilter; }
    PvBayerFilterType GetBayerFilter() const { return mBayerFilter; }

    void SetRGBFilterEnabled( bool aValue ) { mRGBFilterEnabled = aValue; }
    void SetBayerFilter( PvBayerFilterType aValue ) { mBayerFilter = aValue; }

    PvRangeFilter *GetRangeFilter() { return &mRangeFilter; }

protected:

private:

    bool mRGBFilterEnabled;
    PvBufferConverterRGBFilter mRGBFilter;

    PvBayerFilterType mBayerFilter;

    PvRangeFilter mRangeFilter;

};


#endif // __IMAGE_FILTERING_H__

