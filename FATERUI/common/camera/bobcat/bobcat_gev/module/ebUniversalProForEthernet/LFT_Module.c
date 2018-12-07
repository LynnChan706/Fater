// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************
 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/udp.h>
#include <linux/utsname.h>
#include <linux/version.h>

#include "LFT_Public.h"
#include "LFT_Resources.h"

#include "OS_Init.h"
#include "OS_Result.h"
#include "OS_Types.h"

#define LFT_DEVICE_COUNT    	( 1 )

static dev_t sDevice;
static struct cdev sCharDevice;
static struct nf_hook_ops sNetFilterOperations;

// Parameters
// By default, we want auto generated device node
static unsigned int sParamMajor = 0; 
module_param( sParamMajor, uint, S_IRUGO );
static unsigned int sTxPoolSize = 0; 
module_param( sTxPoolSize, uint, S_IRUGO );

static int LFT_ModuleOpen( struct inode* aNode, struct file* aFile )
{
    aFile->private_data = 0;
    return 0;
}

static int LFT_ModuleRelease( struct inode* aNode, struct file* aFile )
{  
    if( unlikely( aFile->private_data ) )
    {
        LFT_PublicDeviceRelease( ( LFT_HDEVICE** ) &aFile->private_data );
    }
    return 0;
}

#if HAVE_UNLOCKED_IOCTL
static long LFT_ModuleUnlockedIoctl( struct file* aFile, unsigned int aCmd, unsigned long aArg )
#else
static int LFT_ModuleIoctl( struct inode* aNode, struct file* aFile, 
	     unsigned int aCmd, unsigned long aArg )
#endif
{
    OS_RESULT lResult;
    
    lResult = LFT_PublicDeviceIoctl( ( LFT_HDEVICE** ) &aFile->private_data, aCmd, ( OS_VOID* ) aArg );
    
    return ( lResult == OS_RESULT_OK || lResult == OS_RESULT_PENDING ) ? 0 : -1;
}

static struct file_operations sFileOperations = {
    .open            = LFT_ModuleOpen,
    .release         = LFT_ModuleRelease,
#if HAVE_UNLOCKED_IOCTL
    .unlocked_ioctl  = LFT_ModuleUnlockedIoctl 
#else
    .ioctl           = LFT_ModuleIoctl
#endif
};

__attribute__( ( unused ) )
static unsigned int LFT_ModuleHookV1( unsigned int aHookNumber, struct sk_buff** aSKBuffer, const struct net_device* aIn, 
    const struct net_device* aOut, int (*okfn)(struct sk_buff*) )
{
    OS_RESULT lResult;

    lResult = LFT_PublicFilterHook( ( OS_VOID* ) *aSKBuffer );

    return ( lResult == OS_RESULT_OK ) ? NF_STOLEN : NF_ACCEPT;
}

__attribute__( ( unused ) )
static unsigned int LFT_ModuleHookV2( unsigned int aHookNumber, struct sk_buff* aSKBuffer, const struct net_device* aIn, 
    const struct net_device* aOut, int (*okfn)(struct sk_buff*) )
{
    OS_RESULT lResult;
  
    lResult = LFT_PublicFilterHook( ( OS_VOID* ) aSKBuffer );

    return ( lResult == OS_RESULT_OK ) ? NF_STOLEN : NF_ACCEPT;
}

__attribute__( ( unused ) )
static unsigned int LFT_ModuleHookV3( const struct nf_hook_ops* aOperations, struct sk_buff* aSKBuffer, 
    const struct net_device* aIn, const struct net_device* aOut, int (*okfn)(struct sk_buff*) )
{
    OS_RESULT lResult;
  
    lResult = LFT_PublicFilterHook( ( OS_VOID* ) aSKBuffer );

    return ( lResult == OS_RESULT_OK ) ? NF_STOLEN : NF_ACCEPT;
}

