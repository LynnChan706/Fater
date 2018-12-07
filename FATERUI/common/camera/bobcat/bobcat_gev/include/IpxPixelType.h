// IpxPixelType.h: Description of color models, that are supported by IIPL. Basic definitions for the IIPL.
//
//////////////////////////////////////////////////////////////////////

#if !defined(IpxPixelType_H__INCLUDED_)
#define IpxPixelType_H__INCLUDED_

#include "IpxBaseDef.h"

#ifdef LINUX
	#include <string.h>
#endif
//////////////////////////////////////////////////////////////////////
/// \defgroup ref_pixel
/// \ingroup ref_data
///@{

/// Define pixel alignment.
//================================================================================
/**
* \note
* Pixel alignment defines order of bits placement.   
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
* \see II_PIXEL_TYPE_DEFINES
*/
#ifdef IIL_DOCS
	typedef enum
#else
	typedef enum : unsigned long
#endif
{
	II_ALIGN_8				=	0x00000100, /**< 8-bit unsigned. Value range: 0 to 255 */
	II_ALIGN_8_SIGNED		=	0x00000200, /**< 8-bit signed. Value range: -128 to 127 */
	II_ALIGN_10				=	0x00000300, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_10_PACKED		=	0x00000400, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12				=	0x00000500, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_12_PACKED		=	0x00000600, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_16				=	0x00000700, /**< 16-bit unsigned. Value range: 0 to 65535 */
	II_ALIGN_10_PACKED_V1	=	0x00000800, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12_PACKED_V1	=	0x00000900, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_10P32			=	0x00000A00, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_565			=	0x00000B00, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_1				=	0x00000C00, /**< 1-bit unsigned. Value range: 0 to 1 */
	II_ALIGN_2				=	0x00000D00, /**< 1-bit unsigned. Value range: 0 to 3 */
	II_ALIGN_4				=	0x00000E00, /**< 1-bit unsigned. Value range: 0 to 15 */
// Imperx Imaging Library specific
	II_ALIGN_10_PACKED_IPX	=	0x00000F00, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12_PACKED_IPX	=	0x00001000, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_10_NORMALIZED	=	0x00001100, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12_NORMALIZED	=	0x00001200, /**< 12-bit unsigned. Value range: 0 to 4095 */
// GEV packed
	II_ALIGN_10_PACKED_GEV  =   0x00001300, /**< 10-bit unsigned. Value range: 0 to 1023 */
	II_ALIGN_12_PACKED_GEV  =   0x00001400, /**< 12-bit unsigned. Value range: 0 to 4095 */
	II_ALIGN_14				=	0x00001500, /**< 14-bit unsigned. Value range: 0 to 16383 */
}II_PIXEL_ALIGNMENT;

/** \brief Mask to get pixel alignment. */
#define	II_PIXEL_ALIGNMENT_MASK	0x0000FF00

/// Define pixel chromaticity.
//================================================================================
/**
* \note
* Pixel chromaticity defines number of color channels in an image.   
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
* \see II_PIXEL_TYPE_DEFINES
*/
#ifdef IIL_DOCS
	typedef enum
#else
	typedef enum : unsigned long
#endif
{
	II_PIX_MONO				=	0x01000000,	/**< Monochrome pixel. */
	II_PIX_COLOR			=	0x02000000,	/**< Colored pixel. */
	II_PIX_CUSTOM			=	0x80000000,	/**< Custom defined chromaticity. */
}II_PIXEL_CHROMATICITY;

/** \brief Mask to get pixel chromaticity. */
#define	II_PIXEL_COLOR_MASK		0xFF000000

/// Define effective number of bits occupied by the pixel (including padding).
//================================================================================
/**
* \note
* This value can be used to quickly compute the amount of memory required
* to store an image using pixel type.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
* \see II_PIXEL_TYPE_DEFINES
*/
#ifdef IIL_DOCS
	typedef enum
#else
	typedef enum : unsigned long
#endif
{
	II_PIX_OCCUPY_1_BIT		=	0x00010000,	/**< Pixel size: 1 bits */
	II_PIX_OCCUPY_2_BIT		=	0x00020000,	/**< Pixel size: 2 bits */
	II_PIX_OCCUPY_4_BIT		=	0x00040000,	/**< Pixel size: 4 bits */
	II_PIX_OCCUPY_8_BIT		=	0x00080000,	/**< Pixel size: 8 bits */
	II_PIX_OCCUPY_12_BIT	=	0x000C0000,	/**< Pixel size: 12 bits */
	II_PIX_OCCUPY_16_BIT	=	0x00100000,	/**< Pixel size: 16 bits */
	II_PIX_OCCUPY_20_BIT	=	0x00140000, /**< Pixel size: 20 bits */
	II_PIX_OCCUPY_24_BIT	=	0x00180000,	/**< Pixel size: 24 bits */
	II_PIX_OCCUPY_32_BIT	=	0x00200000,	/**< Pixel size: 32 bits */
	II_PIX_OCCUPY_36_BIT	=	0x00240000,	/**< Pixel size: 36 bits */
	II_PIX_OCCUPY_48_BIT	=	0x00300000,	/**< Pixel size: 48 bits */
}II_PIXEL_BITS;

/** \brief Mask to get pixel chromaticity. */
#define	II_PIXEL_SIZE_MASK		0x00FF0000

/** \brief Mask to get shift of pixel size. */
#define	II_PIXEL_SIZE_SHIFT		16

#define	GVSP_PIX_COUNT			0x46	// Next pixel ID availabel for GiGE Vision 2.0

/// Definition of Pixel Types for Images which are processed in IIL.
//================================================================================
/**
* \note
* Each pixel type is represented by a 32-bit value. The upper 8-bit indicates the 
* pixel chromaticity. The second upper 8-bit indicates the number of bit accupied by a pixel
* (including any padding). This can be used to quickly compute the amount of memory required
* to store an image using pixel type. Next 8-bit indicates pixel alignment that defines order
* of bits placement. Lower 8-bit indicates the pixel type identificator (pixel ID).
* Thus, pixel type contains main information about pixel structure. But pixel type don't
* define such parameters as color depth and channel order.   
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
* \see II_PIXEL_CHROMATICITY<BR/>II_PIXEL_BITS<BR/>II_PIXEL_ALIGNMENT
*/

#ifdef IIL_DOCS
	typedef enum
#else
	typedef enum : unsigned long
#endif
{
//
	II_PIX_MONO1P			=	(II_PIX_MONO | II_PIX_OCCUPY_1_BIT	| II_ALIGN_1			| 0x37),
/**< That and next types define grayscale pixels */
	II_PIX_MONO2P			=	(II_PIX_MONO | II_PIX_OCCUPY_2_BIT	| II_ALIGN_2			| 0x38),
	II_PIX_MONO4P			=	(II_PIX_MONO | II_PIX_OCCUPY_4_BIT	| II_ALIGN_4			| 0x39),
	II_PIX_MONO8			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| 0x01),
	II_PIX_MONO8S			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8_SIGNED		| 0x02),
	II_PIX_MONO10			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| 0x03),
	II_PIX_MONO10_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED	| 0x04),
	II_PIX_MONO12			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| 0x05),
	II_PIX_MONO12_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED	| 0x06),
	II_PIX_MONO14			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| 0x25),
	II_PIX_MONO16			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			| 0x07),
//
	II_PIX_BAYGR8			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| 0x08),
/**< That and next types define Bayer pixels */
	II_PIX_BAYRG8			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| 0x09),
	II_PIX_BAYGB8			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| 0x0A),
	II_PIX_BAYBG8			=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| 0x0B),
	II_PIX_BAYGR10			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| 0x0C),
	II_PIX_BAYRG10			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| 0x0D),
	II_PIX_BAYGB10			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| 0x0E),
	II_PIX_BAYBG10			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| 0x0F),
	II_PIX_BAYGR12			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| 0x10),
	II_PIX_BAYRG12			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| 0x11),
	II_PIX_BAYGB12			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| 0x12),
	II_PIX_BAYBG12			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| 0x13),
	II_PIX_BAYGR10_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED	| 0x26),
	II_PIX_BAYRG10_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED	| 0x27),
	II_PIX_BAYGB10_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED	| 0x28),
	II_PIX_BAYBG10_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED	| 0x29),
	II_PIX_BAYGR12_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED	| 0x2A),
	II_PIX_BAYRG12_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED	| 0x2B),
	II_PIX_BAYGB12_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED	| 0x2C),
	II_PIX_BAYBG12_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED	| 0x2D),
	II_PIX_BAYGR14			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x3F),
	II_PIX_BAYRG14			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x40),
	II_PIX_BAYGB14			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x41),
	II_PIX_BAYBG14			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x42),
	II_PIX_BAYGR16			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			| 0x2E),
	II_PIX_BAYRG16			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			| 0x2F),
	II_PIX_BAYGB16			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			| 0x30),
	II_PIX_BAYBG16			=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_16			| 0x31),
