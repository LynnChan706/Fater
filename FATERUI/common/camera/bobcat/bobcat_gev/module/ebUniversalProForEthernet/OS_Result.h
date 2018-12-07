// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_RESULT_H__
#define __OS_RESULT_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum
{
    OS_RESULT_PACKET_IGNORED = -2,
    OS_RESULT_END_OF_BLOCK = -1,
    OS_RESULT_OK = 0,
    OS_RESULT_FAIL,
    OS_RESULT_OUT_OF_MEMORY,
    OS_RESULT_NOT_ENOUGH_SPACE,
    OS_RESULT_OUT_OF_RESOURCES,
    OS_RESULT_TIMEOUT,
    OS_RESULT_NOT_IMPLEMENTED,
    OS_RESULT_INVALID_CONFIG,
    OS_RESULT_INVALID_ARGUMENT,
    OS_RESULT_BAD_SIZE,
    OS_RESULT_NO_REQUEST_FOUND,
    OS_RESULT_NOT_FOUND,
    OS_RESULT_NOT_CONNECTED,
    OS_RESULT_WRONG_DESTINATION,
    OS_RESULT_PENDING,
    OS_RESULT_ONLY_ONE_CALL_ALLOWED_AT_THE_TIME,
    /*Insert all error codes above here */
    OS_RESULT_LAST
} OS_RESULT;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_RESULT_H__
