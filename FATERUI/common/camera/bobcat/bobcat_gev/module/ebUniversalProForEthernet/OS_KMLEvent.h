// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_EVENT_H__
#define __OS_KML_EVENT_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_EVENT_SIZE 80

typedef struct
{
    OS_UINT8            Space[ OS_EVENT_SIZE ];
    OS_UINT32           Flag;
}
OS_EVENT;

OS_DECL_EXTERN OS_RESULT OS_EventCheck( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_EventInit( OS_EVENT* aEvent );
OS_DECL_EXTERN OS_RESULT OS_EventFree( OS_EVENT* aEvent );
OS_DECL_EXTERN OS_RESULT OS_EventWait( OS_EVENT* aEvent );
OS_DECL_EXTERN OS_RESULT OS_EventWaitWithTimeout( OS_EVENT* aEvent, OS_UINT64 aTimeout );
OS_DECL_EXTERN OS_RESULT OS_EventRaise( OS_EVENT* aEvent );
OS_DECL_EXTERN OS_RESULT OS_EventReset( OS_EVENT* aEvent );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_EVENT_H__
