// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifndef __OS_KML_NETWORK_H__
#define __OS_KML_NETWORK_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct
{
    OS_VOID*  Data;
    OS_UINT32 Size;
}
OS_SOCKETDATAITEM;

#define OS_MAX_SEND_ITEM       3
typedef struct
{
    OS_SOCKETDATAITEM   Items[ OS_MAX_SEND_ITEM ];
    OS_UINT32           NumberOfItem;
}
OS_SOCKET_DATA;

typedef struct
{
    OS_VOID*    Space;
} 
OS_SOCKET;

OS_DECL_EXTERN OS_RESULT OS_SocketCreateRawUDPWithIp( OS_SOCKET* aSocket );
OS_DECL_EXTERN OS_UINT32 OS_SocketSend( OS_SOCKET* aSocket, OS_SOCKET_DATA* aData );
OS_DECL_EXTERN OS_RESULT OS_SocketClose( OS_SOCKET* aSocket );

#if OS_IS_BIG_ENDIAN

#define OS_HTON16( aValue ) ( aValue )
#define OS_NTOH16( aValue ) ( aValue )
#define OS_HTON32( aValue ) ( aValue )
#define OS_NTOH32( aValue ) ( aValue )
#define OS_HTON64( aValue ) ( aValue )
#define OS_NTOH64( aValue ) ( aValue )

#else

#define OS_HTON16( aValue ) OS_BYTES_SWAP16( aValue )
#define OS_NTOH16( aValue ) OS_BYTES_SWAP16( aValue )
#define OS_HTON32( aValue ) OS_BYTES_SWAP32( aValue )
#define OS_NTOH32( aValue ) OS_BYTES_SWAP32( aValue )
#define OS_HTON64( aValue ) OS_BYTES_SWAP64( aValue )
#define OS_NTOH64( aValue ) OS_BYTES_SWAP64( aValue )

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_NETWORK_H__
