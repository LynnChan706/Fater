// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __NET_TYPE_H__
#define __NET_TYPE_H__

#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/// 
/// \file Describe the Ethernet, IP and UDP packet format to be used 
/// independly of the OS
/// 

//
// Ethernet definitions
//

#define NET_MAC_ADDR_LEN                6
#define NET_IPV4_ADDR_LEN               4
#define NET_IPV6_ADDR_LEN               16
#define NET_UDP_PORT_LEN                2

#define NET_ETH_TYPE_IPV4               0x0800
#define NET_ETH_TYPE_IPV6               0x86dd

#define NET_IP_ADDRESS_FAMILY_LEN       1
#define NET_IP_ADDRESS_FAMILY( aBuffer ) ( ( *( ( OS_UINT8* ) ( aBuffer ) ) ) >> 4 )

#define NET_IP_ADDRESS_FAMILY_IP4       4
#define NET_IP_ADDRESS_FAMILY_IP6       6

#define NET_IP_DONTFRAGMENTFLAG         0x4000

#define NET_PROTOCOL_UDP                17

// Flag for packet configutation
// Define by the system when the packet is mark as low resource. This imply a different
// way to handle the packet ownership internal
#define NET_LOW_RESOURCE                0x00000001
#ifdef NDIS51
// Define by the system when a packet is allocated internally in the miniport to wrap the
// incoming packet because their in no remaining slot in the current packet stack
#define NET_MP_GENERATED_PACKET         0x00000002
#endif // NDIS51

#ifdef NDIS51
// To be able to release the packet properly, we need to be able to retrieve the original
// packet pointer from the current packet. 
// IMPORTANT: MUST BE IN SYNCH WITH THE STRUCT WFT_RECV_RSVD IN THE FILE WFT_Miniport.h
typedef struct
{
    PNDIS_PACKET    OriginalPkt;
} NET_RECV_RSVD;
#endif // NDIS51

#define NET_RXPACKET_CUSTOM   256
#define NET_INVALID_PORT    0xFFFFFFFF

// Type of protocol of the address
typedef enum
{
	NET_PROTOCOL_IPV4 = 0,     
	NET_PROTOCOL_IPV6 = 1,   
    NET_PROTOCOL_IP_UNDEFINED
} NET_PROTOCOL_TYPE;

typedef struct
{
	// Define to know what type of function to use for processing
	NET_PROTOCOL_TYPE Type;
	// Address field in network order in network byte ordering
    OS_UINT8 IP[ NET_IPV6_ADDR_LEN ];
    // Ethernet Address
    OS_UINT8 MAC[ NET_MAC_ADDR_LEN ];
    // Port field in network order in network byte ordering
	OS_UINT16 Port;
} NET_INFO;

#pragma pack( push, 1 )
typedef struct
{
    OS_UINT8       DstAddr[ NET_MAC_ADDR_LEN ];
    OS_UINT8       SrcAddr[ NET_MAC_ADDR_LEN ];
    OS_UINT16      EthType;
} NET_HEADER_ETHERNET;
#pragma pack( pop )

#pragma pack( push, 1 )
typedef struct
{
    OS_UINT8       VersionAndHeaderLength;
    OS_UINT8       DifferentiatedServices;
    OS_UINT16      TotalLength;

    OS_UINT16      Identification;
    OS_UINT16      FlagsAndFragmentOffset;

    OS_UINT8       TTL;
    OS_UINT8       Protocol;
    OS_UINT16      HeaderChecksum ;

    OS_UINT8       SourceIP[ NET_IPV4_ADDR_LEN ];

    OS_UINT8       DestinationIP[ NET_IPV4_ADDR_LEN ];
} NET_HEADER_IPV4;
#pragma pack( pop )

#pragma pack( push, 1 )
typedef struct
{
    OS_UINT32 VerTrafficFlow; /* FlowLabel:20, TrafficClass:8, Version:4 */
    OS_UINT16 PayloadLength;
    OS_UINT8  NextHeader;
    OS_UINT8  HopLimit;

    OS_UINT8  SourceIP[ NET_IPV6_ADDR_LEN ];

    OS_UINT8  DestinationIP[ NET_IPV6_ADDR_LEN ];
} NET_HEADER_IPV6;
#pragma pack( pop )

#pragma pack( push, 1 )
typedef struct
{
    OS_UINT16 SourcePort;
    OS_UINT16 DestinationPort;
    OS_UINT16 Length;
    OS_UINT16 Checksum;
} NET_HEADER_UDP;
#pragma pack( pop )

// UDP header stack for IPv4 packets
#pragma pack( push, 1 )
typedef struct
{
	NET_HEADER_ETHERNET  	Ethernet;
    NET_HEADER_IPV4	        IP;
    NET_HEADER_UDP		    UDP;
} NET_HEADERS_V4;
#pragma pack( pop )

// UDP header stack for IPvv packets
#pragma pack( push, 1 )
typedef struct
{
	NET_HEADER_ETHERNET  	Ethernet;
    NET_HEADER_IPV6	        IP;
    NET_HEADER_UDP		    UDP;
} NET_HEADERS_V6;
#pragma pack( pop )

// This is a union for the version of IP
typedef union
{
    NET_HEADER_IPV4* V4;
    NET_HEADER_IPV6* V6;
} NET_HEADER_IP;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __NET_TYPE_H__

