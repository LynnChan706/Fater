// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __NET_RXPACKET_H__
#define __NET_RXPACKET_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "NET_KMNRXPacket.h"
#else
#endif // _PT_NDIS_
#else
#include "NET_UMRXPacket.h"
#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "NET_KMMRXPacket.h"
#else
#include "NET_UMRXPacket.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "NET_KMLRXPacket.h"
#else
#include "NET_UMRXPacket.h"
#endif // _PT_KERNEL_
#endif // __linux__

#endif // __NET_RXPACKET_H__
