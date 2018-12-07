// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "OS_KMNTypes.h"
#else
#endif // _PT_NDIS_
#else
#include "OS_UMWTypes.h"
#endif // _PT_KERNEL_
#endif // WIN32


#if defined( __APPLE__ )
#if defined( KERNEL )
#include "OS_KMMTypes.h"
#else
#include "OS_UMGCCTypes.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "OS_KMLTypes.h"
#else
#include "OS_UMGCCTypes.h"
#endif //_PT_KERNEL_
#endif // __linux

#endif // __OS_TYPES_H__
