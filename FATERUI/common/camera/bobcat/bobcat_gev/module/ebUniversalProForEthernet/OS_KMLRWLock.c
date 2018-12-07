// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLRWLock.h"

#include "OS_Misc.h"
   
#include <linux/kernel.h>
#include <linux/spinlock.h>

OS_RESULT OS_RWLockCheck( OS_VOID )
{
    if( unlikely( OS_RWLOCK_SIZE < sizeof( rwlock_t ) ) )
    {
        printk( "rwlock_t size = %d / %d\n", ( int ) sizeof( rwlock_t ), ( int ) OS_RWLOCK_SIZE );
        printk( "OS_RWLock not compatible with your distribution\n" );
        return OS_RESULT_FAIL;
    }

    return OS_RESULT_OK;
}

OS_RESULT OS_RWLockInit( OS_RWLOCK* aLock )
{ 
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;

    OS_ASSERT( OS_RWLOCK_SIZE >= sizeof( rwlock_t ) );

#ifdef RW_LOCK_UNLOCKED
    *lRWLock = RW_LOCK_UNLOCKED;
#else
    rwlock_init( lRWLock );
#endif     

#ifdef OS_TAG_ENABLED
    aLock->Tag = OS_TAG_NOTAG; 
#endif // OS_TAG_ENABLED

    return OS_RESULT_OK;
}
 
OS_RESULT OS_RWLockFree( OS_RWLOCK* aLock )
{ 
    OS_UNREFERENCED_PARAMETER( aLock );

    // Nothing to do for the linux kernel
    return OS_RESULT_OK;
}

OS_VOID OS_RWLockWriteAcquire( OS_RWLOCK* aLock, OS_TAG aTag, OS_RWLOCK_FLAG* aFlag )
{
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;
	unsigned long lFlag;

#ifndef OS_TAG_ENABLED
    OS_UNREFERENCED_PARAMETER( aTag );    
#else
    OS_ASSERT( aTag != OS_TAG_NOTAG );
#endif // OS_TAG_ENABLED

    write_lock_irqsave( lRWLock, lFlag );
	*aFlag = ( OS_RWLOCK_FLAG ) lFlag;

#ifdef OS_TAG_ENABLED
    aLock->Tag = aTag;
#endif // OS_TAG_ENABLED
}

OS_VOID OS_RWLockWriteRelease( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag )
{
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;

#ifdef OS_TAG_ENABLED
    OS_ASSERT( aTag != OS_TAG_NOTAG );
    aLock->Tag = OS_TAG_NOTAG;
#endif // OS_TAG_ENABLED

    write_unlock_irqrestore( lRWLock, ( unsigned long ) *aFlag );
}
 
OS_VOID OS_RWLockReadAcquire( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag )
{
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;
	unsigned long lFlag;

    read_lock_irqsave( lRWLock, lFlag );
	*aFlag = ( OS_RWLOCK_FLAG ) lFlag;
}

OS_VOID OS_RWLockReadRelease( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag )
{
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;

    read_unlock_irqrestore( lRWLock, ( unsigned long ) *aFlag );
}

OS_VOID OS_RWLockConvertWriteToRead( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag )
{
    rwlock_t* lRWLock = ( rwlock_t * ) aLock->Space;
	unsigned long lFlag;

    write_unlock_irqrestore( lRWLock, ( unsigned long ) *aFlag );
    read_lock_irqsave( lRWLock, lFlag );
	*aFlag = ( OS_RWLOCK_FLAG ) lFlag;
}
