// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLTimer.h"

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/timer.h>

static void OS_TimerInternalFunc( unsigned long aData );
static void OS_TimerPeriodicInternalFunc( unsigned long aData );

OS_RESULT OS_TimerCheck( OS_VOID )
{
    if( unlikely( OS_TIMER_SIZE < sizeof( struct timer_list ) ) )
    {
        printk( "timer_list size = %d / %d\n", ( int ) sizeof( struct timer_list ), ( int ) OS_TIMER_SIZE );
        printk( "OS_Timer not compatible with your distribution\n" );
        return OS_RESULT_FAIL;
    }

    return OS_RESULT_OK;
}

void OS_TimerInternalFunc( unsigned long aData )
{
    OS_TIMER* lTimerData = ( OS_TIMER* ) aData;
    
    // Execute timer
    lTimerData->Function( lTimerData->Data );    
}

void OS_TimerPeriodicInternalFunc( unsigned long aData )
{
    OS_TIMER* lTimerData = ( OS_TIMER* ) aData;
    struct timer_list* lTimer = ( struct timer_list* ) lTimerData->Space;
    
    // Execute timer
    lTimerData->Function( lTimerData->Data );    
    
    // Prepare the next execution of the timer
    lTimer->expires += lTimerData->Period;

    // reschedule the next execution
    add_timer( lTimer );
}

OS_RESULT OS_TimerCreate( OS_DRV_CONTEXT* aContext, OS_TIMER* aTimer, OS_UINT32 aPeriodic, OS_TIMER_FNC aFunction, 
		    OS_VOID* aData, OS_UINT32 aPeriod )
{
    struct timer_list* lTimer = ( struct timer_list* ) aTimer->Space;
    unsigned long lJiffies = jiffies;
    unsigned long lPeriod = msecs_to_jiffies( aPeriod );
    
    OS_UNREFERENCED_PARAMETER( aContext );

    OS_ASSERT( OS_TIMER_SIZE >= sizeof( timer_list ) );

    init_timer( lTimer );

    lTimer->expires = lJiffies + lPeriod;
    lTimer->data = ( unsigned long ) aTimer;
    aTimer->Function = aFunction;
    aTimer->Data = aData;   
    aTimer->Period = lPeriod;
    if ( aPeriodic )
    {
        lTimer->function = OS_TimerPeriodicInternalFunc; 
    }
    else
    {
        lTimer->function = OS_TimerInternalFunc; 
    }

    add_timer( lTimer );

    return OS_RESULT_OK;
}

OS_RESULT OS_TimerDestroy( OS_TIMER* aTimer )
{
    struct timer_list* lTimer = ( struct timer_list* ) aTimer->Space;
    
    del_timer_sync( lTimer );

    return OS_RESULT_OK;
}
