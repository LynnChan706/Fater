// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_TIME_H__
#define __OS_KML_TIME_H__

#include "OS_Compiler.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

OS_DECL_EXTERN OS_VOID   OS_TimeInit( OS_VOID );
OS_DECL_EXTERN OS_UINT64 OS_TimeGetUS( OS_VOID );
OS_DECL_EXTERN OS_UINT64 OS_TimeGetPreciseUS( OS_VOID );
OS_DECL_EXTERN OS_VOID   OS_TimeYield( OS_VOID );
OS_DECL_EXTERN OS_VOID   OS_TimeSleep( OS_UINT32 aTime );
OS_DECL_EXTERN OS_VOID   OS_TimeSinceEpoch( OS_UINT32* aSecond, OS_UINT32* aMicroSecond );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_TIME_H__
