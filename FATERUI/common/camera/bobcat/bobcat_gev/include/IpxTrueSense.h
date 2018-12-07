// IpxTrueSense.h: TrueSense demosaicking.
// Author: Maxim Bokov (maxb@imperx.com or maxim.bokoff@gmail.com)
//////////////////////////////////////////////////////////////////////

#ifndef _TRUE_SENSE_H_
#define _TRUE_SENSE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef TS_EXTERN_C
    #ifdef __cplusplus
        #define TS_EXTERN_C extern "C"
    #else
        #define TS_EXTERN_C
    #endif
#endif

#ifndef TS_CALL
#ifdef LINUX
	#define TS_CALL
#else
    #define TS_CALL __stdcall
#endif    
#else
    #define TS_CALL
#endif

#ifdef TS_EXPORTS

    // Define the export symbol
	#if !defined(TS_STATIC) && defined(WIN32)
		#define TS_API __declspec( dllexport )
	#else
		#define TS_API
	#endif // TS_STATIC

	#ifndef _M_AMD64

		// For 32b DLL only, user-friendly usage in .NET 
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_16u8u_C1C3R=_TrueSenseToRGB_16u8u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_16u8u_C1C4R=_TrueSenseToRGB_16u8u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_16u8u_C1C3R=_TrueSenseToBGR_16u8u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_16u8u_C1C4R=_TrueSenseToBGR_16u8u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_16u_C1C3R=_TrueSenseToRGB_16u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_16u_C1C4R=_TrueSenseToRGB_16u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_16u_C1C3R=_TrueSenseToBGR_16u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_16u_C1C4R=_TrueSenseToBGR_16u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_8u_C1C3R=_TrueSenseToRGB_8u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToRGB_8u_C1C4R=_TrueSenseToRGB_8u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_8u_C1C3R=_TrueSenseToBGR_8u_C1C3R@60")
		#pragma comment(linker, "/EXPORT:TrueSenseToBGR_8u_C1C4R=_TrueSenseToBGR_8u_C1C4R@60")
		#pragma comment(linker, "/EXPORT:TrueSense_DefaultValues=_TrueSense_DefaultValues@4")
		#pragma comment(linker, "/EXPORT:TrueSenseToColor=_TrueSenseToColor@20")

	#endif 

#else // TS_EXPORTS

    // Define the import symbol
	#if !defined(TS_STATIC) && defined(WIN32)
		#define TS_API __declspec( dllimport )
	#else
		#define TS_API
	#endif // TS_STATIC

#endif // TS_EXPORTS

#include "IpxImage.h"
#include "IpxDemosaicingDef.h"

#define NOISE_CURVE_PIECES_NUM 5

//////////////////////////////////////////////////////
/// \defgroup ipxts_ref TrueSense Converter Reference
/// \ingroup ipxdemosaicing
/// The following items are exists in TrueSense Converter
///@{
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
/// \defgroup ipxts_enum TrueSense Converter Enumerations
/// The following enumerations are used in TrueSense Converter:
///@{
//////////////////////////////////////////////////////

/// Type of TrueSense CFA to RGB conversion algorithm
typedef enum 
{
	TSASimpleF,		/**< Simple algorithm. Average quality, high speed. You can set tonescale table in TrueSenseParam structure. */
	TSASimpleS,		/**< Simple Quality algorithm. Average quality, high speed. You can set tonescale table and white balance coefficients in TrueSenseParam structure. */
	TSABayerLike,	/**< Simple Bayer-like algorithm. Average quality, high speed. You can set tonescale table in TrueSenseParam structure. */
	TSAMedium,		/**< High Quality algorithm. High quality, medium speed. You can set all of the adjusting parameters in TrueSenseParam structure. */
	TSAQuality,		/**< High Quality algorithm. High quality, very low speed. You can set all of the adjusting parameters in TrueSenseParam structure.  */
} TrueSenseAlgType;

