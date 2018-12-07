// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __NET_KMLRXPACKET_H__
#define __NET_KMLRXPACKET_H__

#include "OS_Compiler.h"
#include "OS_LinkList.h"
#include "OS_Memory.h"
#include "OS_Network.h"
#include "OS_Types.h"

#include "NET_Types.h"
#include "NET_Util.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/// 
/// \file This is the definition of a packet for a user mode data receiver
/// 

// Packet description
typedef struct
{
	// Packet management information
    OS_LINKLIST             List;
    OS_UINT32               Flag;
    OS_VOID*                Manager;
    OS_UINT32               ReferenceCount;

    // Internal information
    OS_VOID*                Buffer;

    // Quick access to the headers
    NET_HEADER_ETHERNET*    Ethernet;
    NET_HEADER_IP 	        IP;
    NET_HEADER_UDP* 	    UDP;

    // Storage for linearization of the headers
    NET_HEADER_IPV6         StorageIP;
    NET_HEADER_UDP          StorageUDP;

    // Cached information
    OS_UINT32               PayloadSize;     // Host order
    OS_UINT32               HeaderOffsetForCopy;

    // This allow any plug-in to add its own member fields to the packet and keep 
    // it during the life of the packet
    OS_CHAR                 Custom[ NET_RXPACKET_CUSTOM ];
} NET_RXPACKET;

/// 
/// \brief Initialize the Packet 
/// This is a function used by the packet manager at allocation time
/// \param aContext Context of the driver allocating the memory
/// \param aPacket Pre-Allocated Packet from the Manager
/// \param aData Data Pointer passed by the manager
/// \param aSize Size of aData 
/// \param aType Protocol type of the packet is know by the manager
/// \param aFlag Packet flag information
///
/// \return Result state
///
OS_DECL_EXTERN OS_RESULT NET_RXPacketInit( OS_DRV_CONTEXT* aContext, NET_RXPACKET* aPacket, OS_VOID* aData, OS_UINT32 aSize,
    NET_PROTOCOL_TYPE aType, OS_UINT32 aFlag );

/// 
/// \brief Free the NDIS packets
/// We can only free a packet if the resource flag is not set...
/// because in this case, the filter need to release the memory in the context
/// of the hook call
///
/// \param aPacket Packet to be freeed
/// \param aDprLevel DPR level flag for Windows Driver
///
/// \note The packet should not be freeed when the flag NET_LOW_RESOURCE is set because
/// the driver cannot pend or own this memory. 
///
OS_DECL_EXTERN OS_VOID NET_RXPacketDestroy( NET_RXPACKET* aPacket, OS_UINT32 aDprLevel );

/// 
/// \brief Get the payload size out of the packet 
///
/// \param aPacket NET_RXPACKET Pointer Packet to look at
/// \return OS_UINT32 UDP Payload Size in host byte ordering
///
#define NET_RXPACKETGETPAYLOADSIZE( aPacket )                                   \
    ( ( aPacket )->PayloadSize )

/// 
/// \brief Get the Source UDP port of the packet
///
/// \param aPacket NET_RXPACKET Pointer Packet to look at
/// \return OS_UINT32 Port number in network byte ordering
///
#define NET_RXPACKETGETUDPPORT( aPacket )                                       \
    OS_NTOH16( ( aPacket )->UDP->DestinationPort )

///
/// \brief Copy function. From a Ipv4 packet to a Memory destination
///
/// \param aSource Pre-Allocated Packet from Manager
/// \param aSourceOffset Offset after the UDP header to skip in the packet
/// \param aDestination Memory address destination ( should be at least aDestinationOffset bytes )
/// \param aDestination Offset Offset to be added to the beginning of the Memory address destination
/// \param aAmountOfDataToCopy Amount of bytes to be copied
///
#define NET_RXPACKETCOPYTOMEMORY( aSource, aSourceOffset, aDestination, aDestinationOffset, aAmountOfDataToCopy )               \
    NET_CopyToMemory( ( aSource )->Buffer, ( aSourceOffset ) + ( aSource )->HeaderOffsetForCopy + sizeof( NET_HEADER_UDP ),     \
    ( ( OS_UINT8* ) ( aDestination ) ) + ( aDestinationOffset ), ( aAmountOfDataToCopy ) )

///
/// \brief Copy function. From a Ipv4 packet to a MemoryMapped destination
///
/// \param aSource Pre-Allocated Packet from Manager
/// \param aSourceOffset Offset after the UDP header to skip in the packet
/// \param aDestination MemoryMap destination ( should be at least aDestinationOffset bytes )
/// \param aDestinationOffset Offset from the beginning in the MemoryMap destination
/// \param aAmountOfDataToCopy Amount of bytes to be copied
///
#define NET_RXPACKETCOPYTOMEMORYMAP( aSource, aSourceOffset, aDestination, aDestinationOffset, aAmountOfDataToCopy )            \
    NET_CopyToMemoryMap( ( aSource )->Buffer, ( aSourceOffset ) + ( aSource )->HeaderOffsetForCopy + sizeof( NET_HEADER_UDP ),  \
        ( aDestination ), ( aDestinationOffset ), ( aAmountOfDataToCopy ) )

/// 
/// \brief Compare function for a packet
/// 
/// \param aPacket NET_RXPACKET Pointer Packet to compare
/// \param aLocalInfo NET_INFO Pointer Compare with information from this local address
/// \param aRemoteInfo NET_INFO Pointer Compare with information from this remote address
/// \return !0 if same
///
#define NET_RXPACKETCOMPARE( aPacket, aLocalInfo, aRemoteInfo )                                                       \
    ( ( OS_NTOH16( ( ( aPacket )->UDP->DestinationPort ) ) == ( aLocalInfo )->Port )                                  \
        && ( OS_NTOH16( ( aPacket )->Ethernet->EthType ) == NET_ETH_TYPE_IPV4 )                                       \
        && ( *( ( OS_UINT32* ) ( ( aPacket )->IP.V4 )->SourceIP ) == *( ( OS_UINT32* ) ( aRemoteInfo )->IP ) )        \
	    && ( *( ( OS_UINT32* ) ( ( aPacket )->IP.V4 )->DestinationIP ) == *( ( OS_UINT32* ) ( aLocalInfo )->IP ) ) )

       /* || (     ( ( aPacket )->Ethernet->EthType == NET_ETH_TYPE_IPV6 )                                                                    \
	        && ( *( ( OS_UINT64* ) ( ( aPacket )->IP.V6 )->SourceIP ) == *( ( OS_UINT64* ) ( aRemoteInfo )->IP )        )                   \
	        && ( *( ( ( OS_UINT64* ) ( ( aPacket )->IP.V6 )->SourceIP ) + 1 ) == *( ( ( OS_UINT64* ) ( aRemoteInfo )->IP ) ) + 1  )         \
        && ( *( ( OS_UINT64* ) ( ( aPacket )->IP.V6 )->DestinationIP ) == *( ( OS_UINT64* ) ( aLocalInfo )->IP )         )                  \
	        && ( *( ( ( OS_UINT64* ) ( ( aPacket )->IP.V6 )->DestinationIP + 1 ) ) == *( ( ( OS_UINT64* ) ( aLocalInfo )->IP ) ) + 1 ) ) ) )*/
	        

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __NET_KMLRXPACKET_H__
