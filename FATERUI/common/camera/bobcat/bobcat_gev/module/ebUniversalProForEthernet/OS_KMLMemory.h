// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_MEMORY_H__
#define __OS_KML_MEMORY_H__

#include "OS_Misc.h"
#include "OS_Types.h"

#define OS_MALLOC_TAG( a ) ( 0 )

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_MALLOC_MAX_SIZE      ( 131072 )

OS_DECL_EXTERN OS_VOID*  OS_Malloc( OS_DRV_CONTEXT* aContext, OS_SIZE aSize, OS_UINT32 aTag );
OS_DECL_EXTERN OS_VOID   OS_Free( OS_VOID* aAddress );
OS_DECL_EXTERN OS_INT32  OS_Memcmp( const OS_VOID* aBuffer1, const OS_VOID* aBuffer2, OS_SIZE aSize );
OS_DECL_EXTERN OS_VOID   OS_Memset( OS_VOID* aAddress, OS_UINT32 aValue, OS_SIZE aSize );
OS_DECL_EXTERN OS_VOID   OS_Memcpy( OS_VOID* aDestination, const OS_VOID* aSource, OS_SIZE aSize );
OS_DECL_EXTERN OS_UINT32 OS_MemcpyToUser( OS_VOID* aTo, const OS_VOID* aFrom, OS_SIZE aSize );
OS_DECL_EXTERN OS_UINT32 OS_MemcpyFromUser( OS_VOID* aTo, const OS_VOID* aFrom, OS_SIZE aSize );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_MEMORY_H__
