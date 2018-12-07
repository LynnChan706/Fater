
#include "NET_KMLUtil.h"

#include <linux/skbuff.h>

OS_UINT32 NET_GetPacketSize( OS_VOID* aData )
{
    struct sk_buff* lSKBuffer = ( struct sk_buff * ) aData;

    return ( OS_UINT32 ) lSKBuffer->len;
}

OS_VOID NET_CopyToMemory( OS_VOID* aSource, OS_UINT32 aSourceOffset, OS_VOID* aDestination, OS_UINT32 aAmountOfDataToCopy )
{
    struct sk_buff* lSKBuffer = ( struct sk_buff * ) aSource;

	if( ( aSourceOffset + aAmountOfDataToCopy ) > lSKBuffer->len )
	{
		aAmountOfDataToCopy = lSKBuffer->len - aSourceOffset;
	}

    skb_copy_bits( lSKBuffer, aSourceOffset, aDestination, aAmountOfDataToCopy );  
}

OS_VOID NET_CopyToMemoryMap( OS_VOID* aSource, OS_UINT32 aSourceOffset, OS_MEMORYMAP* aDestination,
    OS_UINT32 aDestinationOffset, OS_UINT32 aAmountOfDataToCopy )
{
    struct sk_buff* lSKBuffer = ( struct sk_buff * ) aSource;
    OS_UINT8* lDestination = ( OS_UINT8* ) OS_MapGetKernelAddress( aDestination );

	if( ( aSourceOffset + aAmountOfDataToCopy ) > lSKBuffer->len )
	{
		aAmountOfDataToCopy = lSKBuffer->len - aSourceOffset;
	}

    skb_copy_bits( lSKBuffer, aSourceOffset, lDestination + aDestinationOffset, aAmountOfDataToCopy ); 
}
			
