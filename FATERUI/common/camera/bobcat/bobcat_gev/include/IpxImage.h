// IpxImage.h: Definition of data structure for Image, that is supported by IIPL. Basic definitions for the IIPL.

//
//////////////////////////////////////////////////////////////////////

#if !defined(IpxImage_H__INCLUDED_)
#define IpxImage_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IpxUserData.h"
#include "IpxPixelType.h"

#ifdef LINUX
	#include <sys/types.h>
#endif
//////////////////////////////////////////////////////////////////////
/// \defgroup ref_img
/// \ingroup ref_data
///@{

//////////////////////////////////////////////////////////////////////
/** \brief Defines major version of image data. */
#define II_IMAGE_MAJOR_VERSION				1
/** \brief Defines minor version of image data. */
#define II_IMAGE_MINOR_VERSION				22
/** \brief Defines whole version of image data. */
#define II_IMAGE_VERSION					((II_IMAGE_MAJOR_VERSION<<16)|II_IMAGE_MINOR_VERSION)
/** \brief Gets major version of image data. */
#define II_GET_MAJOR_VERSION(version)			(version>>16)
/** \brief Gets minor version of image data. */
#define II_GET_MINOR_VERSION(version)			((version<<16)>>16)

//////////////////////////////////////////////////////////////////////

/// Data structure for description of pixel format.
//================================================================================
/**
* \note
* IIPixelTypeDescr stores parameters of the pixel format.
* \req
* \table2{Header, IIImageDef.h}
* \endtable
* \see II_PIXEL_TYPE_DEFINES<BR/>IICreateImage<BR/>IICreateImageHeader<BR/>IIReleaseImage
*/
typedef struct _IIPixelTypeDescr
{
	_IIPixelTypeDescr()	{pixelType = depth = pixAlign = channels = pixSize = 0; pixSigned = false;};
	unsigned long	pixelType;   /**< Pixel type. */
	unsigned long	depth;	     /**< Bit depth of channels. */
	bool			pixSigned;   /**< true for signed pixel. */
	unsigned long	pixAlign;    /**< Pixel packing (packed/normalized). */
	unsigned long	channels;    /**< Number of channels. */
	unsigned long	pixSize;     /**< Pixel size in bits. */
} IIPixelTypeDescr;

/// Data structure for description of Imperx Image.
//================================================================================
/**
* \note
* IIImage stores the image data in a char array. The number of bytes per pixel channel
* is defined by Pixel Type. The field 'imageDataOrigin' is used by IIL for memory management
* and should not be changed by user.
* \req
* \table2{Header, IIImageDef.h}
* \endtable
* \see IIPixelTypeDescr<BR/>II_PIXEL_TYPE_DEFINES<BR/>IICreateImage<BR/>IICreateImageHeader<BR/>IIReleaseImage
*/
typedef struct _IIImage: public IIData
{
	_IIImage()	{nSize = sizeof(_IIImage);
				version = II_IMAGE_VERSION;	
				dataOrder = origin = 0;
				width = height = imageSize = rowSize = imageID = 0; 
				imageData = imageDataOrigin = NULL;
				timestamp = 0; userData = NULL;};
    unsigned long version;				/**< Version of data structure for image. */
	IIPixelTypeDescr pixelTypeDescr;	/**< Descriptor of pixel format for image. */
    int  dataOrder;						/**< 0 - interleaved color channels, 1 - separate color channels.
 											IICreateImage can only create interleaved images. */
    int  origin;						/**< Origin of Image coordinate system: 0 - top-left origin: IIPL_ORIGIN_TL;
											1 - bottom-left origin: IIPL_ORIGIN_BL (Windows bitmaps style). */
    unsigned long width;				/**< Image width in pixels. */
    unsigned long height;				/**< Image height in pixels. */
    IIROI roi;							/**< Region Of Interest (ROI) for image. */
    IICOI coi;							/**< Channels Of Interest (COI) for image. */
    unsigned long imageSize;			/**< Image data size in bytes
											(==image->height*image->rowSize
											in case of interleaved data). */
    unsigned long rowSize;				/**< Size of aligned image row in bytes
											(not necessarily aligned) -
											needed for correct deallocation. */
#ifdef LINUX
    unsigned long long timestamp;
#else
	unsigned __int64 timestamp;			/**< Image timestamp. */
#endif
	unsigned long imageID;
	IIUserData*	userData;				/**< User data, linked with this image. */
    char *imageData;					/**< Pointer to aligned image data. */
    char *imageDataOrigin;				/**< Pointer to very origin of image data. */
}IIImage;