static nf_hookfn* LFT_GetNetFilterHookFunction( void )
{
    // Make a patch for RHEL 7 and Centos 7 who are using a newer version of netfilter hook on 
    // a older version of the kernel so the kernel version cannot be used to select the 
    // proper function!
    if( ( utsname()->release != NULL ) 
        && ( strstr( utsname()->release, ".el7.") != NULL ) )
    {
        // printk( KERN_ERR LFT_MODULE_NAME " Special hook for RHEL 7-> LFT_ModuleHookV3\n" );
        return ( nf_hookfn* ) LFT_ModuleHookV3;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION( 2, 6, 24 )
    // printk( KERN_ERR LFT_MODULE_NAME " Netfilter Hook-> LFT_ModuleHookV1\n" );
    return ( nf_hookfn* ) LFT_ModuleHookV1;
#elif LINUX_VERSION_CODE < KERNEL_VERSION( 3, 13, 0 ) 
    // printk( KERN_ERR LFT_MODULE_NAME " Netfilter Hook-> LFT_ModuleHookV2\n" );
    return ( nf_hookfn* ) LFT_ModuleHookV2;
#else
    // printk( KERN_ERR LFT_MODULE_NAME " Netfilter Hook-> LFT_ModuleHookV3\n" );
    return ( nf_hookfn* ) LFT_ModuleHookV3;
#endif
}

static inline unsigned int LFT_GetNetFilterHookNumber( void )
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION( 2, 6, 24 )
    return NF_IP_PRE_ROUTING;
#else
    return NF_INET_PRE_ROUTING;
#endif
}

static int LFT_ModuleInit( void )
{
    LFT_PARAMETERS lParameters;

    // Assign major number
    if( sParamMajor )
    {
        sDevice = MKDEV( sParamMajor, 0 );
        if( register_chrdev_region( sDevice, LFT_DEVICE_COUNT, LFT_MODULE_NAME ) < 0 )
        {
            sParamMajor = MAJOR( sDevice );
            printk( KERN_WARNING LFT_MODULE_NAME " module is cannot get the major requested. New major is %d\n", sParamMajor );
        }
    }
    else
    {
        alloc_chrdev_region( &sDevice, 0, LFT_DEVICE_COUNT, LFT_MODULE_NAME );
        sParamMajor = MAJOR( sDevice );
    }

    // Add a device node
    cdev_init( &sCharDevice, &sFileOperations );
    sCharDevice.owner = THIS_MODULE;
    sCharDevice.ops = &sFileOperations;
    if( unlikely( cdev_add( &sCharDevice, sDevice, LFT_DEVICE_COUNT ) < 0 ) )
    {
        printk( KERN_ERR LFT_MODULE_NAME " module is unable to create a char device node\n" );
		goto Error1;
    }

    // Initialize the internal library
    if( unlikely( LFT_PublicFilterInitStatic( LFT_VERSION_MAJOR, LFT_VERSION_MINOR,
        LFT_VERSION_SUB, LFT_VERSION_BUILD ) != OS_RESULT_OK ) )
    {
        printk( KERN_ERR LFT_MODULE_NAME " module is unable to initialize its internal library\n" );
        goto Error2;
    }

    // Create internal filter handle
    lParameters.TxPoolSize = sTxPoolSize;
    if( unlikely( LFT_PublicFilterInit( &lParameters ) != OS_RESULT_OK ) )
    {
        printk( KERN_ERR LFT_MODULE_NAME " module is unable to create the internal filter logig\n" );
        goto Error3;
    }

    // Initialize the net filter hook
    memset( &sNetFilterOperations, 0, sizeof( sNetFilterOperations ) );
    sNetFilterOperations.hook       = LFT_GetNetFilterHookFunction();
    sNetFilterOperations.pf         = PF_INET; 
    sNetFilterOperations.hooknum    = LFT_GetNetFilterHookNumber();
    sNetFilterOperations.priority   = NF_IP_PRI_FIRST;
    if( unlikely( nf_register_hook( &sNetFilterOperations ) ) )
    {
        printk( KERN_ERR LFT_MODULE_NAME " module is unable to configure the netfilter hook\n" );
		goto Error4;
    }

    return 0;

// Error Handling
Error4:
    LFT_PublicFilterClose();
Error3:
    LFT_PublicFilterCloseStatic();
Error2:
    cdev_del( &sCharDevice );
Error1:
    unregister_chrdev_region( sDevice, LFT_DEVICE_COUNT );
    return -1;
}

static void LFT_ModuleCleanup( void )
{
    nf_unregister_hook( &sNetFilterOperations );
    LFT_PublicFilterClose();
    LFT_PublicFilterCloseStatic();
    cdev_del( &sCharDevice );
    unregister_chrdev_region( sDevice, LFT_DEVICE_COUNT );
}

module_init( LFT_ModuleInit );
module_exit( LFT_ModuleCleanup );

MODULE_AUTHOR( LFT_COMPANY );
MODULE_LICENSE( LFT_COPYRIGHT );
MODULE_DESCRIPTION( LFT_MODULE_LONG_NAME );
MODULE_VERSION( LFT_VERSION );
