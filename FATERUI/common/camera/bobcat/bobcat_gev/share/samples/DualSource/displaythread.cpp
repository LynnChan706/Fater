// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************


#include "displaythread.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTime>

#include <assert.h>


///
/// \brief Constructor
///

DisplayThread::DisplayThread( PvDisplayWnd *aDisplayWnd )
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

void DisplayThread::OnBufferRetrieved( PvBuffer *aBuffer )
{
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDisplay( PvBuffer *aBuffer )
{
    mDisplayWnd->Display( *aBuffer );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDone( PvBuffer *aBuffer )
{
}


