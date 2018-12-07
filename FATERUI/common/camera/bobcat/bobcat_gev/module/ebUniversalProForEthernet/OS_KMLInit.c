// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "OS_KMLInit.h"

#include "OS_Event.h"
#include "OS_Lock.h"
#include "OS_RWLock.h"
#include "OS_ShareMemory.h"
#include "OS_System.h"
#include "OS_Timer.h"
#include "OS_WorkQueue.h"

#include <linux/kernel.h>

static OS_UINT32 sInstanceCounter = 0;

OS_RESULT OS_Init( OS_VOID )
{
    OS_RESULT lResult;
    OS_UINT32 lFail = 0;

    // Must validate the size of the structure to ensure we have enough space 
    // for the support of the compilation layer
    if( !sInstanceCounter )
    {
        // Check for the size of the space to contains Linux natice structures
        lResult = OS_EventCheck();
        lFail |= ( lResult != OS_RESULT_OK );
        lResult = OS_LockCheck();
        lFail |= ( lResult != OS_RESULT_OK );
        lResult = OS_RWLockCheck();
        lFail |= ( lResult != OS_RESULT_OK );
        lResult = OS_TimerCheck();
        lFail |= ( lResult != OS_RESULT_OK );
        lResult = OS_WorkQueueCheck();
        lFail |= ( lResult != OS_RESULT_OK );
        if( lFail )
        {     
            printk( "Internal size allocated by the system is not big enough\n");  
            return OS_RESULT_FAIL;
        }

        // Initialize the internal library
        lResult = OS_ShareMemoryStaticInit();
        if( unlikely( lResult != OS_RESULT_OK ) )
        {
            printk( "OS_ShareMemoryStaticInit FAIL\n");
            return OS_RESULT_FAIL;
        }
        lResult = OS_SystemStaticInit( 0 );
        if( unlikely( lResult != OS_RESULT_OK ) )
        {
            printk( "OS_SystemStaticInit FAIL\n");
            return OS_RESULT_FAIL;
        }
    }
	sInstanceCounter++;

    return OS_RESULT_OK;
}

OS_RESULT OS_Close( OS_VOID )
{
    sInstanceCounter--;
    if ( sInstanceCounter == 0 )
    {
        OS_SystemStaticClose();
		OS_ShareMemoryStaticClose();
    }

    return OS_RESULT_OK;
}
