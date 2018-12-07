// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KMLGCCCOMPILER_H__
#define __OS_KMLGCCCOMPILER_H__

#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_DECL_EXTERN           extern

#define OS_DECL_INLINE           static inline

#define OS_DECL_EXPORTABLE       OS_DECL_EXTERN

#if ( defined( _PT_64_ ) || defined( __LP64__ ) )

#define OS_DEFAULT_PACK     16

OS_DECL_INLINE OS_UINT64 OS_DIV64( OS_UINT64 a, OS_UINT64 b )
{
    return a / b;
}
OS_DECL_INLINE OS_UINT64 OS_MOD64( OS_UINT64 a, OS_UINT64 b )
{
    return a % b;
}
#else

#define OS_DEFAULT_PACK     8

OS_DECL_EXTERN OS_UINT64 OS_DIV64( OS_UINT64 a, OS_UINT64 b );
OS_DECL_EXTERN OS_UINT64 OS_MOD64( OS_UINT64 a, OS_UINT64 b );

#endif 

//#if defined( __LITTLE_ENDIAN__ )
#  define OS_IS_LITTLE_ENDIAN 1
#  define OS_IS_BIG_ENDIAN    0
//#else
//#  define OS_IS_BIG_ENDIAN    1
//#  define OS_IS_LITTLE_ENDIAN 0
//#endif

// Bug in gcc, some platform like x64 doesn't define __builtin_bswap16
// in some version
#ifdef __builtin_bswap16
#define OS_BYTES_SWAP16( aValue ) __builtin_bswap16( aValue )
#else
#define OS_BYTES_SWAP16( aValue ) ( ( ( aValue & 0xFF00 ) >> 8 ) | ( ( aValue & 0x00FF ) << 8 ) )
#endif
#define OS_BYTES_SWAP32( aValue ) __builtin_bswap32( aValue )
#define OS_BYTES_SWAP64( aValue ) __builtin_bswap64( aValue )

OS_DECL_EXTERN OS_VOID OS_Prink( const char* aFormat, ... );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KMLGCCCOMPILER_H__