//
	II_PIX_TS_BGGR_WBBW0_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x00),
/**< That and next types define True Sense pixels */
	II_PIX_TS_BGGR_WBBW1_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x01),
	II_PIX_TS_BGGR_WBBW2_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x02),
	II_PIX_TS_BGGR_WBBW3_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x03),
	II_PIX_TS_GBRG_WGGW0_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x04),
	II_PIX_TS_GBRG_WGGW1_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x05),
	II_PIX_TS_GBRG_WGGW2_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x06),
	II_PIX_TS_GBRG_WGGW3_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x07),
	II_PIX_TS_GRBG_WGGW0_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x08),
	II_PIX_TS_GRBG_WGGW1_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x09),
	II_PIX_TS_GRBG_WGGW2_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0A),
	II_PIX_TS_GRBG_WGGW3_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0B),
	II_PIX_TS_RGGB_WRRW0_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0C),
	II_PIX_TS_RGGB_WRRW1_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0D),
	II_PIX_TS_RGGB_WRRW2_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0E),
	II_PIX_TS_RGGB_WRRW3_8	=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x0F),

	II_PIX_TS_BGGR_WBBW0_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x10),
	II_PIX_TS_BGGR_WBBW1_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x11),
	II_PIX_TS_BGGR_WBBW2_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x12),
	II_PIX_TS_BGGR_WBBW3_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x13),
	II_PIX_TS_GBRG_WGGW0_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x14),
	II_PIX_TS_GBRG_WGGW1_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x15),
	II_PIX_TS_GBRG_WGGW2_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x16),
	II_PIX_TS_GBRG_WGGW3_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x17),
	II_PIX_TS_GRBG_WGGW0_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x18),
	II_PIX_TS_GRBG_WGGW1_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x19),
	II_PIX_TS_GRBG_WGGW2_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1A),
	II_PIX_TS_GRBG_WGGW3_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1B),
	II_PIX_TS_RGGB_WRRW0_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1C),
	II_PIX_TS_RGGB_WRRW1_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1D),
	II_PIX_TS_RGGB_WRRW2_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1E),
	II_PIX_TS_RGGB_WRRW3_10	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x1F),

	II_PIX_TS_BGGR_WBBW0_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x20),
	II_PIX_TS_BGGR_WBBW1_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x21),
	II_PIX_TS_BGGR_WBBW2_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x22),
	II_PIX_TS_BGGR_WBBW3_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x23),
	II_PIX_TS_GBRG_WGGW0_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x24),
	II_PIX_TS_GBRG_WGGW1_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x25),
	II_PIX_TS_GBRG_WGGW2_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x26),
	II_PIX_TS_GBRG_WGGW3_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x27),
	II_PIX_TS_GRBG_WGGW0_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x28),
	II_PIX_TS_GRBG_WGGW1_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x29),
	II_PIX_TS_GRBG_WGGW2_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2A),
	II_PIX_TS_GRBG_WGGW3_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2B),
	II_PIX_TS_RGGB_WRRW0_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2C),
	II_PIX_TS_RGGB_WRRW1_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2D),
	II_PIX_TS_RGGB_WRRW2_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2E),
	II_PIX_TS_RGGB_WRRW3_12	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12			| GVSP_PIX_COUNT + 0x2F),

	II_PIX_TS_BGGR_WBBW0_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x43),
	II_PIX_TS_BGGR_WBBW1_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x44),
	II_PIX_TS_BGGR_WBBW2_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x45),
	II_PIX_TS_BGGR_WBBW3_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x46),
	II_PIX_TS_GBRG_WGGW0_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x47),
	II_PIX_TS_GBRG_WGGW1_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x48),
	II_PIX_TS_GBRG_WGGW2_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x49),
	II_PIX_TS_GBRG_WGGW3_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4A),
	II_PIX_TS_GRBG_WGGW0_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4B),
	II_PIX_TS_GRBG_WGGW1_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4C),
	II_PIX_TS_GRBG_WGGW2_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4D),
	II_PIX_TS_GRBG_WGGW3_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4E),
	II_PIX_TS_RGGB_WRRW0_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x4F),
	II_PIX_TS_RGGB_WRRW1_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x50),
	II_PIX_TS_RGGB_WRRW2_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x51),
	II_PIX_TS_RGGB_WRRW3_14	=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x52),
//
	II_PIX_RGB8				=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x14),
/**< That and next types define RGB-BGR pixels */
	II_PIX_BGR8				=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x15),
	II_PIX_RGBA8			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			| 0x16),
	II_PIX_BGRA8			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			| 0x17),
	II_PIX_RGB10			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_10			| 0x18),
	II_PIX_BGR10			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_10			| 0x19),
	II_PIX_RGB12			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_12			| 0x1A),
	II_PIX_BGR12			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_12			| 0x1B),
	II_PIX_RGB14			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x53),
	II_PIX_BGR14			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_14			| GVSP_PIX_COUNT + 0x54),
	II_PIX_RGB16			=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_16			| 0x33),
	II_PIX_RGB10V1_PACKED	=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_10_PACKED_V1	| 0x1C),
	II_PIX_RGB10P32			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_10P32		| 0x1D),
	II_PIX_RGB12V1_PACKED	=	(II_PIX_COLOR | II_PIX_OCCUPY_36_BIT	| II_ALIGN_12_PACKED_V1	| 0x34),
	II_PIX_RGB565P			=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_565			| 0x35),
	II_PIX_BGR565P			=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_565			| 0x36),
//
	II_PIX_YUV411_8_UYYVYY		=	(II_PIX_COLOR | II_PIX_OCCUPY_12_BIT	| II_ALIGN_8			| 0x1E),
/**< That and next types define YUV and TCbCr packed pixels */
	II_PIX_YUV422_8_UYVY		=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x1F),
	II_PIX_YUV422_8				=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x32),
	II_PIX_YUV422_10			=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_10			| GVSP_PIX_COUNT + 0x35),
	II_PIX_YUV422_10_PACKED		=	(II_PIX_COLOR | II_PIX_OCCUPY_20_BIT	| II_ALIGN_10_PACKED	| GVSP_PIX_COUNT + 0x3E),
	II_PIX_YUV8_UYV				=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x20),
	II_PIX_YUV444_8				=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x34),
	II_PIX_YCBCR8_CBYCR			=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x3A),
	II_PIX_YCBCR422_8			=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x3B),
	II_PIX_YCBCR422_8_CBYCRY	=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x43),
	II_PIX_YCBCR411_8_CBYYCRYY	=	(II_PIX_COLOR | II_PIX_OCCUPY_12_BIT	| II_ALIGN_8			| 0x3C),
	II_PIX_YCBCR601_8_CBYCR		=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x3D),
	II_PIX_YCBCR601_422_8		=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x3E),
	II_PIX_YCBCR601_422_8_CBYCRY=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x44),
	II_PIX_YCBCR601_411_8_CBYYCRYY=	(II_PIX_COLOR | II_PIX_OCCUPY_12_BIT	| II_ALIGN_8			| 0x3F),
	II_PIX_YCBCR709_8_CBYCR		=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x40),
	II_PIX_YCBCR709_422_8		=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x41),
	II_PIX_YCBCR709_422_8_CBYCRY=	(II_PIX_COLOR | II_PIX_OCCUPY_16_BIT	| II_ALIGN_8			| 0x45),
	II_PIX_YCBCR709_411_8_CBYYCRYY=	(II_PIX_COLOR | II_PIX_OCCUPY_12_BIT	| II_ALIGN_8			| 0x42),
//
	II_PIX_RGB8_PLANAR		=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| 0x21),
/**< That and next types define RGB planar pixels */
	II_PIX_RGB10_PLANAR		=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_10			| 0x22),
	II_PIX_RGB12_PLANAR		=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_12			| 0x23),
	II_PIX_RGB16_PLANAR		=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_16			| 0x24),
//
	II_PIX_MONO10IPX_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_10_PACKED_IPX| GVSP_PIX_COUNT + 0x30),
/**< That and next types define custom IIL pixels */
	II_PIX_MONO12IPX_PACKED	=	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT	| II_ALIGN_12_PACKED_IPX| GVSP_PIX_COUNT + 0x31),
	II_PIX_MONO10_NORMALIZED=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_10_NORMALIZED| GVSP_PIX_COUNT + 0x32),
	II_PIX_MONO12_NORMALIZED=	(II_PIX_MONO | II_PIX_OCCUPY_16_BIT	| II_ALIGN_12_NORMALIZED| GVSP_PIX_COUNT + 0x33),
