// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_TIMER_H__
#define __OS_KML_TIMER_H__

#include "OS_Compiler.h"
#include "OS_Misc.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef OS_VOID(*OS_TIMER_FNC)(OS_VOID* aData);

#define OS_TIMER_SIZE 128

typedef struct
{
    OS_UINT8            Space[ OS_TIMER_SIZE ];
    OS_TIMER_FNC	    Function;
	OS_VOID*	        Data;
    OS_UINT32           Period;
} OS_TIMER;

OS_DECL_EXTERN OS_RESULT OS_TimerCheck( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_TimerCreate( OS_DRV_CONTEXT* aContext, OS_TIMER* aTimer, OS_UINT32 aPeriodic,
    OS_TIMER_FNC aFunction, OS_VOID* aData, OS_UINT32 aPeriod /* ms */ );
OS_DECL_EXTERN OS_RESULT OS_TimerDestroy( OS_TIMER* aTimer ); 

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_TIMER_H__
