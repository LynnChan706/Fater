// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KMLMEMORYMAP_H__
#define __OS_KMLMEMORYMAP_H__

#include "OS_Compiler.h"
#include "OS_Memory.h"
#include "OS_Misc.h"
#include "OS_Result.h"
#include "OS_Types.h"

typedef struct 
{
    unsigned long  PageCount;
    OS_VOID**      PageList;
    unsigned long  OffsetPage;
    OS_VOID*       KernelAddress;
	OS_VOID*       UserAddress;
	OS_UINT32      Size;
} OS_MEMORYMAP;  

OS_DECL_INLINE OS_VOID* OS_MapGetKernelAddress( OS_MEMORYMAP* aMap )
{
	return aMap->KernelAddress;
}

OS_DECL_INLINE OS_VOID* OS_MapGetUserAddress( OS_MEMORYMAP* aMap )
{
	return aMap->UserAddress;
}

OS_DECL_INLINE OS_UINT32 OS_MapGetSize( OS_MEMORYMAP* aMap )
{
	return aMap->Size;
}

OS_DECL_EXTERN OS_RESULT OS_MapCreate( OS_DRV_CONTEXT* aContext, OS_MEMORYMAP* aMap, OS_VOID* aUserAddress, OS_UINT32 aSize );
OS_DECL_EXTERN OS_RESULT OS_MapDelete( OS_MEMORYMAP* aMap );

OS_DECL_INLINE OS_RESULT OS_MapCopyTo( OS_MEMORYMAP* aMap, OS_UINT32 aMapOffset, OS_VOID* aSourceAddress, OS_UINT32 aSourceSize )
{
	OS_Memcpy( ( ( OS_UINT8* ) aMap->KernelAddress ) + aMapOffset, aSourceAddress, aSourceSize );	
	return OS_RESULT_OK;
}

#endif // __OS_KMLMEMORYMAP_H__
