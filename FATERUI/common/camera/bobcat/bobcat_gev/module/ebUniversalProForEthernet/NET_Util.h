// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __NET_UTIL_H__
#define __NET_UTIL_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "NET_KMNUtil.h"
#else
#endif // _PT_NDIS_
#else
#include "NET_UMUtil.h"
#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "NET_KMMUtil.h"
#else
#include "NET_UMUtil.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "NET_KMLUtil.h"
#else
#include "NET_UMUtil.h"
#endif // _PT_KERNEL_
#endif // __linux__

#endif // __NET_UTIL_H__
