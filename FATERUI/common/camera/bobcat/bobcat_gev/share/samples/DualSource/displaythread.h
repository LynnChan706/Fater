// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvBuffer.h>
#include <PvBufferConverter.h>
#include <PvFPSStabilizer.h>
#include <PvDisplayWnd.h>
#include <PvDisplayThread.h>


class ImageSaveDlg;
class FilteringDlg;




class DisplayThread : public PvDisplayThread
{
public:

	DisplayThread( PvDisplayWnd *aDisplayWnd );
	~DisplayThread();

protected:

    void OnBufferRetrieved( PvBuffer *aBuffer );
    void OnBufferDisplay( PvBuffer *aBuffer );
    void OnBufferDone( PvBuffer *aBuffer );

private:

	PvDisplayWnd *mDisplayWnd;
};

