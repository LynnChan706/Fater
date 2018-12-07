// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_WORKQUEUE_H__
#define __OS_KML_WORKQUEUE_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef OS_VOID(*POS_WORKQUEUE_FUNC)(OS_VOID* aData);

typedef struct
{
	POS_WORKQUEUE_FUNC Function;
#ifdef OS_ALLOW_GPL
	OS_VOID* WorkQueue;
#else
    int UseCount;
#endif // OS_ALLOW_GPL
} OS_WORKQUEUE;

#define OS_WORKITEM_SIZE ( 128 )
typedef struct
{
    OS_UINT8 Space[ OS_WORKITEM_SIZE ];
	OS_WORKQUEUE* WorkQueue;
	OS_VOID* Data;
} OS_WORKITEM;

OS_DECL_EXTERN OS_RESULT OS_WorkQueueCheck( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_WorkQueueInit( OS_WORKQUEUE* aWorkQueue, OS_CHAR* aName,
	POS_WORKQUEUE_FUNC aFunction );
OS_DECL_EXTERN OS_RESULT OS_WorkQueueFree( OS_WORKQUEUE* aWorkQueue );
OS_DECL_EXTERN OS_RESULT OS_WorkQueueSchedule( OS_WORKQUEUE* aWorkQueue, 
	OS_WORKITEM* aWorkItem, OS_VOID* aData );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_WORKQUEUE_H__
