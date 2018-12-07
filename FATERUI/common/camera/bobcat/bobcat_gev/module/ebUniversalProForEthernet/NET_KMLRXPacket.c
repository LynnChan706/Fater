
#include "NET_KMLRXPacket.h"

#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/version.h>


OS_RESULT NET_RXPacketInit( OS_DRV_CONTEXT* aContext, NET_RXPACKET* aPacket, OS_VOID* aData, OS_UINT32 aSize, 
    NET_PROTOCOL_TYPE aType, OS_UINT32 aFlag )
{
    struct sk_buff* lSKBuffer = ( struct sk_buff * ) aData;

    OS_UNREFERENCED_PARAMETER( aContext );
    OS_UNREFERENCED_PARAMETER( aType );
    OS_UNREFERENCED_PARAMETER( aSize );

    if( likely( aPacket ) )
    {
	    aPacket->Flag = aFlag;

        // Keep track of the sk_buff
        aPacket->Buffer = aData;
        aPacket->Ethernet = ( NET_HEADER_ETHERNET* ) eth_hdr( lSKBuffer );

        if( likely( aPacket->Ethernet ) )
        {
            switch( OS_NTOH16( aPacket->Ethernet->EthType ) )
            {
            case NET_ETH_TYPE_IPV4:
                aPacket->IP.V4 = ( NET_HEADER_IPV4* ) skb_header_pointer( lSKBuffer, 
                    0, sizeof( NET_HEADER_IPV4 ), &aPacket->StorageIP );
                if( likely( ( aPacket->IP.V4 )->Protocol == NET_PROTOCOL_UDP ) )
                {
                    aPacket->UDP = ( NET_HEADER_UDP* ) skb_header_pointer( lSKBuffer, 
                        ( ( struct iphdr* ) aPacket->IP.V4 )->ihl * 4, sizeof( NET_HEADER_UDP ), &aPacket->StorageUDP );
                    if( likely( aPacket->UDP ) )
                    {
                        aPacket->PayloadSize = OS_NTOH16( aPacket->UDP->Length ) - sizeof( NET_HEADER_UDP );            
                        aPacket->HeaderOffsetForCopy = ( ( struct iphdr* ) aPacket->IP.V4 )->ihl * 4;
                        return OS_RESULT_OK;
                    }
                }
                break;
            case NET_ETH_TYPE_IPV6:
                break;
            default:
                break;
            }
        }
    }
    
    // This is not a UDP packet, so it cannot be process by the DIS layer!!!
    return OS_RESULT_WRONG_DESTINATION;   
}

OS_VOID NET_RXPacketDestroy( NET_RXPACKET* aPacket, OS_UINT32 aDprLevel )
{
    struct sk_buff* lSKBuffer = ( struct sk_buff * ) aPacket->Buffer;

    OS_UNREFERENCED_PARAMETER( aDprLevel );

    kfree_skb( lSKBuffer );
}


