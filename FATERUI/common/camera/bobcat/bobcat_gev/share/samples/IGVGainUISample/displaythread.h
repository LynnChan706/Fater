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


class DisplayThread : public PvDisplayThread
{
public:

	DisplayThread(
        PvDisplayWnd *aDisplayWnd);
	~DisplayThread();

protected:

    void OnBufferDisplay( PvBuffer *aBuffer );

private:

	PvDisplayWnd *mDisplayWnd;

};