/// Parameter container fed to top level image processing
typedef struct tagTrueSenseParam
{
	int threads_num;			/**< Quantity of threads used in calcucation. Default value is 0, it means maximum number of available threads */
	int normalizationEnable;	/**< Enable normalization. 0 - off, 1 - on. Default value is off. */
	int horMirrored;			/**< If image is mirrored horrizontally. Default value is 0. */
	int verMirrored;			/**< If image is mirrored vertically. Default value is 0. */
    int monoEnable;				/**< Switch on monochrome processing instead of color processing. 0 - color, 1 - monochrome. Reserved. */
	int impulseFilterEnable;	/**< Enable the impulse filter processing. 0 - off, 1 - on.*/
	int sharpnessEnable;		/**< Enable the sharpness processing. 0 - off, 1 - on.*/
    int darkFloor;				/**< Dark floor of raw image, fetched from raw file header. */
    float redGain;				/**< Red gain of white balance. */
    float greenGain;			/**< Green gain of white balance. */
    float blueGain;				/**< Blue gain of white balance. */
    float panGain;				/**< Panchromatic gain of white balance. It should be set as 1 currently. */
	float globalGain;			/**< Digital gain. It will be applied to processing if more than 1.0. */
    float analogGain;			/**< Actual sensor gain of raw image, fetched from raw file header. */
    float ISOAnalogGain[NOISE_CURVE_PIECES_NUM];	/**< Sensor gain array of typical ISO levels,
    													used to interpolate intermediate noise variation slope and intercept.
    													Example: (NOISE_CURVE_PIECES_NUM = 5)
    															ISO400_AnalogGain = 11.04 dB
    															ISO800_AnalogGain = 17.69 dB
    															ISO1600_AnalogGain = 23.96 dB
    															ISO3200_AnalogGain = 29.91 dB
    															ISO6400_AnalogGain = 36.77 dB */
    float ISOPanSlope[NOISE_CURVE_PIECES_NUM];		/**< Noise variation slope of panchromatic channel at typical sensor gains,
    													used to interpolate intermediate noise variation slope.
    													Example: (NOISE_CURVE_PIECES_NUM = 5)
    															ISO400_PanSlope = 0.31793097
    															ISO800_PanSlope = 0.6009852
    															ISO1600_PanSlope = 1.16587611
    															ISO3200_PanSlope = 2.26059552
    															ISO6400_PanSlope = 5.11044291 */
    float ISOPanIntercept[NOISE_CURVE_PIECES_NUM];	/**< Noise variation intercept of panchromatic channel at typical sensor gains,
    													used to interpolate intermediate noise variation intercept.
    													Example: (NOISE_CURVE_PIECES_NUM = 5)
    															ISO400_PanIntercept = -25.07685652
    															ISO800_PanIntercept = 17.01752105
    															ISO1600_PanIntercept = 185.43026
    															ISO3200_PanIntercept = 831.07495077
    															ISO6400_PanIntercept = 4154.73883603 */
    float ISOColorSlope[NOISE_CURVE_PIECES_NUM];	/**< Noise variation slope of color channel at typical sensor gains,
    													used to interpolate intermediate noise variation slope.
    													Example: (NOISE_CURVE_PIECES_NUM = 5)
    															ISO400_ColorSlope = 0.16289523
    															ISO800_ColorSlope = 0.30242107
    															ISO1600_ColorSlope = 0.58180185
    															ISO3200_ColorSlope = 1.15281985
    															ISO6400_ColorSlope = 2.53400236 */
    float ISOColorIntercept[NOISE_CURVE_PIECES_NUM];/**< Noise variation intercept of color channel at typical sensor gains,
    													used to interpolate intermediate noise variation intercept.
    													Example: (NOISE_CURVE_PIECES_NUM = 5)
    															ISO400_ColorIntercept = -2.97408598
    															ISO800_ColorIntercept = 15.97559859
    															ISO1600_ColorIntercept = 92.84640595
    															ISO3200_ColorIntercept = 399.49923562
    															ISO6400_ColorIntercept = 2080.24259272 */
    int panRadius0;		/**< Pixel radius for the sigma filter of first round panchromatic channel noise cleaning,
    						0 means bypass current round cleaning. Example: 1 */
    int panRadius1;		/**< Pixel radius for the sigma filter of second round panchromatic channel noise cleaning,
    						0 means bypass current round cleaning. Example: 0 */
    int panRadius2;		/**< Pixel radius for the sigma filter of third round panchromatic channel noise cleaning,
    						0 means bypass current round cleaning. Example: 0 */
    float panSigma0;	/**< Scalar for the sigma filter of first round panchromatic channel noise cleaning, Example: 1.7 */
    float panSigma1;	/**< Scalar for the sigma filter of second round panchromatic channel noise cleaning, Example: 0 */
    float panSigma2;	/**< Scalar for the sigma filter of second round panchromatic channel noise cleaning, Example: 0 */
    int colorRadius0;	/**< Pixel radius for the sigma filter of first round color channel noise cleaning,
    						0 means bypass current round cleaning. Example: 1 */
    int colorRadius1;	/**< Pixel radius for the sigma filter of second round color channel noise cleaning,
    						0 means bypass current round cleaning. Example: 2 */
    int colorRadius2;	/**< Pixel radius for the sigma filter of third round color channel noise cleaning,
    						0 means bypass current round cleaning. Example: 0 */
    float colorSigma0;	/**< Scalar for the sigma filter of first round color channel noise cleaning, Example: 2 */
    float colorSigma1;	/**< Scalar for the sigma filter of second round color channel noise cleaning, Example: 2 */
    float colorSigma2;	/**< Scalar for the sigma filter of third round color channel noise cleaning, Example: 0 */
    float RR;			/**< Red-blue coefficient of color correction matrix, Example: 1.657 */
    float RG;			/**< Red-green coefficient of color correction matrix, Example: -0.5325 */
    float RB;			/**< Red-blue coefficient of color correction matrix, Example: -0.1245 */
    float GR;			/**< Green-red coefficient of color correction matrix, Example: -0.106 */
    float GG;			/**< Green-green coefficient of color correction matrix, Example: 1.443 */
    float GB;			/**< Green-blue coefficient of color correction matrix, Example: -0.337 */
    float BR;			/**< Blue-red coefficient of color correction matrix, Example: 0.131 */
    float BG;			/**< Blue-green coefficient of color correction matrix, Example: -0.445 */
    float BB;			/**< Blue-blue coefficient of color correction matrix, Example: 1.314 */
    float sharpenParam;	/**< Sharp parameter Example: 40 */
    float maxSharpen;	/**< Sharp maximal threshold Example: 1.0 */
    float highLumaNoise;/**< Noise threshold Example: 2.0 */
    float lowLumaNoise;	/**< Noise threshold Example: 0.5 */
	short* toneScale;	/**< Tonescale look-up table buffer from outside.
							If it equals to 0, the processing will use default tonescale table(Tonescale12Table[])(from 12 bit to 8 bit).
							Contains 4096 elements*/
}TrueSenseParam;

