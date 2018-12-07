// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_KML_STRING_H__
#define __OS_KML_STRING_H__

#include "OS_Compiler.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

OS_DECL_EXTERN OS_INT32 OS_StringFormat( OS_CHAR* aLine, OS_SIZE aLineSize, const OS_CHAR* aFormat, ... );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_KML_STRING_H__
