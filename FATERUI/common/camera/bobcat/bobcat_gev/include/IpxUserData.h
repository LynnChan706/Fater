// IpxImage.h: Definition of data structure for Image, that is supported by IIPL. Basic definitions for the IIPL.

//
//////////////////////////////////////////////////////////////////////

#if !defined(IpxUserData_H__INCLUDED_)
#define IpxUserData_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <stdio.h>

#include "IpxPixelType.h"


//////////////////////////////////////////////////////////////////////
/// \defgroup ref_img
/// \ingroup ref_data
///@{

/// Definition of user data types for Images which are processed in IIL.
//================================================================================
/**
* \note
* User data are intended to store additional information about image and aren't used
* for image proccessing.
* \req
* \table2{Header, IIImageDef.h}
* \endtable
* \see IIUserData<BR/>IICreateUserData<BR/>IIReleaseUserData
*/
#ifdef IIL_DOCS
	typedef enum
#else
	typedef enum : unsigned long
#endif
{
	II_NOT_DATA	= 0,		/**< Type of user data is undefined. */
	II_HASHTABLE_DATA,		/**< User data are placed into hashtable. */
	II_XML_DATA,			/**< User data have XML format. */
	II_CUSTOM_DATA,			/**< Format of user data is defined by customer. */
}II_USER_DATA;

//================================================================================
/// Data structure for description of User Data linked with Imperx Image.
/**
* \note
* IIL provides only base operation for handling of user data.
* \req
* \table2{Header, IIImageDef.h}
* \endtable
* \see IICreateUserData<BR/>IIReleaseUserData
*/
typedef struct _IIUserData
{
	unsigned long	type;		/**< Type of user data. */
	unsigned long	id;			/**< ID of user data (must be > 0). */
	unsigned long	size;		/**< Size of user data. */
	void*			data;		/**< User data. */
	bool			createdIpx;	/**< true - if user data is created by IpxTools. */
} IIUserData;

/// Allocates and initializes the User Data.
//================================================================================
/**
* @param userData Pointer to the User Data.
* @param type Type of user data.
* @param id ID of user data (must be > 0).
* @param dataSize Size of user data.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function allocates memory for the User Data and cleans allocated memory.
* \req
* \req_table{Header, IpxUserData.h}
* \see  IIReleaseUserData<BR/>IILoadUserData<BR/>IISaveUserData
*/
static bool IICreateUserData(IIUserData** userData, unsigned long type, unsigned long id, unsigned long dataSize)
{
	if (!userData)
		return false;
	size_t totalSize = dataSize + sizeof(IIUserData);
	char* buff = (char*)malloc(totalSize);
	if (!buff)
	{
		return false;
	}
	memset(buff, 0, totalSize);
	IIUserData* tmpData = (IIUserData*)buff;
	tmpData->createdIpx = true;
	tmpData->id = id;
	tmpData->type = type;
	tmpData->size = dataSize;
	if (dataSize > 0)
		tmpData->data = buff + sizeof(IIUserData);
	else
		tmpData->data = 0;

	*userData = tmpData;
	return true;
}

/// Deallocates memory associated with the User Data.
//================================================================================
/**
* @param userData Pointer to the User Data.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function cleans and deallocates memory associated with the User Data.
* \req
* \req_table{Header, IpxUserData.h}
* \see  IICreateUserData<BR/>IILoadUserData<BR/>IISaveUserData
*/
static bool IIReleaseUserData(IIUserData** userData)
{
	if (!userData || !(*userData))
		return true;
	if (!(*userData)->createdIpx)
		return false;
	free(*userData);
	*userData = 0;
	return true;
}

/// Save User Data to stream.
//================================================================================
/**
* @param userData Pointer to the User Data.
* @param file Pointer to data stream.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function copies User Data into data stream.
* \req
* \req_table{Header, IpxUserData.h}
* \see  IILoadUserData<BR/>IICreateUserData<BR/>IIReleaseUserData
*/
static bool IISaveUserData(IIUserData* userData, FILE* file)
{
	if (!file)
		return false;
	if(!userData)
		return true; 
	if(!userData->data)
		userData->size = 0;
	if (1 != ::fwrite(userData, sizeof(IIUserData), 1, file))
	{
		return false;
	}
	if (userData->size > 0)
	{
		if (1 != ::fwrite(userData->data, userData->size, 1, file))
			return false;
	}
	return true;
}

/// Load User Data from stream.
//================================================================================
/**
* @param userData Pointer to new User Data.
* @param file Pointer to data stream.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \note 
* This function allocates memory for new User Data and copies stream data to it.
* \req
* \req_table{Header, IpxUserData.h}
* \see  IISaveUserData<BR/>IICreateUserData<BR/>IIReleaseUserData
*/
static bool IILoadUserData(IIUserData** userData, FILE* file)
{
	if (!userData || !file)
		return false;
	IIUserData tmpUData;
	if (1 != ::fread(&tmpUData, sizeof(IIUserData), 1, file))
	{
		return false;
	}

	if (!IICreateUserData(userData, tmpUData.type, tmpUData.id, tmpUData.size))
		return false;
	if (tmpUData.size > 0 && (1 != ::fread((*userData)->data, tmpUData.size, 1, file)))
	{
		IIReleaseUserData(userData);
		return false;
	}
	return true;
}

///@}

//////////////////////////////////////////////////////////////////////
#endif // !defined(IpxUserData_H__INCLUDED_)
