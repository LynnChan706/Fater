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
    PvDisplayWnd *aDisplayWnd, TrueSenseConverter *tsConv)
    : mDisplayWnd( aDisplayWnd )
    , mTrueSenseConv(tsConv)
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
    if(mTrueSenseConv->Enabled())
    {
		PvBuffer &buff = mTrueSenseConv->ConvertForDisplay(*aBuffer);
		mDisplayWnd->Display(buff, GetVSyncEnabled());
	}
	else
	{
		mDisplayWnd->Display( *aBuffer, GetVSyncEnabled() );
	}
}