/** \brief Checks whether data is IIImage type. */
#define II_IS_IMAGE_HDR(iiData) \
    ((iiData) != NULL && ((const IIData*)(iiData))->nSize == sizeof(IIImage))
/** \brief Gets pointer to data of first pixel. */
#define II_GET_FIRST_PIXEL_DATA(image)				image->imageData
/** \brief Gets pointer to data of defined pixel. */
#define II_GET_PIXEL_DATA(image, w, h, c)			image->imageData + h * image->rowSize\
													+ w * image->pixelTypeDescr.pixSize / 8\
													+ c * image->pixelTypeDescr.depth / 8


//////////////////////////////////////////////////////////////////////

/// Fills descriptor on base value of pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* @param descr Descriptor of pixel format.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IIImageDef.h<BR/>IIPixelTypeDef.h}
* \endtable
*/
II_INLINE bool IIInitPixelTypeDescr(unsigned long pixelType, IIPixelTypeDescr* descr)
{
	::memset(descr, 0, sizeof(IIPixelTypeDescr));
	const IIColorModelDescription* colorModDescr = IIGetColorModelDescription(pixelType);
	if (!colorModDescr)
		return false;
	descr->pixelType = pixelType;
	descr->depth = colorModDescr->depth;
	if (II_PIX_MONO8S == pixelType)
		descr->pixSigned = 1;
	descr->pixAlign = II_GET_PIXEL_ALIGNMENT(pixelType);
	descr->channels = colorModDescr->channels;
	descr->pixSize = II_GET_PIXEL_BITS_SIZE(pixelType);
	return true;
}

/// Gets parameters of image ROI.
//================================================================================
/**
* @param roi Pointer to ROI of image.
* @param image Pointer to image.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IIImageDef.h<BR/>IIPixelTypeDef.h}
* \endtable
*/
II_INLINE bool IIGetActualROI(IIROI* roi, const IIImage* image)
{
	if (!roi || !image)
		return false;
	roi->xOffset = image->roi.xOffset;
	roi->yOffset = image->roi.yOffset;
	roi->width = image->roi.width;
	roi->height = image->roi.height;
    return true;
}

/// Gets parameters of image COI.
//================================================================================
/**
* @param coi Pointer to COI of image.
* @param image Pointer to image.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IIImageDef.h<BR/>IIPixelTypeDef.h}
* \endtable
*/
II_INLINE bool IIGetActualCOI(IICOI* coi, const IIImage* image)
{
	if (!coi || !image)
		return false;
	coi->indNumber = MAX_IMAGE_CHANNELS;
	if (!IIGetIndexesFromCOI(&image->coi, coi->indexes, &coi->indNumber))
		return false;
    return true;
}

/// Cleans up image data.
//================================================================================
/**
* @param image Pointer to image.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IIImageDef.h<BR/>IIPixelTypeDef.h}
* \endtable
*/
II_INLINE bool IIClearImage(IIImage* image)
{
	if (!image || !image->imageData)
		return false;
	::memset(image->imageData, 0, image->imageSize);
    return true;
}



///@}
//////////////////////////////////////////////////////////////////////
#endif // !defined(IpxImage_H__INCLUDED_)
