// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KMGEN_SHAREMEMORY_H__
#define __OS_KMGEN_SHAREMEMORY_H__

#include "OS_Include.h"

#include "OS_Compiler.h"
#include "OS_Lock.h"
#include "OS_LinkList.h"
#include "OS_Misc.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_SHAREMEMORYMAXNAMESIZE   32

typedef struct
{
    OS_LINKLIST         List;
    OS_CHAR             Name[ OS_SHAREMEMORYMAXNAMESIZE ];
    OS_LOCK             Lock;
    OS_UINT32           UseCount;
}
OS_SHAREMEMORY_INTERNAL;

typedef struct 
{
    OS_SHAREMEMORY_INTERNAL*    Item;
} OS_SHAREMEMORY;

typedef OS_VOID(*OS_SHAREDMEMORYINITCALLBACK)(OS_UINT8* aPtr);

OS_DECL_EXTERN OS_RESULT OS_ShareMemoryStaticInit( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_ShareMemoryStaticClose( OS_VOID );
OS_DECL_EXTERN OS_RESULT OS_ShareMemoryInit( OS_DRV_CONTEXT* aContext, OS_SHAREMEMORY* aShareMemory,
                                        OS_CHAR* aName, OS_UINT32 aNameSize, OS_SIZE aSize, OS_UINT8** aPtr,
                                        OS_SHAREDMEMORYINITCALLBACK aCallback );
OS_DECL_EXTERN OS_RESULT OS_ShareMemoryFree( OS_SHAREMEMORY* aShareMemory );

///
/// \brief Lock the access to the shared memory
///
/// \param aShareMemory OS_SHAREMEMORY Pointer to the initialized shared memory region
/// \param aTag OS_TAG to be able to debug deadlock
/// \param aFlag OS_LOCK_FLAG Pointer to the flag state to hold the context of the lock
/// \param aDprLevel OS_UINT32 Current DPR level
#define OS_SHAREMEMORYLOCK( aShareMemory, aTag, aFlag, aDprLevel )              \
    OS_LockAcquire( &( aShareMemory )->Item->Lock, ( aTag ), ( aFlag ), ( aDprLevel ) );

///
/// \brief Unlock the access to the shared memory
///
/// \param aShareMemory OS_SHAREMEMORY Pointer to the initialized shared memory region
/// \param aFlag OS_LOCK_FLAG Pointer to the flag state populate during the call of OS_SHAREMEMORYLOCK
/// \param aDprLevel OS_UINT32 Current DPR level
#define OS_SHAREMEMORYUNLOCK( aShareMemory, aFlag, aDprLevel )                  \
    OS_LockRelease( &( aShareMemory )->Item->Lock, ( aFlag ), ( aDprLevel ) );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KMGEN_SHAREMEMORY_H__
