
#ifndef __OS_OPTIMIZEDMEMCPY_H__
#define __OS_OPTIMIZEDMEMCPY_H__

#if defined(_WIN32)
#ifdef OS_SUPPORT_OPTIMIZED_MEMCPY
#include "OS_WINOptimizedMemcpy.h"
#else
#include "OS_GENNoOptimizedMemcpy.h"
#endif
#endif // _WIN32

#if defined (_UNIX_)
#include "OS_GENNoOptimizedMemcpy.h"
#endif // _UNIX_

#endif // __OS_OPTIMIZEDMEMCPY_H__
