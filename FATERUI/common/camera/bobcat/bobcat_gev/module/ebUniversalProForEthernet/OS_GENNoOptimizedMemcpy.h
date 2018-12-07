
#ifndef __OS_NOOPTIMIZEDMEMCPY_H__
#define __OS_NOOPTIMIZEDMEMCPY_H__

#include "OS_Result.h"
#include "OS_Memory.h"
#include "OS_Misc.h"
#include "OS_Include.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define OS_OPTIMIZED_MEMCPY_CONTEXT OS_UINT32

OS_DECL_INLINE OS_UINT32 OS_OptimizedMemoryConfigure()
{
    return OS_RESULT_OK; 
}

OS_DECL_INLINE OS_RESULT OS_OptimizedMemcpyInit( OS_OPTIMIZED_MEMCPY_CONTEXT* aContext )
{
    OS_UNREFERENCED_PARAMETER( aContext );
    return OS_RESULT_OK; 
}

OS_DECL_INLINE OS_VOID OS_OptimizedMemcpyClose( OS_OPTIMIZED_MEMCPY_CONTEXT* aContext )
{
    OS_UNREFERENCED_PARAMETER( aContext );
}

OS_DECL_INLINE OS_VOID OS_OptimizedMemcpy( OS_VOID* aDestination, const OS_VOID* aSource, OS_SIZE aSize )
{
    OS_Memcpy( aDestination, aSource, aSize );
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_NOOPTIMIZEDMEMCPY_H__

