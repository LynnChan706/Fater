// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
#include "OS_KMLGCCCompiler.h"
#include "OS_KMLSystem.h"
#include "OS_String.h"
#include "OS_Time.h"

#include <asm/cputime.h>
#include "linux/kernel.h"
#include <linux/kernel_stat.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/version.h>

#ifdef OS_SUPPORT_CPU_USAGE
#define OS_SYSTEM_TIMER_PERIOD          100
#define OS_SYSTEM_MAX_CPU               64   
#endif // OS_SUPPORT_CPU_USAGE

#define OS_MAX_VERSION_NUMBER_STRING    32 

static OS_CHAR sVersionNumberAsString[ OS_MAX_VERSION_NUMBER_STRING ];
#ifdef OS_SUPPORT_CPU_USAGE
static spinlock_t sSystemCpuLoadLock = SPIN_LOCK_UNLOCKED;
struct timer_list sSystemCpuLoadTimer;
static OS_UINT32  sSystemCpuEnabled = 0;
static cputime64_t sIdle[ OS_SYSTEM_MAX_CPU ];
static cputime64_t sTotal[ OS_SYSTEM_MAX_CPU ];
static OS_UINT32   sUsage[ OS_SYSTEM_MAX_CPU ];
#endif // OS_SUPPORT_CPU_USAGE

#ifdef OS_SUPPORT_CPU_USAGE
static void OS_SystemTimerRun( unsigned long aData )
{
    cputime64_t lIdle;
    cputime64_t lTotal;
    OS_UINT32   lUsage;
    cputime64_t lDiffIdle;
    cputime64_t lDiffTotal;
    unsigned long i;
    unsigned long lFlag;

    for_each_present_cpu( i )
    {
        // Read the idle time for this CPU
        lIdle = kstat_cpu( i ).cpustat.idle;
#ifdef arch_idle_time
        lIdle += arch_idle_time( i );
#endif
        // Compute the total usage
        lTotal = kstat_cpu( i ).cpustat.user + kstat_cpu( i ).cpustat.nice;
        lTotal += kstat_cpu( i ).cpustat.system;
        lTotal += kstat_cpu( i ).cpustat.softirq;
        lTotal += kstat_cpu( i ).cpustat.irq;
        lTotal += lIdle;
        lTotal += kstat_cpu( i ).cpustat.iowait;
        lTotal += kstat_cpu( i ).cpustat.steal;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24)
        lTotal += kstat_cpu( i ).cpustat.guest;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33)
		lTotal += kstat_cpu( i ).cpustat.guest_nice;
#endif
        // Compute the Cpu usage
        lDiffIdle = lIdle - sIdle[ i ];
        lDiffTotal = lTotal - sTotal[ i ];
        if( likely( lDiffTotal ) )
        {
            lUsage = ( OS_UINT32 ) OS_DIV64( 1000 * OS_DIV64( lDiffTotal - lDiffIdle, lDiffTotal ) + 5, 10 );

            // Update the internal number
            spin_lock_irqsave( &sSystemCpuLoadLock, lFlag );
            sUsage[ i ] = lUsage;
            spin_unlock_irqrestore( &sSystemCpuLoadLock, lFlag );
        }

        // Keep value for the next iteration
        sIdle[ i ] = lIdle;
        sTotal[ i ] = lTotal;
    }   

    // Prepare the next execution of the timer
    sSystemCpuLoadTimer.expires += msecs_to_jiffies( OS_SYSTEM_TIMER_PERIOD );

    // reschedule the next execution
    add_timer( &sSystemCpuLoadTimer );    
}
#endif // OS_SUPPORT_CPU_USAGE

OS_RESULT OS_SystemStaticInit( OS_UINT32 aEnableCPUUsage )
{
#ifdef OS_SUPPORT_CPU_USAGE
	int lSizeCpuUsageStatCurrent = sizeof( struct cpu_usage_stat );
	int lSizeCpuUsageStatExpected = 8 * sizeof( cputime64_t );

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24)
	lSizeCpuUsageStatExpected += sizeof( cputime64_t ); // addition of guest
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33)
	lSizeCpuUsageStatExpected += sizeof( cputime64_t ); // addition of guest_nice
