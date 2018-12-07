// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_TYPES_H__
#define __OS_KML_TYPES_H__


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef char                OS_INT8;
typedef unsigned char       OS_UINT8;

typedef short               OS_INT16;
typedef unsigned short      OS_UINT16;

typedef int         		OS_INT32;
typedef unsigned int        OS_UINT32;

typedef long long           OS_INT64;
typedef unsigned long long  OS_UINT64;

typedef void            	OS_VOID;
typedef char            	OS_CHAR;
typedef int             	OS_SIZE;

typedef union 
{
    struct 
    {
        OS_UINT32 LowPart;
        OS_UINT32 HighPart;
    };
    OS_UINT64 Address;
} OS_COMPATIBLE_PTR;

#if defined( __LP64__ ) || defined( _PT_64_ )
#define OS_GET_COMPATIBLE_PTR( aDestination, aDestinationType, aSource )  { aDestination = ( aDestinationType* ) ( aSource ).Address; }
#define OS_SET_COMPATIBLE_PTR( aDestination, aSource )                    { ( aDestination ).Address = ( OS_UINT64 ) aSource; }
#else
#define OS_GET_COMPATIBLE_PTR( aDestination, aDestinationType, aSource )  { aDestination = ( aDestinationType* ) ( aSource ).LowPart; }
#define OS_SET_COMPATIBLE_PTR( aDestination, aSource )                    { ( aDestination ).LowPart = ( OS_UINT32 ) aSource; }
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_TYPES_H__
