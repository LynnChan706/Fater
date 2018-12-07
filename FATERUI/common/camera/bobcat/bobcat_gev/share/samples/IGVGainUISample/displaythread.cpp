// *****************************************************************************
//
//     Copyright (c) 2014, Imperx Inc., All rights reserved.
//
// *****************************************************************************


#include "displaythread.h"
#include <assert.h>
#include <sstream>

///
/// \brief Constructor
///

DisplayThread::DisplayThread(
    PvDisplayWnd *aDisplayWnd)
    : mDisplayWnd( aDisplayWnd )
{
}


///
/// \brief Destructor
///

DisplayThread::~DisplayThread()
{

}

///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDisplay( PvBuffer *aBuffer )
{
    // Configure converter, display buffer
    mDisplayWnd->Display( *aBuffer, GetVSyncEnabled() );
}

