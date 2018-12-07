// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_WORKQUEUE_H__
#define __OS_WORKQUEUE_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_

#else
#endif // _PT_NDIS_
#else

#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "OS_KMMWorkQueue.h"
#else

#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "OS_KMLWorkQueue.h"
#else

#endif //_PT_KERNEL_
#endif // __linux__

#endif // __OS_WORKQUEUE_H__

