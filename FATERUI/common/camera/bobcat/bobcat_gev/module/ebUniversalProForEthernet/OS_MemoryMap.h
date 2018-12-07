// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_MAPMEMORY_H__
#define __OS_MAPMEMORY_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "OS_KMNMemoryMap.h"
#else

#endif // _PT_NDIS_
#else
#include "OS_UMMemoryMap.h"
#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "OS_KMMMemoryMap.h"
#else
#include "OS_UMMemoryMap.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "OS_KMLMemoryMap.h"
#else
#include "OS_UMMemoryMap.h"
#endif // _PT_KERNEL_
#endif // __linux__

#endif // __OS_MAPMEMORY_H__
