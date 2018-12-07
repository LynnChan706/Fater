// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_SYSTEM_H__
#define __OS_KML_SYSTEM_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

OS_DECL_EXTERN OS_RESULT OS_SystemStaticInit( OS_UINT32 aEnableCPUUsage );
OS_DECL_EXTERN OS_RESULT OS_SystemStaticClose( OS_VOID );
OS_DECL_EXTERN OS_UINT32 OS_SystemVersion( OS_VOID );
OS_DECL_EXTERN OS_CHAR*  OS_SystemVersionAsString( OS_VOID );
OS_DECL_EXTERN OS_UINT32 OS_SystemNumberOfCpu( OS_VOID );
OS_DECL_EXTERN OS_UINT32 OS_SystemCurrentCpuIndex( OS_VOID );
OS_DECL_EXTERN OS_UINT32 OS_SystemCpuUsage( OS_UINT32 aCpuIndex );
 
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_SYSTEM_H__