//
	II_PIX_MONO10GEV_PACKED =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT | II_ALIGN_10_PACKED_GEV	| GVSP_PIX_COUNT + 0x36),
/**< That and next types define FleX packed pixels */
	II_PIX_MONO12GEV_PACKED =	(II_PIX_MONO | II_PIX_OCCUPY_12_BIT | II_ALIGN_12_PACKED_GEV	| GVSP_PIX_COUNT + 0x37),
	II_PIX_RGB8_PACKED		=	(II_PIX_COLOR | II_PIX_OCCUPY_24_BIT	| II_ALIGN_8			| GVSP_PIX_COUNT + 0x38),
	II_PIX_RGB10_PACKED		=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT | II_ALIGN_10_PACKED		| GVSP_PIX_COUNT + 0x39),
	II_PIX_RGB10GEV_PACKED	=	(II_PIX_COLOR | II_PIX_OCCUPY_32_BIT | II_ALIGN_10_PACKED_GEV	| GVSP_PIX_COUNT + 0x3A),
	II_PIX_RGB12_PACKED		=	(II_PIX_COLOR | II_PIX_OCCUPY_36_BIT | II_ALIGN_12_PACKED		| GVSP_PIX_COUNT + 0x3B),
	II_PIX_RGB12GEV_PACKED	=	(II_PIX_COLOR | II_PIX_OCCUPY_36_BIT | II_ALIGN_12_PACKED_GEV	| GVSP_PIX_COUNT + 0x3C),
	II_PIX_MONO8_PACKED		=	(II_PIX_MONO | II_PIX_OCCUPY_8_BIT | II_ALIGN_8	| GVSP_PIX_COUNT + 0x55),

	II_PIX_BGR16	=	(II_PIX_COLOR | II_PIX_OCCUPY_48_BIT	| II_ALIGN_16	| GVSP_PIX_COUNT + 0x3D),

//
	II_PIX_NONE_TYPE		= 0
/**< The label for undefined pixel type */
}II_PIXEL_TYPE_DEFINES;

/** \brief Mask to get pixel ID value. */
#define	II_PIXEL_ID_MASK		0x000000FF

//////////////////////////////////////////////////////////////////////////////////////////////////////

/** \brief Returns aligned row size for defined pixel type and number of pixels in row. */
#define	II_GET_ROW_SIZE(pixType, width)				IIGetRowSize(pixType, width)
/** \brief Returns pixel alignment for defined pixel type. */
#define	II_GET_PIXEL_ALIGNMENT(pixType)				(pixType & II_PIXEL_ALIGNMENT_MASK)
/** \brief Returns pixel chromaticity for defined pixel type. */
#define	II_GET_PIXEL_CHROMATICITY(pixType)			(pixType & II_PIXEL_COLOR_MASK)
/** \brief Returns 'true' if pixel is colored. */
#define	II_IS_COLOR_PIXEL(pixType)					((pixType & II_PIXEL_COLOR_MASK) == II_PIX_COLOR)
/** \brief Returns 'true' if pixel is custom. */
#define	II_IS_CUSTOM_PIXEL(pixType)					((pixType & II_PIXEL_COLOR_MASK) == II_PIX_CUSTOM)
/** \brief Returns size of pixel in bits for defined pixel type. */
#define	II_GET_PIXEL_BITS_SIZE(pixType)				((pixType & II_PIXEL_SIZE_MASK)>>II_PIXEL_SIZE_SHIFT)
/** \brief Returns identificator of pixel type for defined pixel type. */
#define	II_GET_PIXEL_ID(pixType)					(pixType & II_PIXEL_ID_MASK)
/** \brief Returns index of pixel type for defined pixel type. */
#define	II_GET_PIXEL_TYPE_INDEX(pixType)			((pixType & II_PIXEL_ID_MASK) - 1)
/** \brief Returns aligned image size for defined pixel type, width and height. */
#define	II_GET_IMAGE_SIZE(pixType, width, height)	(height * II_GET_ROW_SIZE(pixType, width))
/** \brief Returns 'true' if pixel is planar. */
#define	II_IS_PLANAR_PIXEL(pixType)					(pixType == II_PIX_RGB8_PLANAR ||\
													pixType == II_PIX_RGB10_PLANAR ||\
													pixType == II_PIX_RGB12_PLANAR ||\
													pixType == II_PIX_RGB16_PLANAR )
/** \brief Returns 'true' if pixel is packed. */
#define	II_IS_PACKED_PIXEL(pixType)					(II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_10_PACKED ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_12_PACKED ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_10_PACKED_V1 ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_12_PACKED_V1 ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_10_PACKED_IPX ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_12_PACKED_IPX ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_10_PACKED_GEV ||\
													II_GET_PIXEL_ALIGNMENT(pixType) == II_ALIGN_12_PACKED_GEV )


