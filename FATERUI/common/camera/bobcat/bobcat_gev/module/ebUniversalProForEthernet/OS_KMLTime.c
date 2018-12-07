
#include "OS_KMLTime.h"
#include "OS_KMLGCCCompiler.h"

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/version.h>

OS_VOID OS_TimeInit( OS_VOID )
{
    // Nothing to do for this OS
}

OS_UINT64 OS_TimeGetUS( OS_VOID )
{
	struct timespec lTime;

	lTime = current_kernel_time();

	return ( lTime.tv_sec * 1000000 + OS_DIV64( lTime.tv_nsec, 1000 ) );
}

OS_UINT64 OS_TimeGetPreciseUS( OS_VOID )
{
	struct timespec lTime;

#ifdef OS_ALLOW_GPL
	getnstimeofday( &lTime );
#else
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
	getnstimeofday( &lTime );
#  else
	lTime = current_kernel_time();
#  endif 
#endif 
	return ( lTime.tv_sec * 1000000 + OS_DIV64( lTime.tv_nsec, 1000 ) );
}

OS_VOID OS_TimeYield( OS_VOID )
{
    yield();
}

OS_VOID OS_TimeSleep( OS_UINT32 aTime )
{
    set_current_state( TASK_INTERRUPTIBLE );
    schedule_timeout( msecs_to_jiffies( aTime ) );
}

OS_VOID OS_TimeSinceEpoch( OS_UINT32* aSecond, OS_UINT32* aMicroSecond )
{
    struct timeval lTimeVal;

    do_gettimeofday( &lTimeVal );
    
    *aSecond = lTimeVal.tv_sec;
    *aMicroSecond = lTimeVal.tv_usec;
}
