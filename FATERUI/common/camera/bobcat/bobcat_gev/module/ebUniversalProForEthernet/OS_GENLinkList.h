// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __OS_GEN_LINKLIST_H__
#define __OS_GEN_LINKLIST_H__

#include "OS_Compiler.h"
#include "OS_Misc.h"
#include "OS_Result.h"
#include "OS_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct 
{
    OS_VOID* Next;
    OS_VOID* Prev;
} OS_LINKLIST;

///
/// \brief Initialize the link list 
///
/// \param aList Link list to initialize
///
#define OS_LINKLIST_CREATE( aList )                                             \
    ( aList )->Next = ( aList )->Prev = ( aList );

///
/// \brief Check if the linklist contains element 
///
/// \param aList The linklist
///
/// \return 1 if element, 0 if empty
///
#define OS_LINKLIST_IS_EMPTY( aList )                                           \
    ( ( OS_UINT32 ) ( ( aList )->Next == ( aList ) ) )

///
/// \brief Get the link list element out of a structure
///
/// \param aElement The structure that constians the element
/// \param aType The type of the aElement instance
/// \param aField The field name of the structure OS_LINKLIST
///
/// \return Pointer to the element item
///
#define OS_LINKLIST_GET_ITEM( aElement, aType, aField )                         \
    OS_CONTAINER_OF( aElement, aType, aField )

/// 
/// \brief Add the element to the head of the link list
///
/// \param aList The linklist structure where we wants to add the element
/// \param aNewElement The element to add to the list
/// 
#define OS_LINKLIST_ADD( aList, aNewElement )                                   \
{                                                                               \
    OS_LINKLIST* lNextElement = ( OS_LINKLIST* ) ( aList )->Next;               \
                                                                                \
    ( aNewElement )->Prev = lNextElement->Prev;                                 \
    lNextElement->Prev = ( aNewElement );                                       \
    ( aNewElement )->Next = lNextElement;                                       \
    ( aList )->Next = aNewElement;                                              \
}

/// 
/// \brief Add the element to the tail of the link list
///
/// \param aList The linklist structure where we wants to add the element
/// \param aNewElement The element to add to the list
/// 
#define OS_LINKLIST_ADD_TO_TAIL( aList, aNewElement )                           \
{                                                                               \
    OS_LINKLIST* lPrevElement = ( OS_LINKLIST* ) ( aList )->Prev;               \
                                                                                \
    ( aList )->Prev = ( aNewElement );                                          \
    lPrevElement->Next = ( aNewElement );                                       \
    ( aNewElement )->Next = ( aList );                                          \
    ( aNewElement )->Prev = lPrevElement;                                       \
}

/// 
/// \brief Delete an element from the linklist
///
/// \param aElement Element to be deleted
///
#define OS_LINKLIST_DELETE( aElement )                                          \
{                                                                               \
    OS_LINKLIST* lNextBackup;                                                   \
    OS_LINKLIST* lPrevBackup;                                                   \
                                                                                \
    lNextBackup = ( OS_LINKLIST* ) ( aElement )->Next;                          \
    lPrevBackup = ( OS_LINKLIST* ) ( aElement )->Prev;                          \
                                                                                \
    /* Remove the element from the list */                                      \
    lPrevBackup->Next = lNextBackup;                                            \
    lNextBackup->Prev = lPrevBackup;                                            \
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OS_GEN_LINKLIST_H__

