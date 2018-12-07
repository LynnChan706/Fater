// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_LOCK_H__
#define __OS_KML_LOCK_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Tag.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef unsigned long OS_LOCK_FLAG;

#define OS_SPINLOCK_SIZE 64

typedef struct
{
    OS_UINT8 Space[ OS_SPINLOCK_SIZE ];
#ifdef OS_TAG_ENABLED
    OS_TAG   Tag; 
#endif // OS_TAG_ENABLED
} OS_LOCK;

OS_DECL_EXTERN OS_RESULT OS_LockCheck( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_LockInit( OS_LOCK* aLock );
OS_DECL_EXTERN OS_RESULT OS_LockFree( OS_LOCK* aLock );
OS_DECL_EXTERN OS_RESULT OS_LockAcquire( OS_LOCK* aLock, OS_TAG aTag, OS_LOCK_FLAG* aFlag, OS_UINT32 aDprLevel );
OS_DECL_EXTERN OS_RESULT OS_LockRelease( OS_LOCK* aLock, OS_LOCK_FLAG* aFlag, OS_UINT32 aDprLevel );
OS_DECL_EXTERN OS_VOID OS_LockIncreaseInt32( OS_INT32* aValue, OS_LOCK* aLock, OS_UINT32 aDprLevel );
OS_DECL_EXTERN OS_VOID OS_LockDecreaseInt32( OS_INT32* aValue, OS_LOCK* aLock, OS_UINT32 aDprLevel );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_LOCK_H__
