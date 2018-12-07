// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLMemory.h"

#include <linux/kernel.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 18)
#include <asm/uaccess.h>
#else
#include <linux/uaccess.h>
#endif
#include <linux/slab.h>

OS_VOID* OS_Malloc( OS_DRV_CONTEXT* aContext, OS_SIZE aSize, OS_UINT32 aTag )
{
    OS_UNREFERENCED_PARAMETER( aContext );
    OS_UNREFERENCED_PARAMETER( aTag );

    if( aSize > OS_MALLOC_MAX_SIZE )
    {
        printk( "Allocation of %d bytes of memory with tag %d is too big for OS_Malloc\n", aSize, aTag );
        return NULL;
    }

    return kmalloc( aSize, GFP_KERNEL );
}

OS_VOID OS_Free( OS_VOID* aAddress )
{
    kfree( aAddress );
}

OS_INT32 OS_Memcmp( const OS_VOID* aBuffer1, const OS_VOID* aBuffer2, OS_SIZE aSize )
{
    return ( OS_INT32 ) memcmp( aBuffer1, aBuffer2, aSize );
}

OS_VOID OS_Memset( OS_VOID* aAddress, OS_UINT32 aValue, OS_SIZE aSize )
{
    memset( aAddress, aValue, aSize );
}

OS_VOID OS_Memcpy( OS_VOID* aDestination, const OS_VOID* aSource, OS_SIZE aSize )
{
    memcpy( aDestination, aSource, aSize );
}

OS_UINT32 OS_MemcpyToUser( OS_VOID* aTo, const OS_VOID* aFrom, OS_SIZE aSize )
{
    return copy_to_user( aTo, aFrom, ( unsigned long ) aSize );
}

OS_UINT32 OS_MemcpyFromUser( OS_VOID* aTo, const OS_VOID* aFrom, OS_SIZE aSize )
{
    return copy_from_user( aTo, aFrom, ( unsigned long ) aSize );
}
