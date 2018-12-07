// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
 
#ifndef __OS_GEN_MEMORYPOOL_H__
#define __OS_GEN_MEMORYPOOL_H__

#include "OS_Compiler.h"
#include "OS_LinkList.h"
#include "OS_Misc.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct   
{
    OS_VOID**     Free;
    OS_VOID*      Storage;
    OS_UINT32     CurrentIndex;
    OS_UINT32     Size;
} OS_MEMORYPOOL;

OS_DECL_EXTERN OS_RESULT OS_MemoryPoolCreate( OS_DRV_CONTEXT* aContext, OS_MEMORYPOOL* aMemPool, OS_UINT32 aNumElement, OS_SIZE aElementSize );
OS_DECL_EXTERN OS_VOID OS_MemoryPoolDestroy( OS_MEMORYPOOL* aMemPool );

///
/// \brief Check how many element are pre-allocated in the memory pool
/// 
/// \param aMemPool OS_MEMORYPOOL Pointer to the Initialize memory pool
///
#define OS_MEMORYPOOL_GETUSECOUNT( aMemPool )      ( ( aMemPool )->Size - ( aMemPool )->CurrentIndex )

///
/// \brief Check if any elements are avaible
/// 
/// \param aMemPool OS_MEMORYPOOL Pointer to the Initialize memory pool
///
#define OS_MEMORYPOOL_ISAVAILABLE( aMemPool )      ( ( aMemPool )->CurrentIndex )

///
/// \brief Get a memory element from the memory pool
/// 
/// \param aMemPool OS_MEMORYPOOL Pointer to the Initialize memory pool
/// \param aElement OS_VOID Pointer used to return the new element, 0 on error
///
#define OS_MEMORYPOOL_ALLOC( aMemPool, aElement )                               \
{                                                                               \
    ( aElement ) = 0;                                                           \
    if ( OS_MEMORYPOOL_ISAVAILABLE( aMemPool ) )                                \
    {                                                                           \
        ( aMemPool )->CurrentIndex--;                                           \
        ( aElement ) = *( ( aMemPool )->Free + ( aMemPool )->CurrentIndex );    \
    }                                                                           \
}

///
/// \brief Return a memory element from the memory pool
/// 
/// \param aMemPool OS_MEMORYPOOL Pointer to the Initialize memory pool
/// \param aElement OS_VOID* Pointer on the element to be free
///
#define OS_MEMORYPOOL_FREE( aMemPool, aElement )                                \
{                                                                               \
    *( ( aMemPool )->Free + ( aMemPool )->CurrentIndex ) = aElement;            \
    ( aMemPool )->CurrentIndex++;                                               \
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_GEN_MEMORYPOOL_H__