#endif

    if( unlikely( num_possible_cpus() > OS_SYSTEM_MAX_CPU ) )
    {
        printk( "OS_System requires more memory allocation for cpu variables. Increase OS_SYSTEM_MAX_CPU." );
        return OS_RESULT_FAIL;    
    }
	if( unlikely( lSizeCpuUsageStatExpected != lSizeCpuUsageStatCurrent ) )
	{
        printk( "This kernel version has new fields in the structure cpu_usage_stat from file linux/kernel_stat.h." );
		printk( "This is causing the computation of the cpu usage problems and the new fields" 
				"must be added in the function OS_SystemTimerRun to keep using this module" );
        return OS_RESULT_FAIL;   
	}

    memset( sIdle, 0, OS_SYSTEM_MAX_CPU * sizeof( cputime64_t ) );
    memset( sTotal, 0, OS_SYSTEM_MAX_CPU * sizeof( cputime64_t ) );
    memset( sUsage, 0, OS_SYSTEM_MAX_CPU * sizeof( OS_UINT32 ) );
    
    // Setup a timer to monitor the system
    memset( &sSystemCpuLoadTimer, 0, sizeof ( struct timer_list ) );

    if( aEnableCPUUsage )
    {
        init_timer( &sSystemCpuLoadTimer );

        sSystemCpuLoadTimer.function = OS_SystemTimerRun; 
        sSystemCpuLoadTimer.data = 0;
        sSystemCpuLoadTimer.expires = jiffies + msecs_to_jiffies( OS_SYSTEM_TIMER_PERIOD );
    
        sSystemCpuEnabled = 1;
        add_timer( &sSystemCpuLoadTimer );
    }
#endif // OS_SUPPORT_CPU_USAGE
    
#ifndef UTS_RELEASE
    OS_StringFormat( sVersionNumberAsString, OS_MAX_VERSION_NUMBER_STRING, "%d.%d.%d", 
        ( LINUX_VERSION_CODE >> 16 ) & 0x000000FF, ( LINUX_VERSION_CODE >> 8 ) & 0x000000FF, 
        ( LINUX_VERSION_CODE       ) & 0x000000FF );
#endif // UTS_RELEASE    

    return OS_RESULT_OK;
}

OS_RESULT OS_SystemStaticClose( OS_VOID )
{
#ifdef OS_SUPPORT_CPU_USAGE
	if( sSystemCpuEnabled )
    {
        del_timer_sync( &sSystemCpuLoadTimer );
    }
#endif // OS_SUPPORT_CPU_USAGE   
	return OS_RESULT_OK;
}

OS_UINT32 OS_SystemVersion( OS_VOID )
{
    return ( OS_UINT32 ) LINUX_VERSION_CODE;
}

OS_CHAR* OS_SystemVersionAsString( OS_VOID )
{
#ifdef UTS_RELEASE
    return UTS_RELEASE;
#else
    return sVersionNumberAsString;
#endif // UTS_RELEASE
}

OS_UINT32 OS_SystemNumberOfCpu( OS_VOID )
{
    return ( OS_UINT32 ) num_present_cpus();
}

OS_UINT32 OS_SystemCurrentCpuIndex( OS_VOID )
{
    return ( OS_UINT32 ) get_cpu();
}

OS_UINT32 OS_SystemCpuUsage( OS_UINT32 aCpuIndex )
{
#ifdef OS_SUPPORT_CPU_USAGE
    OS_UINT32 lUsage;
    unsigned long lFlag;

    spin_lock_irqsave( &sSystemCpuLoadLock, lFlag );
    lUsage = sUsage[ aCpuIndex ];
    spin_unlock_irqrestore( &sSystemCpuLoadLock, lFlag );

    return lUsage;
#else
    return 0;    
#endif // OS_SUPPORT_CPU_USAGE      
}

