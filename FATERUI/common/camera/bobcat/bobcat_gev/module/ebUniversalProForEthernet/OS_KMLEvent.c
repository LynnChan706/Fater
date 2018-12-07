// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLEvent.h"

#include "OS_Misc.h"

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>

OS_RESULT OS_EventCheck( OS_VOID )
{
    if( unlikely( OS_EVENT_SIZE < sizeof( wait_queue_head_t ) ) )
    {
        printk( "wait_queue_head_t size = %d / %d\n", ( int ) sizeof( wait_queue_head_t ), ( int ) OS_EVENT_SIZE );
        printk( "OS_Event not compatible with your distribution" );
        return OS_RESULT_FAIL;
    }

    return OS_RESULT_OK;
}

OS_RESULT OS_EventInit( OS_EVENT* aEvent )
{
    wait_queue_head_t* lWaitQueue = ( wait_queue_head_t* ) aEvent->Space;
    
    OS_ASSERT( OS_EVENT_SIZE >= sizeof( wait_queue_head_t ) );

    aEvent->Flag = 0;
    init_waitqueue_head( lWaitQueue );

    return OS_RESULT_OK;
}

OS_RESULT OS_EventFree( OS_EVENT* aEvent )
{
    OS_UNREFERENCED_PARAMETER( aEvent );

    return OS_RESULT_OK;
}

OS_RESULT OS_EventWait( OS_EVENT* aEvent )
{
    wait_queue_head_t* lWaitQueue = ( wait_queue_head_t* ) aEvent->Space;
    long lReturn;
    OS_RESULT lResult = OS_RESULT_OK;

    if( likely( !aEvent->Flag ) )
    {
        lReturn = wait_event_interruptible( ( *lWaitQueue ), aEvent->Flag );
        if( unlikely( lReturn != 0 ) )
        {        
            lResult = OS_RESULT_FAIL;
            if( unlikely( aEvent->Flag ) )
            {
                lResult = OS_RESULT_OK;
            }
        }
    }

    return lResult;  

    // This call was interrupt by a signal
    return OS_RESULT_FAIL;
}

OS_RESULT OS_EventWaitWithTimeout( OS_EVENT* aEvent, OS_UINT64 aTimeout )
{
    wait_queue_head_t* lWaitQueue = ( wait_queue_head_t* ) aEvent->Space;
    long lReturn;
    OS_RESULT lResult = OS_RESULT_OK;

    if( likely( !aEvent->Flag ) )
    {
        lReturn = wait_event_interruptible_timeout( ( *lWaitQueue ), aEvent->Flag, 
           usecs_to_jiffies( aTimeout ) );
        if ( lReturn == 0 )
        {
            lResult = OS_RESULT_TIMEOUT;
            if( unlikely( aEvent->Flag ) )
            {
                lResult = OS_RESULT_OK;
            }
        }
        else if( unlikely( lReturn < 0 ) )
        {
            lResult = OS_RESULT_FAIL;
            if( unlikely( aEvent->Flag ) )
            {
                lResult = OS_RESULT_OK;
            }
        }
    }

    return lResult;    
}

OS_RESULT OS_EventRaise( OS_EVENT* aEvent )
{
    wait_queue_head_t* lWaitQueue = ( wait_queue_head_t* ) aEvent->Space;

    aEvent->Flag = 1;
    wake_up_interruptible( lWaitQueue );

    return OS_RESULT_OK;
}

OS_RESULT OS_EventReset( OS_EVENT* aEvent )
{
    aEvent->Flag = 0;

    return OS_RESULT_OK;
}