/// Returns row size for defined pixel type and number of pixels in row.
//================================================================================
/**
* @param pixType Pixel type.
* @param width Number of pixels in row.
* \return 
* The return value is row size.
* \note 
* Row size is aligned for effective memory using.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE  unsigned long IIGetRowSize(unsigned long pixType, unsigned long width)
{
	unsigned long rowSize = width * II_GET_PIXEL_BITS_SIZE(pixType);
	rowSize = (rowSize % 8) ? ((rowSize >> 3) + 1) : (rowSize >> 3);
	return (rowSize + II_DEFAULT_IMAGE_ROW_ALIGN - 1) & (~(II_DEFAULT_IMAGE_ROW_ALIGN - 1));
}

///@}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////            Names of Color Models
//////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Names for Mono pixel format
#define	II_PTN_MONO_1					"Mono1p"
#define	II_PTN_MONO_2					"Mono2p"
#define	II_PTN_MONO_4					"Mono4p"
#define	II_PTN_MONO_8					"Mono8"
#define	II_PTN_MONO_8_SIGNED			"Mono8s"
#define	II_PTN_MONO_10					"Mono10"
#define	II_PTN_MONO_10_PACKED			"Mono10Packed"
#define	II_PTN_MONO_12					"Mono12"
#define	II_PTN_MONO_12_PACKED			"Mono12Packed"
#define	II_PTN_MONO_14					"Mono14"
#define	II_PTN_MONO_16					"Mono16"
//
// Names for Bayer pixel format
#define	II_PTN_BAYER_GR8				"BayerGR8"
#define	II_PTN_BAYER_RG8				"BayerRG8"
#define	II_PTN_BAYER_GB8				"BayerGB8"
#define	II_PTN_BAYER_BG8				"BayerBG8"
#define	II_PTN_BAYER_GR10				"BayerGR10"
#define	II_PTN_BAYER_RG10				"BayerRG10"
#define	II_PTN_BAYER_GB10				"BayerGB10"
#define	II_PTN_BAYER_BG10				"BayerBG10"
#define	II_PTN_BAYER_GR12				"BayerGR12"
#define	II_PTN_BAYER_RG12				"BayerRG12"
#define	II_PTN_BAYER_GB12				"BayerGB12"
#define	II_PTN_BAYER_BG12				"BayerBG12"
#define	II_PTN_BAYER_GR10_PACKED		"BayerGR10Packed"
#define	II_PTN_BAYER_RG10_PACKED		"BayerRG10Packed"
#define	II_PTN_BAYER_GB10_PACKED		"BayerGB10Packed"
#define	II_PTN_BAYER_BG10_PACKED		"BayerBG10Packed"
#define	II_PTN_BAYER_GR12_PACKED		"BayerGR12Packed"
#define	II_PTN_BAYER_RG12_PACKED		"BayerRG12Packed"
#define	II_PTN_BAYER_GB12_PACKED		"BayerGB12Packed"
#define	II_PTN_BAYER_BG12_PACKED		"BayerBG12Packed"
#define	II_PTN_BAYER_GR14				"BayerGR14"
#define	II_PTN_BAYER_RG14				"BayerRG14"
#define	II_PTN_BAYER_GB14				"BayerGB14"
#define	II_PTN_BAYER_BG14				"BayerBG14"
#define	II_PTN_BAYER_GR16				"BayerGR16"
#define	II_PTN_BAYER_RG16				"BayerRG16"
#define	II_PTN_BAYER_GB16				"BayerGB16"
#define	II_PTN_BAYER_BG16				"BayerBG16"
//
// True Sense pixel format defines
#define	II_PTN_TS_BGGR_WBBW_0_8			"TrueSense_BGGR_WBBW_0_8"
#define	II_PTN_TS_BGGR_WBBW_1_8			"TrueSense_BGGR_WBBW_1_8"
#define	II_PTN_TS_BGGR_WBBW_2_8			"TrueSense_BGGR_WBBW_2_8"
#define	II_PTN_TS_BGGR_WBBW_3_8			"TrueSense_BGGR_WBBW_3_8"
#define	II_PTN_TS_GBRG_WGGW_0_8			"TrueSense_GBRG_WGGW_0_8"
#define	II_PTN_TS_GBRG_WGGW_1_8			"TrueSense_GBRG_WGGW_1_8"
#define	II_PTN_TS_GBRG_WGGW_2_8			"TrueSense_GBRG_WGGW_2_8"
#define	II_PTN_TS_GBRG_WGGW_3_8			"TrueSense_GBRG_WGGW_3_8"
#define	II_PTN_TS_GRBG_WGGW_0_8			"TrueSense_GRBG_WGGW_0_8"
#define	II_PTN_TS_GRBG_WGGW_1_8			"TrueSense_GRBG_WGGW_1_8"
#define	II_PTN_TS_GRBG_WGGW_2_8			"TrueSense_GRBG_WGGW_2_8"
#define	II_PTN_TS_GRBG_WGGW_3_8			"TrueSense_GRBG_WGGW_3_8"
#define	II_PTN_TS_RGGB_WRRW_0_8			"TrueSense_RGGB_WRRW_0_8"
#define	II_PTN_TS_RGGB_WRRW_1_8			"TrueSense_RGGB_WRRW_1_8"
#define	II_PTN_TS_RGGB_WRRW_2_8			"TrueSense_RGGB_WRRW_2_8"
#define	II_PTN_TS_RGGB_WRRW_3_8			"TrueSense_RGGB_WRRW_3_8"

#define	II_PTN_TS_BGGR_WBBW_0_10		"TrueSense_BGGR_WBBW_0_10"
#define	II_PTN_TS_BGGR_WBBW_1_10		"TrueSense_BGGR_WBBW_1_10"
#define	II_PTN_TS_BGGR_WBBW_2_10		"TrueSense_BGGR_WBBW_2_10"
#define	II_PTN_TS_BGGR_WBBW_3_10		"TrueSense_BGGR_WBBW_3_10"
#define	II_PTN_TS_GBRG_WGGW_0_10		"TrueSense_GBRG_WGGW_0_10"
#define	II_PTN_TS_GBRG_WGGW_1_10		"TrueSense_GBRG_WGGW_1_10"
#define	II_PTN_TS_GBRG_WGGW_2_10		"TrueSense_GBRG_WGGW_2_10"
#define	II_PTN_TS_GBRG_WGGW_3_10		"TrueSense_GBRG_WGGW_3_10"
#define	II_PTN_TS_GRBG_WGGW_0_10		"TrueSense_GRBG_WGGW_0_10"
#define	II_PTN_TS_GRBG_WGGW_1_10		"TrueSense_GRBG_WGGW_1_10"
#define	II_PTN_TS_GRBG_WGGW_2_10		"TrueSense_GRBG_WGGW_2_10"
#define	II_PTN_TS_GRBG_WGGW_3_10		"TrueSense_GRBG_WGGW_3_10"
#define	II_PTN_TS_RGGB_WRRW_0_10		"TrueSense_RGGB_WRRW_0_10"
#define	II_PTN_TS_RGGB_WRRW_1_10		"TrueSense_RGGB_WRRW_1_10"
#define	II_PTN_TS_RGGB_WRRW_2_10		"TrueSense_RGGB_WRRW_2_10"
#define	II_PTN_TS_RGGB_WRRW_3_10		"TrueSense_RGGB_WRRW_3_10"

#define	II_PTN_TS_BGGR_WBBW_0_12		"TrueSense_BGGR_WBBW_0_12"
#define	II_PTN_TS_BGGR_WBBW_1_12		"TrueSense_BGGR_WBBW_1_12"
#define	II_PTN_TS_BGGR_WBBW_2_12		"TrueSense_BGGR_WBBW_2_12"
#define	II_PTN_TS_BGGR_WBBW_3_12		"TrueSense_BGGR_WBBW_3_12"
#define	II_PTN_TS_GBRG_WGGW_0_12		"TrueSense_GBRG_WGGW_0_12"
#define	II_PTN_TS_GBRG_WGGW_1_12		"TrueSense_GBRG_WGGW_1_12"
#define	II_PTN_TS_GBRG_WGGW_2_12		"TrueSense_GBRG_WGGW_2_12"
#define	II_PTN_TS_GBRG_WGGW_3_12		"TrueSense_GBRG_WGGW_3_12"
#define	II_PTN_TS_GRBG_WGGW_0_12		"TrueSense_GRBG_WGGW_0_12"
#define	II_PTN_TS_GRBG_WGGW_1_12		"TrueSense_GRBG_WGGW_1_12"
#define	II_PTN_TS_GRBG_WGGW_2_12		"TrueSense_GRBG_WGGW_2_12"
#define	II_PTN_TS_GRBG_WGGW_3_12		"TrueSense_GRBG_WGGW_3_12"
#define	II_PTN_TS_RGGB_WRRW_0_12		"TrueSense_RGGB_WRRW_0_12"
#define	II_PTN_TS_RGGB_WRRW_1_12		"TrueSense_RGGB_WRRW_1_12"
#define	II_PTN_TS_RGGB_WRRW_2_12		"TrueSense_RGGB_WRRW_2_12"
#define	II_PTN_TS_RGGB_WRRW_3_12		"TrueSense_RGGB_WRRW_3_12"

#define	II_PTN_TS_BGGR_WBBW_0_14		"TrueSense_BGGR_WBBW_0_14"
#define	II_PTN_TS_BGGR_WBBW_1_14		"TrueSense_BGGR_WBBW_1_14"
#define	II_PTN_TS_BGGR_WBBW_2_14		"TrueSense_BGGR_WBBW_2_14"
#define	II_PTN_TS_BGGR_WBBW_3_14		"TrueSense_BGGR_WBBW_3_14"
#define	II_PTN_TS_GBRG_WGGW_0_14		"TrueSense_GBRG_WGGW_0_14"
#define	II_PTN_TS_GBRG_WGGW_1_14		"TrueSense_GBRG_WGGW_1_14"
#define	II_PTN_TS_GBRG_WGGW_2_14		"TrueSense_GBRG_WGGW_2_14"
#define	II_PTN_TS_GBRG_WGGW_3_14		"TrueSense_GBRG_WGGW_3_14"
#define	II_PTN_TS_GRBG_WGGW_0_14		"TrueSense_GRBG_WGGW_0_14"
#define	II_PTN_TS_GRBG_WGGW_1_14		"TrueSense_GRBG_WGGW_1_14"
#define	II_PTN_TS_GRBG_WGGW_2_14		"TrueSense_GRBG_WGGW_2_14"
#define	II_PTN_TS_GRBG_WGGW_3_14		"TrueSense_GRBG_WGGW_3_14"
#define	II_PTN_TS_RGGB_WRRW_0_14		"TrueSense_RGGB_WRRW_0_14"
#define	II_PTN_TS_RGGB_WRRW_1_14		"TrueSense_RGGB_WRRW_1_14"
#define	II_PTN_TS_RGGB_WRRW_2_14		"TrueSense_RGGB_WRRW_2_14"
#define	II_PTN_TS_RGGB_WRRW_3_14		"TrueSense_RGGB_WRRW_3_14"
//
// RGB Packed pixel format defines
#define	II_PTN_RGB_8					"RGB8"
#define	II_PTN_BGR_8					"BGR8"
#define	II_PTN_RGBA_8					"RGBa8"
#define	II_PTN_BGRA_8					"BGRa8"
#define	II_PTN_RGB_10					"RGB10"
#define	II_PTN_BGR_10					"BGR10"
#define	II_PTN_RGB_12					"RGB12"
#define	II_PTN_BGR_12					"BGR12"
#define	II_PTN_RGB_14					"RGB14"
#define	II_PTN_BGR_14					"BGR14"
#define	II_PTN_RGB_16					"RGB16"
#define	II_PTN_BGR_16					"BGR16"
#define	II_PTN_RGB_10V1_PACKED			"RGB10V1Packed"
#define	II_PTN_RGB_10P32				"RGB10p32"
#define	II_PTN_RGB_12V1_PACKED			"RGB12V1Packed"
#define	II_PTN_RGB_565P					"RGB565p"
#define	II_PTN_BGR_565P					"BGR565p"
//
// YUV Packed pixel format defines
#define	II_PTN_YUV411_8_UYYVYY			"YUV411_8_UYYVYY"
#define	II_PTN_YUV422_8_UYVY			"YUV422_8_UYVY"
#define	II_PTN_YUV422_8					"YUV422_8"
#define	II_PTN_YUV422_10				"YUV422_10"
#define	II_PTN_YUV422_10_PACKED			"YUV422_10_PACKED"
#define II_PTN_YUV444_8					"YUV444_8"
#define	II_PTN_YUV8_UYV					"YUV8_UYV"
#define	II_PTN_YCBCR8_CBYCR				"YCbCr8_CbYCr"
#define	II_PTN_YCBCR422_8				"YCbCr422_8"
#define	II_PTN_YCBCR422_8_CBYCRY		"YCbCr422_8_CbYCrY"
#define	II_PTN_YCBCR411_8_CBYYCRYY		"YCbCr411_8_CbYYCrYY"
#define	II_PTN_YCBCR601_8_CBYCR			"YCbCr601_8_CbYCr"
#define	II_PTN_YCBCR601_422_8			"YCbCr601_422_8"
#define	II_PTN_YCBCR601_422_8_CBYCRY	"YCbCr601_422_8_CbYCrY"
#define	II_PTN_YCBCR601_411_8_CBYYCRYY	"YCbCr601_411_8_CbYYCrYY"
#define	II_PTN_YCBCR709_8_CBYCR			"YCbCr709_8_CbYCr"
#define	II_PTN_YCBCR709_422_8			"YCbCr709_422_8"
#define	II_PTN_YCBCR709_422_8_CBYCRY	"YCbCr709_422_8_CbYCrY"
#define	II_PTN_YCBCR709_411_8_CBYYCRYY	"YCbCr709_411_8_CbYYCrYY"
//
// RGB Planar pixel format defines
#define	II_PTN_RGB_8_PLANAR				"RGB8_Planar"
#define	II_PTN_RGB_10_PLANAR			"RGB_10_Planar"
#define	II_PTN_RGB_12_PLANAR			"RGB_12_Planar"
#define	II_PTN_RGB_16_PLANAR			"RGB_16_Planar"
//
// Custom Imperx Imaging Library pixel format defines
#define	II_PTN_MONO_10_IPX_PACKED	"Mono10_IPX_Packed"
#define	II_PTN_MONO_12_IPX_PACKED	"Mono12_IPX_Packed"
#define	II_PTN_MONO_10_NORMALIZED	"Mono10_Normolized"
#define	II_PTN_MONO_12_NORMALIZED	"Mono12_Normolized"
//
//FleX packed pixel format defines
#define II_PTN_MONO_10_GEV_PACKED	"Mono10_GEV_Packed"
#define II_PTN_MONO_12_GEV_PACKED	"Mono12_GEV_Packed"
#define	II_PTN_RGB_8_PACKED			"RGB8_Packed"
#define	II_PTN_RGB_10_PACKED		"RGB_10_Packed"
#define	II_PTN_RGB_10_GEV_PACKED	"RGB_10_GEV_Packed"
#define	II_PTN_RGB_12_PACKED		"RGB_12_Packed"
#define	II_PTN_RGB_12_GEV_PACKED	"RGB_12_GEV_Packed"
#define	II_PTN_MONO_8_PACKED		"RGB_8_Packed"


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////            Group Names of Pixel Types
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define	II_GNPT_MONO					"Mono"
#define	II_GNPT_BAYER					"Bayer"
#define	II_GNPT_TRUE_SENSE				"TrueSense"
#define	II_GNPT_RGB						"RGB"
#define	II_GNPT_BGR						"BGR"
#define	II_GNPT_YUV						"YUV"
#define	II_GNPT_YCBCR					"YCbCr"


typedef	int GetIndexChannelTYPE(short chName);

typedef enum : short
{
	IIPC_NONE = 0,
	IIPC_ALFA = 'A',
	IIPC_GRAY = 'Y',
	IIPC_RED = 'R',
	IIPC_GREEN = 'G',
	IIPC_GREEN_2 = '2'<<8 | 'G',    // "G2"
	IIPC_BLUE = 'B',
	IIPC_X = 'X',
	IIPC_Y = 'Y',
	IIPC_Z = 'Z',
	IIPC_U = 'U',
	IIPC_V = 'V',
	IIPC_W = 'W',
	IIPC_CB = 'b'<<8 | 'C',
	IIPC_CR = 'r'<<8 | 'C',
	IIPC_YE = 'e'<<8 | 'Y',
	IIPC_IR = 'r'<<8 | 'I',
}II_PIXEL_CHANNELS;

static char* s_undefColorModel = "Undefined Color Model";

typedef struct _IIColorModelDescription
{
    char*					modelName;
    char*					channelSequence;	// also defines convertor type - source or destination
	int						channels;
	int						depth;
    GetIndexChannelTYPE*	GetIndex;
	int						startPos;
	unsigned long			pixelType;
}IIColorModelDescription;

//==================================================================
II_INLINE int GetIndexMONO (short chName)
{
	switch(chName)
	{
	case IIPC_GRAY:
			return 0;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexRGB (short chName)
{
	switch(chName)
	{
	case IIPC_RED:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_BLUE:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexPLANAR (short chName)
{
	switch(chName)
	{
	case IIPC_RED:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_BLUE:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexBGR (short chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexRGBA (short chName)
{
	switch(chName)
	{
	case IIPC_RED:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_BLUE:
			return 2;
	case IIPC_ALFA:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexBGRA (short chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	case IIPC_ALFA:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexBAYER (short chName)
{
	switch(chName)
	{
	case IIPC_BLUE:
			return 0;
	case IIPC_GREEN:
			return 1;
	case IIPC_RED:
			return 2;
	case IIPC_GREEN_2:
			return 3;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexXYZ (short chName)
{
	switch(chName)
	{
	case IIPC_X:
			return 0;
	case IIPC_Y:
			return 1;
	case IIPC_Z:
			return 2;
	default:
		return  -1;
	}
}
//==================================================================
II_INLINE int GetIndexYUV (short chName)
{
	switch(chName)
	{
	case IIPC_Y:
			return 0;
	case IIPC_U:
			return 1;
	case IIPC_V:
			return 2;
	default:
		return  -1;
	}
}

static IIColorModelDescription s_colorModelDescription[] =
{
	{II_PTN_MONO_8, "Y", 1, 8, GetIndexMONO,			0, II_PIX_MONO8},
	{II_PTN_MONO_8_SIGNED, "Y", 1, 8, GetIndexMONO,		0, II_PIX_MONO8S},
	{II_PTN_MONO_10, "Y", 1, 10, GetIndexMONO,			0, II_PIX_MONO10},
	{II_PTN_MONO_10_PACKED, "Y", 1, 10, GetIndexMONO,	0, II_PIX_MONO10_PACKED},
	{II_PTN_MONO_12, "Y", 1, 12, GetIndexMONO,			0, II_PIX_MONO12},
	{II_PTN_MONO_12_PACKED, "Y", 1, 12, GetIndexMONO,	0, II_PIX_MONO12_PACKED},
	{II_PTN_MONO_16, "Y", 1, 16, GetIndexMONO,			0, II_PIX_MONO16},
	{II_PTN_BAYER_GR8,"G R G2 B", 1, 8, GetIndexMONO,	0, II_PIX_BAYGR8},
	{II_PTN_BAYER_RG8,"R G B G2", 1, 8, GetIndexMONO,	0, II_PIX_BAYRG8},
	{II_PTN_BAYER_GB8,"G B G2 R", 1, 8, GetIndexMONO,	0, II_PIX_BAYGB8},
	{II_PTN_BAYER_BG8,"B G R G2", 1, 8, GetIndexMONO,	0, II_PIX_BAYBG8},
	{II_PTN_BAYER_GR10,"G R G2 B", 1, 10, GetIndexMONO,	0, II_PIX_BAYGR10},
	{II_PTN_BAYER_RG10,"R G B G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYRG10},
	{II_PTN_BAYER_GB10,"G B G2 R", 1, 10, GetIndexMONO,	0, II_PIX_BAYGB10},
	{II_PTN_BAYER_BG10,"B G R G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYBG10},
	{II_PTN_BAYER_GR12,"G R G2 B", 1, 12, GetIndexMONO,	0, II_PIX_BAYGR12},
	{II_PTN_BAYER_RG12,"R G B G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYRG12},
	{II_PTN_BAYER_GB12,"G B G2 R", 1, 12, GetIndexMONO,	0, II_PIX_BAYGB12},
	{II_PTN_BAYER_BG12,"B G R G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYBG12},
	{II_PTN_RGB_8, "R G B", 3, 8, GetIndexRGB,		0, II_PIX_RGB8},		
	{II_PTN_BGR_8, "B G R", 3, 8, GetIndexBGR,		0, II_PIX_BGR8},		
	{II_PTN_RGBA_8, "R G B A", 4, 8, GetIndexRGBA,	0, II_PIX_RGBA8},	
	{II_PTN_BGRA_8, "B G R A", 4, 8, GetIndexBGRA,	0, II_PIX_BGRA8},	
	{II_PTN_RGB_10, "R G B", 3, 10, GetIndexRGB,	0, II_PIX_RGB10},	
	{II_PTN_BGR_10, "B G R", 3, 10, GetIndexBGR,	0, II_PIX_BGR10},	
	{II_PTN_RGB_12, "R G B", 3, 12, GetIndexRGB,	0, II_PIX_RGB12},	
	{II_PTN_BGR_12, "B G R", 3, 12, GetIndexBGR,	0, II_PIX_BGR12},	
	{II_PTN_RGB_10V1_PACKED, "R G B", 3, 10, GetIndexRGB, 0, II_PIX_RGB10V1_PACKED},		
	{II_PTN_RGB_10P32, "R G B", 3, 12, GetIndexRGB,	0, II_PIX_RGB10P32},		
	{II_PTN_YUV411_8_UYYVYY, "U Y Y V Y Y", 3, 8, GetIndexYUV, 0, II_PIX_YUV411_8_UYYVYY},
	{II_PTN_YUV422_8_UYVY, "U Y V Y", 3, 8, GetIndexYUV, 0, II_PIX_YUV422_8_UYVY},
	{II_PTN_YUV8_UYV, "U Y V", 3, 8, GetIndexYUV, 0, II_PIX_YUV8_UYV},
	{II_PTN_RGB_8_PLANAR, "R G B", 3, 8, GetIndexPLANAR, 0, II_PIX_RGB8_PLANAR},
	{II_PTN_RGB_10_PLANAR, "R G B", 3, 10, GetIndexPLANAR, 0, II_PIX_RGB10_PLANAR},
	{II_PTN_RGB_12_PLANAR, "R G B", 3, 12, GetIndexPLANAR, 0, II_PIX_RGB12_PLANAR},
	{II_PTN_RGB_16_PLANAR, "R G B", 3, 16, GetIndexPLANAR, 0, II_PIX_RGB16_PLANAR},
	{II_PTN_MONO_14, "Y", 1, 14, GetIndexMONO, 0, II_PIX_MONO14},
	{II_PTN_BAYER_GR10_PACKED,"G R G2 B", 1, 10, GetIndexMONO,	0, II_PIX_BAYGR10_PACKED},
	{II_PTN_BAYER_RG10_PACKED,"R G B G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYRG10_PACKED},
	{II_PTN_BAYER_GB10_PACKED,"G B G2 R", 1, 10, GetIndexMONO,	0, II_PIX_BAYGB10_PACKED},
	{II_PTN_BAYER_BG10_PACKED,"B G R G2", 1, 10, GetIndexMONO,	0, II_PIX_BAYBG10_PACKED},
	{II_PTN_BAYER_GR12_PACKED,"G R G2 B", 1, 12, GetIndexMONO,	0, II_PIX_BAYGR12_PACKED},
	{II_PTN_BAYER_RG12_PACKED,"R G B G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYRG12_PACKED},
	{II_PTN_BAYER_GB12_PACKED,"G B G2 R", 1, 12, GetIndexMONO,	0, II_PIX_BAYGB12_PACKED},
	{II_PTN_BAYER_BG12_PACKED,"B G R G2", 1, 12, GetIndexMONO,	0, II_PIX_BAYBG12_PACKED},
	{II_PTN_BAYER_GR16,"G R G2 B", 1, 16, GetIndexMONO,	0, II_PIX_BAYGR16},
	{II_PTN_BAYER_RG16,"R G B G2", 1, 16, GetIndexMONO,	0, II_PIX_BAYRG16},
	{II_PTN_BAYER_GB16,"G B G2 R", 1, 16, GetIndexMONO,	0, II_PIX_BAYGB16},
	{II_PTN_BAYER_BG16,"B G R G2", 1, 16, GetIndexMONO,	0, II_PIX_BAYBG16},
	{II_PTN_YUV422_8, "Y U Y V", 3, 8, GetIndexYUV, 0, II_PIX_YUV422_8},
	{II_PTN_RGB_16, "R G B", 3, 16, GetIndexRGB, 0, II_PIX_RGB16},	
	{II_PTN_RGB_12V1_PACKED, "R G B", 3, 10, GetIndexRGB, 0, II_PIX_RGB12V1_PACKED},		
	{II_PTN_RGB_565P, "R G B", 3, 10, GetIndexRGB,	0, II_PIX_RGB565P},		
	{II_PTN_BGR_565P, "B G R", 3, 10, GetIndexRGB,	0, II_PIX_BGR565P},		
	{II_PTN_MONO_1, "Y", 1, 1, GetIndexMONO,			0, II_PIX_MONO1P},
	{II_PTN_MONO_2, "Y", 1, 2, GetIndexMONO,			0, II_PIX_MONO2P},
	{II_PTN_MONO_4, "Y", 1, 4, GetIndexMONO,			0, II_PIX_MONO4P},
	{II_PTN_YCBCR8_CBYCR, "Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR8_CBYCR},
	{II_PTN_YCBCR422_8, "Y Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR422_8},
	{II_PTN_YCBCR411_8_CBYYCRYY, "Cb Y Y Cr Y Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR411_8_CBYYCRYY},
	{II_PTN_YCBCR601_8_CBYCR, "Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR601_8_CBYCR},
	{II_PTN_YCBCR601_422_8, "Y Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR601_422_8},
	{II_PTN_YCBCR601_411_8_CBYYCRYY, "Cb Y Y Cr Y Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR601_411_8_CBYYCRYY},
	{II_PTN_YCBCR709_8_CBYCR, "Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR709_8_CBYCR},
	{II_PTN_YCBCR709_422_8, "Y Cb Y Cr", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR709_422_8},
	{II_PTN_YCBCR709_411_8_CBYYCRYY, "Cb Y Y Cr Y Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR709_411_8_CBYYCRYY},
	{II_PTN_YCBCR422_8_CBYCRY, "Cb Y Cr Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR422_8_CBYCRY},
	{II_PTN_YCBCR601_422_8_CBYCRY, "Cb Y Cr Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR601_422_8_CBYCRY},
	{II_PTN_YCBCR709_422_8_CBYCRY, "Cb Y Cr Y", 3, 8, GetIndexYUV, 0, II_PIX_YCBCR709_422_8_CBYCRY},
//
// Custom Imperx Imaging Library pixel format defines
//--------------------------------------------------------------------------------------------------------------
	{II_PTN_TS_BGGR_WBBW_0_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_8},
	{II_PTN_TS_BGGR_WBBW_1_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_8},
	{II_PTN_TS_BGGR_WBBW_2_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_8},
	{II_PTN_TS_BGGR_WBBW_3_8,"W B W G B W G W W G W R G W R W", 1, 8, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_8},
	{II_PTN_TS_GBRG_WGGW_0_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_8},
	{II_PTN_TS_GBRG_WGGW_1_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_8},
	{II_PTN_TS_GBRG_WGGW_2_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_8},
	{II_PTN_TS_GBRG_WGGW_3_8,"W G W B G W B W W R W G R W G W", 1, 8, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_8},
	{II_PTN_TS_GRBG_WGGW_0_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_8},
	{II_PTN_TS_GRBG_WGGW_1_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_8},
	{II_PTN_TS_GRBG_WGGW_2_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_8},
	{II_PTN_TS_GRBG_WGGW_3_8,"W G W R G W R W W B W G B W G W", 1, 8, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_8},
	{II_PTN_TS_RGGB_WRRW_0_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_8},
	{II_PTN_TS_RGGB_WRRW_1_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_8},
	{II_PTN_TS_RGGB_WRRW_2_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_8},
	{II_PTN_TS_RGGB_WRRW_3_8,"W R W G R W G W W G W B G W B W", 1, 8, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_8},

	{II_PTN_TS_BGGR_WBBW_0_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_10},
	{II_PTN_TS_BGGR_WBBW_1_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_10},
	{II_PTN_TS_BGGR_WBBW_2_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_10},
	{II_PTN_TS_BGGR_WBBW_3_10,"W B W G B W G W W G W R G W R W", 1, 10, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_10},
	{II_PTN_TS_GBRG_WGGW_0_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_10},
	{II_PTN_TS_GBRG_WGGW_1_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_10},
	{II_PTN_TS_GBRG_WGGW_2_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_10},
	{II_PTN_TS_GBRG_WGGW_3_10,"W G W B G W B W W R W G R W G W", 1, 10, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_10},
	{II_PTN_TS_GRBG_WGGW_0_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_10},
	{II_PTN_TS_GRBG_WGGW_1_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_10},
	{II_PTN_TS_GRBG_WGGW_2_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_10},
	{II_PTN_TS_GRBG_WGGW_3_10,"W G W R G W R W W B W G B W G W", 1, 10, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_10},
	{II_PTN_TS_RGGB_WRRW_0_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_10},
	{II_PTN_TS_RGGB_WRRW_1_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_10},
	{II_PTN_TS_RGGB_WRRW_2_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_10},
	{II_PTN_TS_RGGB_WRRW_3_10,"W R W G R W G W W G W B G W B W", 1, 10, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_10},

	{II_PTN_TS_BGGR_WBBW_0_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_12},
	{II_PTN_TS_BGGR_WBBW_1_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_12},
	{II_PTN_TS_BGGR_WBBW_2_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_12},
	{II_PTN_TS_BGGR_WBBW_3_12,"W B W G B W G W W G W R G W R W", 1, 12, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_12},
	{II_PTN_TS_GBRG_WGGW_0_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_12},
	{II_PTN_TS_GBRG_WGGW_1_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_12},
	{II_PTN_TS_GBRG_WGGW_2_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_12},
	{II_PTN_TS_GBRG_WGGW_3_12,"W G W B G W B W W R W G R W G W", 1, 12, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_12},
	{II_PTN_TS_GRBG_WGGW_0_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_12},
	{II_PTN_TS_GRBG_WGGW_1_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_12},
	{II_PTN_TS_GRBG_WGGW_2_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_12},
	{II_PTN_TS_GRBG_WGGW_3_12,"W G W R G W R W W B W G B W G W", 1, 12, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_12},
	{II_PTN_TS_RGGB_WRRW_0_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_12},
	{II_PTN_TS_RGGB_WRRW_1_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_12},
	{II_PTN_TS_RGGB_WRRW_2_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_12},
	{II_PTN_TS_RGGB_WRRW_3_12,"W R W G R W G W W G W B G W B W", 1, 12, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_12},

	{II_PTN_MONO_10_IPX_PACKED, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10IPX_PACKED},
	{II_PTN_MONO_12_IPX_PACKED, "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12IPX_PACKED},
	{II_PTN_MONO_10_NORMALIZED, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10_NORMALIZED},
	{II_PTN_MONO_12_NORMALIZED, "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12_NORMALIZED},
	{II_PTN_YUV444_8, "Y U V A", 4, 8, GetIndexYUV, 0, II_PIX_YUV444_8},
	{II_PTN_YUV422_10, "Y U Y V", 3, 10, GetIndexYUV, 0, II_PIX_YUV422_10},

	{II_PTN_MONO_10_GEV_PACKED, "Y", 1, 10, GetIndexMONO, 0, II_PIX_MONO10GEV_PACKED},
	{II_PTN_MONO_12_GEV_PACKED, "Y", 1, 12, GetIndexMONO, 0, II_PIX_MONO12GEV_PACKED},
	{II_PTN_RGB_8_PACKED, "R G B", 3, 8, GetIndexRGB, 0, II_PIX_RGB8_PACKED},
	{II_PTN_RGB_10_PACKED, "R G B", 3, 10, GetIndexRGB, 0, II_PIX_RGB10_PACKED},
	{II_PTN_RGB_10_GEV_PACKED, "R G B", 3, 10, GetIndexRGB, 0, II_PIX_RGB10GEV_PACKED},
	{II_PTN_RGB_12_PACKED, "R G B", 3, 12, GetIndexRGB, 0, II_PIX_RGB12_PACKED},
	{II_PTN_RGB_12_GEV_PACKED, "R G B", 3, 12, GetIndexRGB, 0, II_PIX_RGB12GEV_PACKED},
	{II_PTN_BGR_16, "B G R", 3, 16, GetIndexBGR, 0, II_PIX_BGR16},
	{II_PTN_YUV422_10_PACKED, "Y U Y V", 3, 10, GetIndexYUV, 0, II_PIX_YUV422_10_PACKED},
		
	{II_PTN_BAYER_GR14,"G R G2 B", 1, 14, GetIndexMONO,	0, II_PIX_BAYGR14},
	{II_PTN_BAYER_RG14,"R G B G2", 1, 14, GetIndexMONO,	0, II_PIX_BAYRG14},
	{II_PTN_BAYER_GB14,"G B G2 R", 1, 14, GetIndexMONO,	0, II_PIX_BAYGB14},
	{II_PTN_BAYER_BG14,"B G R G2", 1, 14, GetIndexMONO,	0, II_PIX_BAYBG14},

	{II_PTN_TS_BGGR_WBBW_0_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 0, II_PIX_TS_BGGR_WBBW0_14},
	{II_PTN_TS_BGGR_WBBW_1_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 1, II_PIX_TS_BGGR_WBBW1_14},
	{II_PTN_TS_BGGR_WBBW_2_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 2, II_PIX_TS_BGGR_WBBW2_14},
	{II_PTN_TS_BGGR_WBBW_3_14,"W B W G B W G W W G W R G W R W", 1, 14, GetIndexMONO, 3, II_PIX_TS_BGGR_WBBW3_14},
	{II_PTN_TS_GBRG_WGGW_0_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 0, II_PIX_TS_GBRG_WGGW0_14},
	{II_PTN_TS_GBRG_WGGW_1_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 1, II_PIX_TS_GBRG_WGGW1_14},
	{II_PTN_TS_GBRG_WGGW_2_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 2, II_PIX_TS_GBRG_WGGW2_14},
	{II_PTN_TS_GBRG_WGGW_3_14,"W G W B G W B W W R W G R W G W", 1, 14, GetIndexMONO, 3, II_PIX_TS_GBRG_WGGW3_14},
	{II_PTN_TS_GRBG_WGGW_0_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 0, II_PIX_TS_GRBG_WGGW0_14},
	{II_PTN_TS_GRBG_WGGW_1_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 1, II_PIX_TS_GRBG_WGGW1_14},
	{II_PTN_TS_GRBG_WGGW_2_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 2, II_PIX_TS_GRBG_WGGW2_14},
	{II_PTN_TS_GRBG_WGGW_3_14,"W G W R G W R W W B W G B W G W", 1, 14, GetIndexMONO, 3, II_PIX_TS_GRBG_WGGW3_14},
	{II_PTN_TS_RGGB_WRRW_0_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 0, II_PIX_TS_RGGB_WRRW0_14},
	{II_PTN_TS_RGGB_WRRW_1_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 1, II_PIX_TS_RGGB_WRRW1_14},
	{II_PTN_TS_RGGB_WRRW_2_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 2, II_PIX_TS_RGGB_WRRW2_14},
	{II_PTN_TS_RGGB_WRRW_3_14,"W R W G R W G W W G W B G W B W", 1, 14, GetIndexMONO, 3, II_PIX_TS_RGGB_WRRW3_14},	

	{II_PTN_RGB_14, "R G B", 3, 14, GetIndexRGB,	0, II_PIX_RGB14},	
	{II_PTN_BGR_14, "B G R", 3, 14, GetIndexBGR,	0, II_PIX_BGR14},

	{II_PTN_MONO_8_PACKED, "Y", 1, 8, GetIndexMONO,			0, II_PIX_MONO8_PACKED}

};


/**< Number of pixel types used by IIL */
static unsigned long II_PIX_TYPES_NUMBER = sizeof(s_colorModelDescription) / sizeof(IIColorModelDescription);


