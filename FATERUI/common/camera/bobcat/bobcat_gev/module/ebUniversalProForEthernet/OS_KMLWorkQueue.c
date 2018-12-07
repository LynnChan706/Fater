// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLMisc.h"
#include "OS_KMLWorkQueue.h"

#include <asm/atomic.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/version.h>
#include <linux/workqueue.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
static void OS_WorkQueueRun( struct work_struct* aWork );
#else
static void OS_WorkQueueRun( void* aData );
#endif // KERNEL_VERSION

OS_RESULT OS_WorkQueueCheck( OS_VOID )
{
    if( sizeof( int ) < sizeof( atomic_t ) )
    {
        printk( "atomic_t type size = %d / %d\n", ( int ) sizeof( atomic_t ), ( int ) sizeof( int ) );
        printk( "OS_WorkQueue not compatible with your distribution\n" );
        return OS_RESULT_FAIL;
    }

    if( OS_WORKITEM_SIZE < sizeof( struct work_struct ) )
    {
        printk( "struct tasklet_struct size = %d / %d\n", ( int ) sizeof( struct work_struct ), ( int ) OS_WORKITEM_SIZE );
        printk( "OS_WorkQueue not compatible with your distribution\n" );
        return OS_RESULT_FAIL;
    }

    return OS_RESULT_OK;
}

OS_RESULT OS_WorkQueueInit( OS_WORKQUEUE* aWorkQueue, OS_CHAR* aName, 
	POS_WORKQUEUE_FUNC aFunction )
{
#ifdef OS_ALLOW_GPL
	struct workqueue_struct* lWorkQueue;

	// Create the work queue
	lWorkQueue = create_workqueue( ( const char* ) aName );
	if( !lWorkQueue )
	{
		return OS_RESULT_OUT_OF_RESOURCES;
	}

	// Now keep all the configuration for future usage
	aWorkQueue->WorkQueue = ( OS_WORKQUEUE* ) lWorkQueue;
#else
	OS_UNREFERENCED_PARAMETER( aName );
    
    // Initialize the use count
    atomic_set( ( atomic_t* ) &aWorkQueue->UseCount, 0 );
#endif // OS_ALLOW_GPL

	aWorkQueue->Function = aFunction;

	// Note:
	// The function if a by workqueue to be compatible with NDIS

    return OS_RESULT_OK;
}

OS_RESULT OS_WorkQueueFree( OS_WORKQUEUE* aWorkQueue )
{
#ifdef OS_ALLOW_GPL
    struct workqueue_struct* lWorkQueue = ( struct workqueue_struct* ) aWorkQueue->WorkQueue;

	flush_workqueue( lWorkQueue );
	destroy_workqueue( lWorkQueue );
#else
    // flush_scheduled_work() is not used because of potential deadlock and the time 
    // to execute is not controlled.

    // Wait for the use count to go back to 0. This will ensure a proper synchronization
    // an prevent race condition at close time
    while( atomic_add_return( 0, ( atomic_t* ) &aWorkQueue->UseCount ) != 0 )
    {
        set_current_state( TASK_INTERRUPTIBLE );
        schedule_timeout( msecs_to_jiffies( 50 ) );
    }    
#endif // OS_ALLOW_GPL

	return OS_RESULT_OK;
}

OS_RESULT OS_WorkQueueSchedule( OS_WORKQUEUE* aWorkQueue, OS_WORKITEM* aWorkItem, 
	OS_VOID* aData )
{
#ifdef OS_ALLOW_GPL
    struct workqueue_struct* lWorkQueue = ( struct workqueue_struct* ) aWorkQueue->WorkQueue;
#endif // OS_ALLOW_GPL
    struct work_struct* lWorkItem = ( struct work_struct* ) aWorkItem->Space;
	int lReturn;

	aWorkItem->WorkQueue = aWorkQueue;
	aWorkItem->Data = aData;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	INIT_WORK( lWorkItem, OS_WorkQueueRun );
#else
	INIT_WORK( lWorkItem, OS_WorkQueueRun, aWorkItem );
#endif // KERNEL_VERSION

#ifdef OS_ALLOW_GPL
	lReturn = queue_work( lWorkQueue, lWorkItem );
#else
    atomic_inc( ( atomic_t* ) &aWorkQueue->UseCount );
	lReturn = schedule_work( lWorkItem );

#endif // OS_ALLOW_GPL
	if( !lReturn )
	{
		return OS_RESULT_FAIL;
	}

	return OS_RESULT_OK;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
void OS_WorkQueueRun( struct work_struct* aWork )
{
    OS_WORKITEM* lWorkItem = ( OS_WORKITEM* ) ( ( ( char* ) aWork ) - offsetof( OS_WORKITEM, Space ) );
#else
void OS_WorkQueueRun( void* aData )
{
    OS_WORKITEM* lWorkItem = ( OS_WORKITEM* ) aData;
#endif // KERNEL_VERSION
	
	// Now run the function to execute the processing
	lWorkItem->WorkQueue->Function( lWorkItem->Data );

    atomic_dec( ( atomic_t* ) &lWorkItem->WorkQueue->UseCount );
}


