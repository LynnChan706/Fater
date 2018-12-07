// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_TIME_H__ 
#define __OS_TIME_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "OS_KMNTime.h"
#else
#endif // _PT_NDIS_
#else
#include "OS_UMWTime.h"
#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "OS_KMMTime.h"
#else
#include "OS_UMMTime.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "OS_KMLTime.h"
#else
#include "OS_UMPOSIXTime.h"
#endif //_PT_KERNEL_
#endif // __linux

#endif // __OS_TIME_H__
