// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLMemoryMap.h"

#include <asm/current.h>
#include <linux/mm.h>
#include <linux/page-flags.h>
#include <linux/pagemap.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/version.h>

OS_RESULT OS_MapCreate( OS_DRV_CONTEXT* aContext, OS_MEMORYMAP* aMap, 
    OS_VOID* aUserAddress, OS_UINT32 aSize )
{
    int lPageCount;
    struct page** lPageList;
    void* lKernelPageAddress;
    unsigned int i;
    unsigned long lFirstPage;
    unsigned long lLastPage;

    OS_UNREFERENCED_PARAMETER( aContext );

    aMap->UserAddress = NULL;
    aMap->KernelAddress = NULL; 

    if( unlikely( !aUserAddress 
        || !aSize
        || !access_ok( VERIFY_WRITE, aUserAddress, aSize ) ) )
    {
        return OS_RESULT_INVALID_ARGUMENT;
    }

    lFirstPage = ( ( unsigned long ) aUserAddress ) >> PAGE_SHIFT;
    lLastPage = ( ( ( unsigned long ) aUserAddress ) + ( unsigned long ) aSize ) >> PAGE_SHIFT;
    aMap->PageCount = lLastPage - lFirstPage + 1; 

    aMap->PageList = kmalloc( sizeof( struct page* ) * aMap->PageCount, GFP_KERNEL );
    if( unlikely( !aMap->PageList ) )
    {
        return OS_RESULT_OUT_OF_RESOURCES;
    }
    lPageList = ( struct page** ) aMap->PageList;


#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) || !defined( OS_ALLOW_GPL ) )
    down_read( &current->mm->mmap_sem );
    lPageCount = get_user_pages( current, current->mm, ( ( unsigned long ) aUserAddress ) & PAGE_MASK,
        aMap->PageCount, 1, 0, lPageList, NULL );
    up_read( &current->mm->mmap_sem );
#else
    lPageCount = get_user_pages_fast( ( ( unsigned long ) aUserAddress ) & PAGE_MASK, aMap->PageCount,
        1, lPageList );
#endif 
    if( unlikely( lPageCount != ( int ) aMap->PageCount ) )
    {
        for( i = 0; i < ( unsigned int ) lPageCount; i++ )
        {
            page_cache_release( lPageList[ i ] );
        }
        kfree( lPageList );
        return OS_RESULT_OUT_OF_RESOURCES;
    }
    
    lKernelPageAddress = vmap( ( struct page** ) aMap->PageList, aMap->PageCount, VM_MAP, PAGE_KERNEL );
    if( unlikely( !lKernelPageAddress ) )
    {
        for( i = 0; i < aMap->PageCount; i++ )
        {
             page_cache_release( lPageList[ i ] );
        }
        kfree( lPageList );
        return OS_RESULT_OUT_OF_RESOURCES;
    }

    aMap->UserAddress = aUserAddress;
    aMap->KernelAddress = ( OS_VOID* ) ( ( ( unsigned long ) lKernelPageAddress ) 
        + offset_in_page( aUserAddress ) );
    aMap->Size = aSize;

    return OS_RESULT_OK;
}

OS_RESULT OS_MapDelete( OS_MEMORYMAP* aMap )
{
    struct page* lPage;
    struct page** lPageList = ( struct page** ) aMap->PageList;
    unsigned int i;

    vunmap( ( void* ) ( ( ( unsigned long ) aMap->KernelAddress ) & PAGE_MASK ) );

    for( i = 0; i < aMap->PageCount; i++ )
    {
        lPage = lPageList[ i ];
        if( !PageReserved( lPage ) )
        {
            SetPageDirty( lPage );
        }  
        page_cache_release( lPage ); 
    }        
    kfree( lPageList );

    return OS_RESULT_OK;
}

