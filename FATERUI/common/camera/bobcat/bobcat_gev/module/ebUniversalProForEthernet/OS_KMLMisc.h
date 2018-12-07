// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_MISC_H__
#define __OS_KML_MISC_H__

#include "OS_KMLGCCCompiler.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_METHOD_BUFFERED 0

// That function codes 0-2047 are reserved for Microsoft, 
// and 2048-4095 are reserved for OEMs and IHVs. 
// The function code can be no larger then 4095.
// We will keep the same limitation in linux!
#define OS_IOCTL_MIN            2048

#define OS_MIN( aExpression1, aExpression2 )                                            \
    ( ( ( aExpression1 ) < ( aExpression2 ) ) ? ( aExpression1 ) : ( aExpression2 ) )
#define OS_MAX( aExpression1, aExpression2 )                                            \
    ( ( ( aExpression1 ) > ( aExpression2 ) ) ? ( aExpression1 ) : ( aExpression2 ) )

#define OS_BUILD_CONTROL_CODE( aCode, aMethod ) ( aCode )
#define OS_EXTRACT_CONTROL_CODE( aCode )        ( aCode ) 

#define OS_UNREFERENCED_PARAMETER( aParam )     ( ( aParam ) = ( aParam ) )

#define OS_CHECK_DISPATCH()
#define OS_CHECK_LESS_THAN_DISPATCH()
#define OS_CHECK_DISPATCH_OR_LESS()
#define OS_CHECK_PASSIVE()

#ifdef _PT_DEBUG_
#define OS_TRACE_DEBUG( aExpression )                                                               \
{                                                                                                   \
    printk( "OS: " );                                                                               \
    printk( aExpression );                                                                          \
    printk( "\n" );                                                                                 \
}

#define OS_ASSERT( aExpression )                                                                    \
{                                                                                                   \
    if ( !aExpression )                                                                             \
    {                                                                                               \
        printk( "OS: ** ASSERT ** " );                                                              \
        printk( aExpression );                                                                      \
        printk( "\n" );                                                                             \
    }                                                                                               \
    assert( aExpression );                                                                          \
}
#else
#define OS_TRACE_DEBUG( aExpression )
#define OS_ASSERT( aExpression )
#endif          

// This is the driver context
typedef long OS_DRV_CONTEXT;

#define OS_OFFSET_OF( aType, aField )                                   \
    __builtin_offsetof( aType, aField )

#define OS_CONTAINER_OF( aPtr, aType, aField )                          \
    ( ( aType * ) ( ( char *) ( aPtr ) - OS_OFFSET_OF( aType, aField ) ) )

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_MISC_H__