///@}

/////////////////////////////////////////////////////////
/// \defgroup ipxts_func TrueSense Converter Functions
/// The following functions are presented in TrueSense Converter to perform TrueSense CFA to RGB conversion:
///@{
/////////////////////////////////////////////////////////

///  This function converts 8bpp TrueSense CFA image data to 3-channels 8bpp RGB image (RGB24)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 8bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Not used in this function. Always 8 bpp is used.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values).
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_8u_C1C3R(const Ipx8u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
	
///  This function converts 8bpp TrueSense CFA image data to 4-channels 8bpp RGB image (RGB32)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 8bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Not used in this function. Always 8 bpp is used.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_8u_C1C4R(const Ipx8u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///  This function converts 8bpp TrueSense CFA image data to 3-channels 8bpp BGR image (BGR24)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 8bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Not used in this function. Always 8 bpp is used.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_8u_C1C3R(const Ipx8u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///  This function converts 8bpp TrueSense CFA image data to 4-channels 8bpp BGR image (BGR32)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 8bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Not used in this function. Always 8 bpp is used.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	*\code
	* // Output image bitdepth
	* unsigned int dstBytesPerPix = (dstBuff.GetImage()->GetBitsPerPixel()+7)/8;
	* // Calculate pattern grid start
	* unsigned char grid = m_patStart; 
	* // Source buffer
	* const unsigned char* pSrc = (unsigned char*)srcImg->GetDataPointer();
	* // Source ROI and size
	* IpxRect srcRoi;
	* IpxSize srcSize;
	* srcRoi.x = srcImg->GetOffsetX();
	* srcRoi.y = srcImg->GetOffsetY();
	* srcRoi.width = srcSize.width = srcImg->GetWidth();
	* srcRoi.height = srcSize.height = srcImg->GetHeight();
	* // Source stride
	* int srcStep = ((srcImg->GetWidth()*srcImg->GetBitsPerPixel()+7)/8) + srcImg->GetPaddingX();
	* // Source bitDepth
	* int bitDepth = srcImg->GetBitsPerPixel();
	* // Destination buffer
	* unsigned char* pDst = (unsigned char*)dstBuff.GetDataPointer();
	* // Destination stride
	* int dstStep = (srcImg->GetWidth()*dstBytesPerPix) ;
	* 
	* IpxStatus convRes = TrueSenseToBGR_8u_C1C4R(pSrc, srcRoi, srcSize, srcStep, bitDepth, pDst, dstStep, grid, m_algo);
	* // Check return value 
	* if (convRes!=ipxStsOk)
	* {
	*     // Indicate error here
	*     // ....
	* }
	\endcode
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_8u_C1C4R(const Ipx8u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
						
// 16 bit

///  This function converts 16bpp TrueSense CFA image data to 3-channels 16bpp RGB image (RGB48)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be the same as source bitdepth.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_16u_C1C3R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx16u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
						
///  This function converts 16bpp TrueSense CFA image data to 4-channels 16bpp RGB image (RGB64)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be the same as source bitdepth.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_16u_C1C4R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx16u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
					
///  This function converts 16bpp TrueSense CFA image data to 3-channels 16bpp BGR image (BGR48)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output BGR image bitdepth will be the same as source bitdepth.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_16u_C1C3R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx16u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///  This function converts 16bpp TrueSense CFA image data to 4-channels 16bpp BGR image (BGR64)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output BGR image bitdepth will be the same as source bitdepth.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_16u_C1C4R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx16u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

// 16 bit IN 8 bit OUT

///  This function converts 16bpp TrueSense CFA image data to 3-channels 8bpp RGB image (RGB24)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be 8.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_16u8u_C1C3R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
						
///  This function converts 16bpp TrueSense CFA image data to 4-channels 8bpp RGB image (RGB32)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be 8.
	* @param pDst Pointer to destination image data buffer, where output RGB data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToRGB_16u8u_C1C4R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);
					
///  This function converts 16bpp TrueSense CFA image data to 3-channels 8bpp BGR image (BGR24)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be 8.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_16u8u_C1C3R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///  This function converts 16bpp TrueSense CFA image data to 4-channels 8bpp BGR image (BGR32)
	/**
	* @param pSrc Pointer to source image data buffer, containing RAW image data in 16bpp, TrueSense CFA format
	* @param srcRoi Region of interest in source image data buffer, that should be converted
	* @param srcSize Size of image in source image data buffer
	* @param srcStep Stride value of source image row in bytes
	* @param bitDepth Bit depth of source image. Can be 10, 12, 14, 16 bits for Bobcat cameras. Output RGB image bitdepth will be 8.
	* @param pDst Pointer to destination image data buffer, where output BGR data will be copied
	* @param dstStep Stride value of destination image row in bytes
	* @param grid TrueSense array start pixel number. See notes about the details.
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*\sample
	* See \link TrueSenseToBGR_8u_C1C4R \endlink
	*\note
	* TrueSense array start pixel number, defined in <i>grid</i> argument, should be set according the picture below
	
	* \note ![TrueSense CFA](/grid.png "TrueSense CFA")
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToBGR_16u8u_C1C4R(const Ipx16u* pSrc, IpxRect srcRoi, IpxSize srcSize, int srcStep, int bitDepth,
						Ipx8u* pDst, int dstStep, unsigned char grid, TrueSenseParam* param = 0, TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///  This function fills default values in TrueSenseParam structure
	/**
	* @param param Pointer to an object of TrueSenseParam structure
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSense_DefaultValues(TrueSenseParam* param);

/// This function converts TrueSense CFA image data to color image
	/**
	* @param pSrc Pointer to source image, containing RAW image data of TrueSense CFA format
	* @param pDst Pointer to destination image
	* @param param Some parameters for adjusting the processing. (Use TrueSenseToRGB_DefaultValues function to get default values)
	* @param algo Type of TrueSense CFA demosaicking conversion algorithm
	* @param isClean Defines if function should cleanup internal data after the call. If this function is used
	in multithreaded application then this parameter should be set as true.
	
	*\return
	* Function returns \link IpxStatus \endlink error code.
	*/
TS_EXTERN_C TS_API IpxStatus TS_CALL TrueSenseToColor(const IIImage* pSrc, IIImage* pDst, TrueSenseParam* param = 0,
													  TrueSenseAlgType algo = TSASimpleF, bool isClean = false);

///@}
///@}

/////////////////////////////////////////////////////////////////////////////
#endif // _TRUE_SENSE_H_
