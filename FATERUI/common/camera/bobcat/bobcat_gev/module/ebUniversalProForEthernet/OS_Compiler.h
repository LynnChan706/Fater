// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_COMPILER_H__
#define __OS_COMPILER_H__

#if defined( WIN32 )
#if defined( _PT_KERNEL_ )
#ifdef _PT_NDIS_
#include "OS_KMNCompiler.h"
#else
#endif // _PT_NDIS_
#else
#include "OS_UMWCompiler.h"
#endif // _PT_KERNEL_
#endif // WIN32

#if defined( __APPLE__ )
#if defined( KERNEL )
#include "OS_KMMCompiler.h"
#else
#include "OS_UMGCCCompiler.h"
#endif // KERNEL
#endif // __APPLE__

#if defined( __linux__ )
#if defined( _PT_KERNEL_ )
#include "OS_KMLGCCCompiler.h"
#else
#include "OS_UMGCCCompiler.h"
#endif // _PT_KERNEL_
#endif // __linux__


#endif // __OS_COMPILER_H__
