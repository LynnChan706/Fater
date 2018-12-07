// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KMN_RWLOCK_H__
#define __OS_KMN_RWLOCK_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Tag.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef unsigned long OS_RWLOCK_FLAG;

#define OS_RWLOCK_SIZE 64

typedef struct
{
    OS_UINT8 Space[ OS_RWLOCK_SIZE ];
#ifdef OS_TAG_ENABLED
    OS_TAG   Tag; 
#endif // OS_TAG_ENABLED
} OS_RWLOCK;

OS_DECL_EXTERN OS_RESULT OS_RWLockCheck( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_RWLockInit( OS_RWLOCK* aLock );
OS_DECL_EXTERN OS_RESULT OS_RWLockFree( OS_RWLOCK* aLock );
OS_DECL_EXTERN OS_VOID OS_RWLockWriteAcquire( OS_RWLOCK* aLock, OS_TAG aTag, OS_RWLOCK_FLAG* aFlag );
OS_DECL_EXTERN OS_VOID OS_RWLockWriteRelease( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag );
OS_DECL_EXTERN OS_VOID OS_RWLockReadAcquire( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag );
OS_DECL_EXTERN OS_VOID OS_RWLockReadRelease( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag );
OS_DECL_EXTERN OS_VOID OS_RWLockConvertWriteToRead( OS_RWLOCK* aLock, OS_RWLOCK_FLAG* aFlag );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KMN_RWLOCK_H__
