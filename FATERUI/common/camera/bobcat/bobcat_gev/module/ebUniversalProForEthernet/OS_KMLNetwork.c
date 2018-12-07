// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "OS_KMLNetwork.h"
#include "OS_KMLGCCCompiler.h"

#include <linux/net.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <net/sock.h>

OS_RESULT OS_SocketCreateRawUDPWithIp( OS_SOCKET* aSocket )
{
    struct socket* lSocket;
    struct socket* lSocketTemp;
    int lResult;
    char lOn = 1;
	mm_segment_t lOld_fs;

    lResult = sock_create( AF_INET, SOCK_RAW, IPPROTO_UDP, &lSocketTemp );
	if( unlikely( lResult < 0 || lSocketTemp == 0 ) )
	{
        lSocket = 0;
		return OS_RESULT_FAIL;
	}
    // Keep the socket for later reference
    aSocket->Space = ( OS_VOID* ) lSocketTemp;
    lSocket = ( struct socket* ) aSocket->Space;

	lOld_fs = get_fs();
	set_fs( KERNEL_DS );
    lResult = lSocket->ops->setsockopt( lSocket, IPPROTO_IP, IP_HDRINCL, &lOn, sizeof( lOn ) );
	set_fs( lOld_fs );
	if( unlikely( lResult < 0 ) )
	{
        sock_release( lSocket );
        lSocket = 0;
		return OS_RESULT_FAIL;
	}

    return OS_RESULT_OK;
}


OS_UINT32 OS_SocketSend( OS_SOCKET* aSocket, OS_SOCKET_DATA* aData )
{
    struct socket* lSocket = ( struct socket* ) aSocket->Space;
    struct sockaddr_in lSin;
	struct msghdr lMessage;
	struct iovec lIoVec[ OS_MAX_SEND_ITEM ];
    int i;
	OS_UINT32 lReturn;
	mm_segment_t lOld_fs;
	OS_UINT32 lTotalSize;


    // The first element MUST contains the required headers ( Ethernet + IP ) 
    if( unlikely( aData->Items[ 0 ].Size < 34 ) )
    {
        return 0;
    }

    // Extract the ip address of the destination from the packet
    lSin.sin_family = AF_INET;
    lSin.sin_addr.s_addr = * ( ( int* ) ( ( ( char* ) aData->Items[ 0 ].Data ) + 30 ) ) ;

	lMessage.msg_control = NULL;
	lMessage.msg_controllen = 0;
	lMessage.msg_flags = 0;
    lMessage.msg_name = ( void *) &lSin;
	lMessage.msg_namelen = sizeof( lSin );
	lMessage.msg_iov = lIoVec;
	lMessage.msg_iovlen = aData->NumberOfItem;

    // Need to remove the Ethernet header because the socket only require header from IP and down $%$%@#$@
    lIoVec[ 0 ].iov_base = ( void *) ( ( ( char* ) aData->Items[ 0 ].Data ) + 14 );
    lIoVec[ 0 ].iov_len = ( __kernel_size_t) aData->Items[ 0 ].Size - 14;
    lTotalSize = aData->Items[ 0 ].Size - 14;
    for( i = 1; i < aData->NumberOfItem; i++ )
    {
	    lIoVec[ i ].iov_base = ( void *) aData->Items[ i ].Data;
	    lIoVec[ i ].iov_len = ( __kernel_size_t) aData->Items[ i ].Size;
        lTotalSize += aData->Items[ i ].Size;
    }

	lOld_fs = get_fs();
	set_fs( KERNEL_DS );
	lReturn = sock_sendmsg( lSocket, &lMessage, lTotalSize );
	set_fs( lOld_fs );
	
	return lReturn;
}

OS_RESULT OS_SocketClose( OS_SOCKET* aSocket )
{
    struct socket* lSocket = ( struct socket* ) aSocket->Space;

    if( lSocket )
    {
        sock_release( lSocket );
    }

	return OS_RESULT_OK;
}

