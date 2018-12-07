// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __LFT_PUBLIC_H__
#define __LFT_PUBLIC_H__

#include "OS_Compiler.h"
#include "OS_Result.h"
#include "OS_Types.h"

typedef OS_VOID LFT_HDEVICE;

typedef struct 
{
    OS_UINT32 TxPoolSize;
} LFT_PARAMETERS;

// Device Functionalities
OS_DECL_EXTERN OS_RESULT LFT_PublicDeviceRelease( LFT_HDEVICE** aDevice );
OS_DECL_EXTERN OS_RESULT LFT_PublicDeviceIoctl( LFT_HDEVICE** aDevice, OS_UINT32 aCmd, OS_VOID* aArg );

// Filter functionalities
OS_DECL_EXTERN OS_RESULT LFT_PublicFilterInitStatic( OS_UINT32 aMajor, OS_UINT32 aMinor, 
    OS_UINT32 aSub, OS_UINT32 aBuild );
OS_DECL_EXTERN OS_RESULT LFT_PublicFilterCloseStatic( OS_VOID );

OS_DECL_EXTERN OS_RESULT LFT_PublicFilterInit( LFT_PARAMETERS* aParameters );
OS_DECL_EXTERN OS_RESULT LFT_PublicFilterClose( OS_VOID );
OS_DECL_EXTERN OS_RESULT LFT_PublicFilterHook( OS_VOID* aSKBuffer );

#endif // __LFT_PUBLIC_H__
