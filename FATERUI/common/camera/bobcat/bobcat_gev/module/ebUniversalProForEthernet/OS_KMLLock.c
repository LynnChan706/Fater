// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLLock.h"

#include "OS_Misc.h"

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/version.h>

OS_RESULT OS_LockCheck( OS_VOID )
{
    if( unlikely( OS_SPINLOCK_SIZE < sizeof( spinlock_t ) ) )
    {
        printk( "spinlock_t size = %d / %d\n", ( int ) sizeof( spinlock_t ), ( int ) OS_SPINLOCK_SIZE );
        printk( "OS_Lock not compatible with your distribution\n" );
        return OS_RESULT_FAIL;
    }

    return OS_RESULT_OK;
}

OS_RESULT OS_LockInit( OS_LOCK* aLock )
{
    spinlock_t* lSpinlock = ( spinlock_t * ) aLock->Space;

    OS_ASSERT( OS_SPINLOCK_SIZE >= sizeof( spinlock_t ) );

#ifdef SPIN_LOCK_UNLOCKED
    *lSpinlock = SPIN_LOCK_UNLOCKED;
#else
    spin_lock_init( lSpinlock );
#endif

#ifdef OS_TAG_ENABLED
    aLock->Tag = OS_TAG_NOTAG;
#endif // OS_TAG_ENABLED

    return OS_RESULT_OK;
} 

OS_RESULT OS_LockFree( OS_LOCK* aLock )
{
    OS_UNREFERENCED_PARAMETER( aLock );

    // Nothing to do for the linux kernel
    return OS_RESULT_OK;
}

OS_RESULT OS_LockAcquire( OS_LOCK* aLock, OS_TAG aTag, OS_LOCK_FLAG* aFlag, OS_UINT32 aDprLevel )
{
    spinlock_t* lSpinlock = ( spinlock_t * ) aLock->Space;

    OS_UNREFERENCED_PARAMETER( aDprLevel );
#ifndef OS_TAG_ENABLED
    OS_UNREFERENCED_PARAMETER( aTag );    
#else
    OS_ASSERT( aTag != OS_TAG_NOTAG );
#endif // OS_TAG_ENABLED

    spin_lock_irqsave( lSpinlock, *aFlag );

#ifdef OS_TAG_ENABLED
    aLock->Tag = aTag;
#endif // OS_TAG_ENABLED

    return OS_RESULT_OK;
}

OS_RESULT OS_LockRelease( OS_LOCK* aLock, OS_LOCK_FLAG* aFlag, OS_UINT32 aDprLevel )
{
    spinlock_t* lSpinlock = ( spinlock_t * ) aLock->Space;

    OS_UNREFERENCED_PARAMETER( aDprLevel );

#ifdef OS_TAG_ENABLED
    OS_ASSERT( aLock->Tag != OS_TAG_NOTAG );
    aLock->Tag = OS_TAG_NOTAG;
#endif // OS_TAG_ENABLED

    spin_unlock_irqrestore( lSpinlock, *aFlag );

    return OS_RESULT_OK;
}

OS_VOID OS_LockIncreaseInt32( OS_INT32* aValue, OS_LOCK* aLock, OS_UINT32 aDprLevel ) 
{
    OS_LOCK_FLAG lFlag;
    spinlock_t* lSpinlock = ( spinlock_t * ) aLock->Space;

    OS_UNREFERENCED_PARAMETER( aDprLevel );

#ifndef OS_TAG_ENABLED
    OS_ASSERT( aTag != OS_TAG_NOTAG );
#endif // OS_TAG_ENABLED

    spin_lock_irqsave( lSpinlock, lFlag );
#ifdef OS_TAG_ENABLED
    aLock->Tag = OS_TAG_LOCKINCREASEINT32;
#endif // OS_TAG_ENABLED

    ( *aValue )++;

#ifdef OS_TAG_ENABLED
    OS_ASSERT( aLock->Tag != OS_TAG_NOTAG );
    aLock->Tag = OS_TAG_NOTAG;
#endif // OS_TAG_ENABLED

    spin_unlock_irqrestore( lSpinlock, lFlag );
}

OS_VOID OS_LockDecreaseInt32( OS_INT32* aValue, OS_LOCK* aLock, OS_UINT32 aDprLevel ) 
{
    OS_LOCK_FLAG lFlag;
    spinlock_t* lSpinlock = ( spinlock_t * ) aLock->Space;

    OS_UNREFERENCED_PARAMETER( aDprLevel );

#ifndef OS_TAG_ENABLED
    OS_ASSERT( aTag != OS_TAG_NOTAG );
#endif // OS_TAG_ENABLED

    spin_lock_irqsave( lSpinlock, lFlag );
#ifdef OS_TAG_ENABLED
    aLock->Tag = OS_TAG_LOCKINCREASEINT32;
#endif // OS_TAG_ENABLED

    ( *aValue )--;

#ifdef OS_TAG_ENABLED
    OS_ASSERT( aLock->Tag != OS_TAG_NOTAG );
    aLock->Tag = OS_TAG_NOTAG;
#endif // OS_TAG_ENABLED

    spin_unlock_irqrestore( lSpinlock, lFlag );
}
