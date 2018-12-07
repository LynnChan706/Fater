// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __NET_KMLUTIL_H__
#define __NET_KMLUTIL_H__

#include "OS_Compiler.h"
#include "OS_MemoryMap.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

///
/// \brief Return the size of a packet
///
/// \param aData Data Pointer passed by the manager ( in this OS -> sk_buff )
///
/// \return The size of the packet
///
OS_DECL_EXTERN OS_UINT32 NET_GetPacketSize( OS_VOID* aData );

/// 
/// \brief Copy NET_BUFFER to a continuous memory location
/// 
/// \param aSource Data Pointer passed by the manager ( in this OS -> mbut_t )
/// \param aSourceOffset Offset in the packet to copy from
/// \param aDestination Destination memory area pre-allocated
/// \param aAmountOfDataToCopy Size of the destination
///
OS_DECL_EXTERN OS_VOID NET_CopyToMemory( OS_VOID* aSource, OS_UINT32 aSourceOffset, OS_VOID* aDestination, OS_UINT32 aAmountOfDataToCopy );

///
/// \brief Copy NET_BUFFER to a memory map
///
/// \param aSource Data Pointer passed by the manager ( in this OS -> mbut_t )
/// \param aSourceOffset Offset in the packet to copy from
/// \param aDestination Destination memory map
/// \param aDestinationOffset Destination offset in the memory map
/// \param aAmountOfDataToCopy Size of the destination
OS_DECL_EXTERN OS_VOID NET_CopyToMemoryMap( OS_VOID* aSource, OS_UINT32 aSourceOffset, OS_MEMORYMAP* aDestination,
    OS_UINT32 aDestinationOffset, OS_UINT32 aAmountOfDataToCopy );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __NET_KMLUTIL_H__