#ifdef _DEBUG
	#include <Windows.h>
	#include <crtdbg.h>
#endif
//================================================================================
II_INLINE bool IIShiftDescr(IIColorModelDescription* cmd, unsigned long start)
{
	unsigned long index = II_GET_PIXEL_TYPE_INDEX(cmd->pixelType);
	if (start == index)
	{
		s_colorModelDescription[index] = *cmd;
		return true;
	}
	else
	{
		IIColorModelDescription cmd2 = s_colorModelDescription[index];
		s_colorModelDescription[index] = *cmd;
		return IIShiftDescr(&cmd2, start);
	}
}

//================================================================================
II_INLINE bool IISortColorModelDescription()
{
	unsigned long index = 0, i = 0;
	IIColorModelDescription cmd;
	for (i=0; i<II_PIX_TYPES_NUMBER; ++i)
	{
		index = II_GET_PIXEL_TYPE_INDEX(s_colorModelDescription[i].pixelType);
		if (index >= II_PIX_TYPES_NUMBER)
		{
#ifdef _DEBUG
			_ASSERT_EXPR(false, L"Error in definition of Pixel Types or Color Model Description\n");
#endif
			return false;
		}
		if (i != index)
		{
			cmd = s_colorModelDescription[i];
			IIShiftDescr(&cmd, i);
		}
	}
#ifdef _DEBUG
	OutputDebugStringA("Pixel Types and Color Model Description are successful defined\n");
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////
/// \defgroup ref_pixel
/// \ingroup ref_data
///@{

/// Returns the number of Pixel Types (Color Models) that are supported by this header file.
//================================================================================
/**
* \return 
* The return value is the number of Pixel Types.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE int IIGetPixelTypesNumber()
{
	return II_PIX_TYPES_NUMBER;
}

/// Defines whether the number is the pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is 'true' if pixType is pixel type.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE bool IIisPixelType(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return false;
	else if (s_colorModelDescription[descrIndex].pixelType == pixelType)
		return true;
	else
		return false;
}

/// Defines whether the pixel type is a member of a group.
//================================================================================
/**
* @param groupName Group name or some substring in Color Model Name.
* @param pixelType Pixel type.
* \return 
* The return value is 'true' if pixType is a member of group.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE bool IIisGroup(char* groupName, unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return false;
	else 
		return (::strstr(s_colorModelDescription[descrIndex].modelName, groupName)) ? true : false;
}

/// Defines color model descriptor by Pixel Type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is pointer to color model descriptor.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE const IIColorModelDescription* IIGetColorModelDescription(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return NULL;
	else
		return &s_colorModelDescription[descrIndex];
}

/// Defines color model descriptor by an index.
//================================================================================
/**
* @param index descriptor index.
* \return 
* The return value is pointer to color model descriptor.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE const IIColorModelDescription* IIGetColorModelDescr(unsigned long index)
{
	if (index >= II_PIX_TYPES_NUMBER)
		return NULL;
	else
		return &s_colorModelDescription[index];
}

/// Defines pixel type by name of color model.
//================================================================================
/**
* @param colorModelName Name of color model.
* \return 
* The return value is pixel type.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE unsigned long IIGetPixelType(char* colorModelName)
{
	if (colorModelName)
	{
		for (unsigned long i=0; i<II_PIX_TYPES_NUMBER; i++)
		{
			if (!::strcmp(s_colorModelDescription[i].modelName, colorModelName))
				return s_colorModelDescription[i].pixelType;
		}
	}

	return II_PIX_NONE_TYPE;
}

/// Defines name of color model by pixel type.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is name of color model.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE char* IIGetColorModelName(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return s_undefColorModel;
	else
		return s_colorModelDescription[descrIndex].modelName;
}

/// Defines sequence of channels.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is sequence of channels.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE const char* IIGetChannelSequence(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return s_undefColorModel;
	else
		return s_colorModelDescription[descrIndex].channelSequence;
}

/// Defines number of channels.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is number of channels.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE int IIGetChannelsNumber(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].channels;
}

/// Defines depth of color channel.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is depth of color channel.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE int IIGetChannelsDepth(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].depth;
}

/// Defines start position in a CFA.
//================================================================================
/**
* @param pixelType Pixel type.
* \return 
* The return value is start position in a CFA.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE int IIGetStartPosition(unsigned long pixelType)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].startPos;
}

/// Defines index of color channel.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chName Channel name.
* \return 
* The return value is index of color channel.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE int IIGetChannelIndex(unsigned long pixelType, short chName)
{
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		return s_colorModelDescription[descrIndex].GetIndex(chName);
}

/// Checks channel names.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chNames Array of channel names.
* @param channels Number of checked names.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is -1.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE II_RESULT IICheckChannelNames(unsigned long pixelType, short* chNames, int channels)
{
	GetIndexChannelTYPE*	GetIndex = NULL;
	unsigned long descrIndex = II_GET_PIXEL_TYPE_INDEX(pixelType);
	if (descrIndex >= II_PIX_TYPES_NUMBER)
		return -1;
	else
		GetIndex = s_colorModelDescription[descrIndex].GetIndex;

	for (int i=0; i<channels; i++)
	{
		if (GetIndex(chNames[i]) < 0)
			return -1;
	}
	return 0;
}

/// Converts string to array of channel names.
//================================================================================
/**
* @param nameStr String of channel names.
* @param sep Separator of channel names in the string.
* @param chNames Array of channel names.
* @param channels Number of channel names.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is -1.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE II_RESULT IIConvertChannelStr(char* nameStr, char* sep, short* chNames, int* channels)
{
	if (!nameStr || !chNames || !channels)
		return -1;
	int i = 0;
	int ii = *channels;
	char *token1, *next_token1;
#ifdef LINUX
	token1 = strtok_r(nameStr, (const char*)sep, &next_token1);
#else	
	token1 = strtok_s(nameStr, sep, &next_token1);
#endif
	while (token1 != NULL)
	{
		if (i < ii)
			chNames[i++] = *((short*)token1);
#ifdef LINUX			
		token1 = strtok_r(NULL, sep, &next_token1);
#else
		token1 = strtok_s(NULL, sep, &next_token1);
#endif
	}
	*channels = i;
	return 0;
}

/// Gets channel name.
//================================================================================
/**
* @param pixelType Pixel type.
* @param chnlIndx Channel index.
* \return 
* The return value is channel name.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE short IIGetChannelName(unsigned long pixelType, int chnlIndx)
{
	const char* tmpChnlStr = IIGetChannelSequence(pixelType);
	char chnlStr[16];
#ifdef LINUX
    strncpy(chnlStr, tmpChnlStr,16);
#else
	::strncpy_s(chnlStr, 16, tmpChnlStr, 16);
#endif	
	short chNames[MAX_IMAGE_CHANNELS];
	int channels	= MAX_IMAGE_CHANNELS;
	if (0 != IIConvertChannelStr(chnlStr, " ", chNames, &channels))
	{
		return IIPC_NONE;
	}
	if (0 > chnlIndx || chnlIndx >= channels)
		return IIPC_NONE;
	else
		return chNames[chnlIndx];
}

/// Converts string to Channels Of Intrest (COI).
//================================================================================
/**
* @param pixelType Pixel type.
* @param nameStr String of channel names.
* @param sep Separator of channel names in the string.
* @param coi Channels of intrest.
* \return 
* If the function succeeds, the return value is 0.
* If the function fails, the return value is -1.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE II_RESULT IIConvertChannelStr(unsigned long pixelType, char* nameStr, char* sep, IICOI* coi)
{
	char *token1, *next_token1;
	short chName = IIPC_NONE;
	int indx, ii, i = 0;
	if (!nameStr || !sep || !coi || !IIisPixelType(pixelType))
		return -1;
	ii = IIGetChannelsNumber(pixelType);
#ifdef LINUX
	token1 = strtok_r(nameStr, sep, &next_token1);
#else	
	token1 = strtok_s(nameStr, sep, &next_token1);
#endif
	while (token1 != NULL)
	{
		if (i >= ii)
			break;
		chName = *((short*)token1);
		indx = IIGetChannelIndex(pixelType, chName);
		if (-1 == indx)
		{
			coi->indNumber = 0;
			return -1;
		}
		coi->indexes[i++] = indx;
#ifdef LINUX
		token1 = strtok_r(NULL, sep, &next_token1);
#else		
		token1 = strtok_s(NULL, sep, &next_token1);
#endif		
	}
	coi->indNumber = i;
	return 0;
}

/// Converts array of channel names to Channels Of Intrest (COI).
//================================================================================
/**
* @param coi Channels of intrest.
* @param chNames Array of channel names.
* @param channels Number of channel names.
* @param pixelType Pixel type.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE bool IICOIFrom(IICOI* coi, short* chNames, int* channels, unsigned long pixelType)
{
	int indx, ii, i;
	if (!chNames || !channels || !coi || !IIisPixelType(pixelType))
		return false;
	ii = IIGetChannelsNumber(pixelType);
	ii = MIN(*channels, ii);
	for (i=0; i<ii; i++)
	{
		indx = IIGetChannelIndex(pixelType, chNames[i]);
		if (-1 == indx)
		{
			coi->indNumber = 0;
			return false;
		}
		coi->indexes[i] = indx;
	}
	coi->indNumber = ii;
	
	return true;
}

/// Defines Channels Of Intrest for pixel type.
//================================================================================
/**
* @param coi Channels of intrest.
* @param pixelType Pixel type.
* \return 
* If the function succeeds, the return value is 'true'.
* If the function fails, the return value is 'false'.
* \req
* \table2{Header, IpxPixelType.h<BR/>IpxBaseDef.h}
* \endtable
*/
II_INLINE bool IICOIFrom(IICOI* coi, unsigned long pixelType)
{
	int ii, i;
	if (!coi || !IIisPixelType(pixelType))
		return false;
	ii = IIGetChannelsNumber(pixelType);
	for (i=0; i<ii; i++)
	{
		coi->indexes[i] = i;
	}
	coi->indNumber = ii;
	
	return true;
}

///@}

//////////////////////////////////////////////////////////////////////
#endif // !defined(IpxPixelType_H__INCLUDED_)
