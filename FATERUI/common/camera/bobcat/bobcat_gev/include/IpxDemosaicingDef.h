// IpxDemosaicingDef.h: common types of data for demosaicing process.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#ifndef _IPXDEMOSAICINGDEF_H_
#define _IPXDEMOSAICINGDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////
/// \defgroup ipxdemosaicing_enum Demosaicing Enumerations
/// The following enumerations are used in Demosaicing:
///@{
//////////////////////////////////////////////////////

/// The following enumerator defines the return value of Demosaicing functions. 
/// Negative value means error, positive value means warning, zero means success
typedef enum {
    /// Errors:  
    ipxStsNotSupportedModeErr    = -9999,/**< The requested mode is currently not supported  */
    ipxStsCpuNotSupportedErr     = -9998,/**< The target CPU is not supported */

    ipxStsLoadDynErr             = -221, /**< Fatal error during loading of dynamic library */

    ipxStsPointAtInfinity        = -220, /**< Point at infinity is detected  */

    ipxStsI18nUnsupportedErr     = -219, /**< Internationalization (i18n) is not supported                                                                  */
    ipxStsI18nMsgCatalogOpenErr  = -218, /**< Message Catalog cannot be opened, for extended information use errno in Linux and GetLastError in Windows* */
    ipxStsI18nMsgCatalogCloseErr = -217, /**< Message Catalog cannot be closed, for extended information use errno in Linux and GetLastError in Windows* */

    ipxStsUnknownStatusCodeErr   = -216, /**< Unknown Status Code */

    ipxStsOFBSizeErr             = -215, /**< Wrong value for crypto OFB block size */
    ipxStsLzoBrokenStreamErr     = -214, /**< LZO safe decompression function cannot decode LZO stream */

    ipxStsRoundModeNotSupportedErr  = -213, /**< Unsupported round mode*/
    ipxStsDecimateFractionErr    = -212, /**< Unsupported fraction in Decimate */
    ipxStsWeightErr              = -211, /**< Wrong value of weight */

    ipxStsQualityIndexErr        = -210, /**< Quality Index can't be calculated for image filled with constant */
    ipxStsIIRPassbandRippleErr   = -209, /**< Ripple in passband for Chebyshev1 design is less than zero, equal to zero or greater than 29 */
    ipxStsFilterFrequencyErr     = -208, /**< Cut of frequency of filter is less zero, equal to zero or greater than 0.5 */
    ipxStsFIRGenOrderErr         = -207, /**< Order of an FIR filter for design them is less than one                    */
    ipxStsIIRGenOrderErr         = -206, /**< Order of an IIR filter for design them is less than one or greater than 12 */

    ipxStsConvergeErr            = -205, /**< The algorithm does not converge*/
    ipxStsSizeMatchMatrixErr     = -204, /**< Unsuitable sizes of the source matrices*/
    ipxStsCountMatrixErr         = -203, /**< Count value is negative or equal to 0*/
    ipxStsRoiShiftMatrixErr      = -202, /**< RoiShift value is negative or not dividend to size of data type*/

    ipxStsResizeNoOperationErr   = -201, /**< One of the output image dimensions is less than 1 pixel */
    ipxStsSrcDataErr             = -200, /**< The source buffer contains unsupported data */
    ipxStsMaxLenHuffCodeErr      = -199, /**< Huff: Max length of Huffman code is more than expected one */
    ipxStsCodeLenTableErr        = -198, /**< Huff: Invalid codeLenTable */
    ipxStsFreqTableErr           = -197, /**< Huff: Invalid freqTable */

    ipxStsIncompleteContextErr   = -196, /**< Crypto: set up of context is not complete */

    ipxStsSingularErr            = -195, /**< Matrix is singular */
    ipxStsSparseErr              = -194, /**< Tap positions are not in ascending order, negative or repeated*/
    ipxStsBitOffsetErr           = -193, /**< Incorrect bit offset value */
    ipxStsQPErr                  = -192, /**< Incorrect quantization parameter */
    ipxStsVLCErr                 = -191, /**< Illegal VLC or FLC during stream decoding */
    ipxStsRegExpOptionsErr       = -190, /**< RegExp: Options for pattern are incorrect */
    ipxStsRegExpErr              = -189, /**< RegExp: The structure pRegExpState contains wrong data */
    ipxStsRegExpMatchLimitErr    = -188, /**< RegExp: The match limit has been exhausted */
    ipxStsRegExpQuantifierErr    = -187, /**< RegExp: wrong quantifier */
    ipxStsRegExpGroupingErr      = -186, /**< RegExp: wrong grouping */
    ipxStsRegExpBackRefErr       = -185, /**< RegExp: wrong back reference */
    ipxStsRegExpChClassErr       = -184, /**< RegExp: wrong character class */
    ipxStsRegExpMetaChErr        = -183, /**< RegExp: wrong metacharacter */
    ipxStsStrideMatrixErr        = -182,  /**< Stride value is not positive or not dividend to size of data type */
    ipxStsCTRSizeErr             = -181,  /**< Wrong value for crypto CTR block size */
    ipxStsJPEG2KCodeBlockIsNotAttached =-180, /**< codeblock parameters are not attached to the state structure */
    ipxStsNotPosDefErr           = -179,      /**< Not positive-definite matrix */

    ipxStsEphemeralKeyErr        = -178, /**< ECC: Bad ephemeral key   */
    ipxStsMessageErr             = -177, /**< ECC: Bad message digest  */
    ipxStsShareKeyErr            = -176, /**< ECC: Invalid share key   */
    ipxStsIvalidPublicKey        = -175, /**< ECC: Invalid public key  */
    ipxStsIvalidPrivateKey       = -174, /**< ECC: Invalid private key */
    ipxStsOutOfECErr             = -173, /**< ECC: Point out of EC     */
    ipxStsECCInvalidFlagErr      = -172, /**< ECC: Invalid Flag        */

    ipxStsMP3FrameHeaderErr      = -171,  /**< Error in fields IppMP3FrameHeader structure */
    ipxStsMP3SideInfoErr         = -170,  /**< Error in fields IppMP3SideInfo structure */

    ipxStsBlockStepErr           = -169,  /**< Step for Block less than 8 */
    ipxStsMBStepErr              = -168,  /**< Step for MB less than 16 */

    ipxStsAacPrgNumErr           = -167,  /**< AAC: Invalid number of elements for one program   */
    ipxStsAacSectCbErr           = -166,  /**< AAC: Invalid section codebook                     */
    ipxStsAacSfValErr            = -164,  /**< AAC: Invalid scale factor value                    */
    ipxStsAacCoefValErr          = -163,  /**< AAC: Invalid quantized coefficient value          */
    ipxStsAacMaxSfbErr           = -162,  /**< AAC: Invalid coefficient index  */
    ipxStsAacPredSfbErr          = -161,  /**< AAC: Invalid predicted coefficient index  */
    ipxStsAacPlsDataErr          = -160,  /**< AAC: Invalid pulse data attributes  */
    ipxStsAacGainCtrErr          = -159,  /**< AAC: Gain control not supported  */
    ipxStsAacSectErr             = -158,  /**< AAC: Invalid number of sections  */
    ipxStsAacTnsNumFiltErr       = -157,  /**< AAC: Invalid number of TNS filters  */
    ipxStsAacTnsLenErr           = -156,  /**< AAC: Invalid TNS region length  */
    ipxStsAacTnsOrderErr         = -155,  /**< AAC: Invalid order of TNS filter  */
    ipxStsAacTnsCoefResErr       = -154,  /**< AAC: Invalid bit-resolution for TNS filter coefficients  */
    ipxStsAacTnsCoefErr          = -153,  /**< AAC: Invalid TNS filter coefficients  */
    ipxStsAacTnsDirectErr        = -152,  /**< AAC: Invalid TNS filter direction  */
    ipxStsAacTnsProfileErr       = -151,  /**< AAC: Invalid TNS profile  */
    ipxStsAacErr                 = -150,  /**< AAC: Internal error  */
    ipxStsAacBitOffsetErr        = -149,  /**< AAC: Invalid current bit offset in bitstream  */
    ipxStsAacAdtsSyncWordErr     = -148,  /**< AAC: Invalid ADTS syncword  */
    ipxStsAacSmplRateIdxErr      = -147,  /**< AAC: Invalid sample rate index  */
    ipxStsAacWinLenErr           = -146,  /**< AAC: Invalid window length (not short or long)  */
    ipxStsAacWinGrpErr           = -145,  /**< AAC: Invalid number of groups for current window length  */
    ipxStsAacWinSeqErr           = -144,  /**< AAC: Invalid window sequence range  */
    ipxStsAacComWinErr           = -143,  /**< AAC: Invalid common window flag  */
    ipxStsAacStereoMaskErr       = -142,  /**< AAC: Invalid stereo mask  */
    ipxStsAacChanErr             = -141,  /**< AAC: Invalid channel number  */
    ipxStsAacMonoStereoErr       = -140,  /**< AAC: Invalid mono-stereo flag  */
    ipxStsAacStereoLayerErr      = -139,  /**< AAC: Invalid this Stereo Layer flag  */
    ipxStsAacMonoLayerErr        = -138,  /**< AAC: Invalid this Mono Layer flag  */
    ipxStsAacScalableErr         = -137,  /**< AAC: Invalid scalable object flag  */
    ipxStsAacObjTypeErr          = -136,  /**< AAC: Invalid audio object type  */
    ipxStsAacWinShapeErr         = -135,  /**< AAC: Invalid window shape  */
    ipxStsAacPcmModeErr          = -134,  /**< AAC: Invalid PCM output interleaving indicator  */
    ipxStsVLCUsrTblHeaderErr          = -133,  /**< VLC: Invalid header inside table */
    ipxStsVLCUsrTblUnsupportedFmtErr  = -132,  /**< VLC: Unsupported table format */
    ipxStsVLCUsrTblEscAlgTypeErr      = -131,  /**< VLC: Unsupported Ecs-algorithm */
    ipxStsVLCUsrTblEscCodeLengthErr   = -130,  /**< VLC: Incorrect Esc-code length inside table header */
    ipxStsVLCUsrTblCodeLengthErr      = -129,  /**< VLC: Unsupported code length inside table */
    ipxStsVLCInternalTblErr           = -128,  /**< VLC: Invalid internal table */
    ipxStsVLCInputDataErr             = -127,  /**< VLC: Invalid input data */
    ipxStsVLCAACEscCodeLengthErr      = -126,  /**< VLC: Invalid AAC-Esc code length */
    ipxStsNoiseRangeErr         = -125,  /**< Noise value for Wiener Filter is out range. */
    ipxStsUnderRunErr           = -124,  /**< Data under run error */
    ipxStsPaddingErr            = -123,  /**< Detected padding error shows the possible data corruption */
    ipxStsCFBSizeErr            = -122,  /**< Wrong value for crypto CFB block size */
    ipxStsPaddingSchemeErr      = -121,  /**< Invalid padding scheme  */
    ipxStsInvalidCryptoKeyErr   = -120,  /**< A compromised key causes suspansion of requested cryptographic operation  */
    ipxStsLengthErr             = -119,  /**< Wrong value of string length */
    ipxStsBadModulusErr         = -118,  /**< Bad modulus caused a module inversion failure */
    ipxStsLPCCalcErr            = -117,  /**< Linear prediction could not be evaluated */
    ipxStsRCCalcErr             = -116,  /**< Reflection coefficients could not be computed */
    ipxStsIncorrectLSPErr       = -115,  /**< Incorrect Linear Spectral Pair values */
    ipxStsNoRootFoundErr        = -114,  /**< No roots are found for equation */
    ipxStsJPEG2KBadPassNumber   = -113,  /**< Pass number exceeds allowed limits [0,nOfPasses-1] */
    ipxStsJPEG2KDamagedCodeBlock= -112,  /**< Codeblock for decoding is damaged */
    ipxStsH263CBPYCodeErr       = -111,  /**< Illegal Huffman code during CBPY stream processing */
    ipxStsH263MCBPCInterCodeErr = -110,  /**< Illegal Huffman code during MCBPC Inter stream processing */
    ipxStsH263MCBPCIntraCodeErr = -109,  /**< Illegal Huffman code during MCBPC Intra stream processing */
    ipxStsNotEvenStepErr        = -108,  /**< Step value is not pixel multiple */
    ipxStsHistoNofLevelsErr     = -107,  /**< Number of levels for histogram is less than 2 */
    ipxStsLUTNofLevelsErr       = -106,  /**< Number of levels for LUT is less than 2 */
    ipxStsMP4BitOffsetErr       = -105,  /**< Incorrect bit offset value */
    ipxStsMP4QPErr              = -104,  /**< Incorrect quantization parameter */
    ipxStsMP4BlockIdxErr        = -103,  /**< Incorrect block index */
    ipxStsMP4BlockTypeErr       = -102,  /**< Incorrect block type */
    ipxStsMP4MVCodeErr          = -101,  /**< Illegal Huffman code during MV stream processing */
    ipxStsMP4VLCCodeErr         = -100,  /**< Illegal Huffman code during VLC stream processing */
    ipxStsMP4DCCodeErr          = -99,   /**< Illegal code during DC stream processing */
    ipxStsMP4FcodeErr           = -98,   /**< Incorrect fcode value */
    ipxStsMP4AlignErr           = -97,   /**< Incorrect buffer alignment            */
    ipxStsMP4TempDiffErr        = -96,   /**< Incorrect temporal difference         */
    ipxStsMP4BlockSizeErr       = -95,   /**< Incorrect size of block or macroblock */
    ipxStsMP4ZeroBABErr         = -94,   /**< All BAB values are zero             */
    ipxStsMP4PredDirErr         = -93,   /**< Incorrect prediction direction        */
    ipxStsMP4BitsPerPixelErr    = -92,   /**< Incorrect number of bits per pixel    */
    ipxStsMP4VideoCompModeErr   = -91,   /**< Incorrect video component mode        */
    ipxStsMP4LinearModeErr      = -90,   /**< Incorrect DC linear mode */
    ipxStsH263PredModeErr       = -83,   /**< Prediction Mode value error                                       */
    ipxStsH263BlockStepErr      = -82,   /**< Step value is less than 8                                         */
    ipxStsH263MBStepErr         = -81,   /**< Step value is less than 16                                        */
    ipxStsH263FrameWidthErr     = -80,   /**< Frame width is less than 8                                        */
    ipxStsH263FrameHeightErr    = -79,   /**< Frame height is less than or equal to zero                        */
    ipxStsH263ExpandPelsErr     = -78,   /**< Expand pixels number is less than 8                               */
    ipxStsH263PlaneStepErr      = -77,   /**< Step value is less than the plane width                           */
    ipxStsH263QuantErr          = -76,   /**< Quantizer value is less than or equal to zero, or greater than 31 */
    ipxStsH263MVCodeErr         = -75,   /**< Illegal Huffman code during MV stream processing                  */
    ipxStsH263VLCCodeErr        = -74,   /**< Illegal Huffman code during VLC stream processing                 */
    ipxStsH263DCCodeErr         = -73,   /**< Illegal code during DC stream processing                          */
    ipxStsH263ZigzagLenErr      = -72,   /**< Zigzag compact length is more than 64                             */
    ipxStsFBankFreqErr          = -71,   /**< Incorrect value of the filter bank frequency parameter */
    ipxStsFBankFlagErr          = -70,   /**< Incorrect value of the filter bank parameter           */
    ipxStsFBankErr              = -69,   /**< Filter bank is not correctly initialized              */
    ipxStsNegOccErr             = -67,   /**< Negative occupation count                      */
    ipxStsCdbkFlagErr           = -66,   /**< Incorrect value of the codebook flag parameter */
    ipxStsSVDCnvgErr            = -65,   /**< No convergence of SVD algorithm               */
    ipxStsJPEGHuffTableErr      = -64,   /**< JPEG Huffman table is destroyed        */
    ipxStsJPEGDCTRangeErr       = -63,   /**< JPEG DCT coefficient is out of the range */
    ipxStsJPEGOutOfBufErr       = -62,   /**< Attempt to access out of the buffer    */
    ipxStsDrawTextErr           = -61,   /**< System error in the draw text operation */
    ipxStsChannelOrderErr       = -60,   /**< Wrong order of the destination channels */
    ipxStsZeroMaskValuesErr     = -59,   /**< All values of the mask are zero */
    ipxStsQuadErr               = -58,   /**< The quadrangle is nonconvex or degenerates into triangle, line or point */
    ipxStsRectErr               = -57,   /**< Size of the rectangle region is less than or equal to 1 */
    ipxStsCoeffErr              = -56,   /**< Unallowable values of the transformation coefficients   */
    ipxStsNoiseValErr           = -55,   /**< Bad value of noise amplitude for dithering             */
    ipxStsDitherLevelsErr       = -54,   /**< Number of dithering levels is out of range             */
    ipxStsNumChannelsErr        = -53,   /**< Bad or unsupported number of channels                   */
    ipxStsCOIErr                = -52,   /**< COI is out of range */
    ipxStsDivisorErr            = -51,   /**< Divisor is equal to zero, function is aborted */
    ipxStsAlphaTypeErr          = -50,   /**< Illegal type of image compositing operation                           */
    ipxStsGammaRangeErr         = -49,   /**< Gamma range bounds is less than or equal to zero                      */
    ipxStsGrayCoefSumErr        = -48,   /**< Sum of the conversion coefficients must be less than or equal to 1    */
    ipxStsChannelErr            = -47,   /**< Illegal channel number                                                */
    ipxStsToneMagnErr           = -46,   /**< Tone magnitude is less than or equal to zero                          */
    ipxStsToneFreqErr           = -45,   /**< Tone frequency is negative, or greater than or equal to 0.5           */
    ipxStsTonePhaseErr          = -44,   /**< Tone phase is negative, or greater than or equal to 2*PI              */
    ipxStsTrnglMagnErr          = -43,   /**< Triangle magnitude is less than or equal to zero                      */
    ipxStsTrnglFreqErr          = -42,   /**< Triangle frequency is negative, or greater than or equal to 0.5       */
    ipxStsTrnglPhaseErr         = -41,   /**< Triangle phase is negative, or greater than or equal to 2*PI          */
    ipxStsTrnglAsymErr          = -40,   /**< Triangle asymmetry is less than -PI, or greater than or equal to PI   */
    ipxStsHugeWinErr            = -39,   /**< Kaiser window is too huge                                             */
    ipxStsJaehneErr             = -38,   /**< Magnitude value is negative                                           */
    ipxStsStrideErr             = -37,   /**< Stride value is less than the row length */
    ipxStsEpsValErr             = -36,   /**< Negative epsilon value error             */
    ipxStsWtOffsetErr           = -35,   /**< Invalid offset value of wavelet filter                                       */
    ipxStsAnchorErr             = -34,   /**< Anchor point is outside the mask                                             */
    ipxStsMaskSizeErr           = -33,   /**< Invalid mask size                                                           */
    ipxStsShiftErr              = -32,   /**< Shift value is less than zero                                                */
    ipxStsSampleFactorErr       = -31,   /**< Sampling factor is less than or equal to zero                                */
    ipxStsSamplePhaseErr        = -30,   /**< Phase value is out of range: 0 <= phase < factor                             */
    ipxStsFIRMRFactorErr        = -29,   /**< MR FIR sampling factor is less than or equal to zero                         */
    ipxStsFIRMRPhaseErr         = -28,   /**< MR FIR sampling phase is negative, or greater than or equal to the sampling factor */
    ipxStsRelFreqErr            = -27,   /**< Relative frequency value is out of range                                     */
    ipxStsFIRLenErr             = -26,   /**< Length of a FIR filter is less than or equal to zero                         */
    ipxStsIIROrderErr           = -25,   /**< Order of an IIR filter is not valid */
    ipxStsDlyLineIndexErr       = -24,   /**< Invalid value of the delay line sample index */
    ipxStsResizeFactorErr       = -23,   /**< Resize factor(s) is less than or equal to zero */
    ipxStsInterpolationErr      = -22,   /**< Invalid interpolation mode */
    ipxStsMirrorFlipErr         = -21,   /**< Invalid flip mode                                         */
    ipxStsMoment00ZeroErr       = -20,   /**< Moment value M(0,0) is too small to continue calculations */
    ipxStsThreshNegLevelErr     = -19,   /**< Negative value of the level in the threshold operation    */
    ipxStsThresholdErr          = -18,   /**< Invalid threshold bounds */
    ipxStsContextMatchErr       = -17,   /**< Context parameter doesn't match the operation */
    ipxStsFftFlagErr            = -16,   /**< Invalid value of the FFT flag parameter */
    ipxStsFftOrderErr           = -15,   /**< Invalid value of the FFT order parameter */
    ipxStsStepErr               = -14,   /**< Step value is not valid */
    ipxStsScaleRangeErr         = -13,   /**< Scale bounds are out of the range */
    ipxStsDataTypeErr           = -12,   /**< Bad or unsupported data type */
    ipxStsOutOfRangeErr         = -11,   /**< Argument is out of range or point is outside the image */
    ipxStsDivByZeroErr          = -10,   /**< An attempt to divide by zero */
    ipxStsMemAllocErr           = -9,    /**< Not enough memory allocated for the operation */
    ipxStsNullPtrErr            = -8,    /**< Null pointer error */
    ipxStsRangeErr              = -7,    /**< Bad values of bounds: the lower bound is greater than the upper bound */
    ipxStsSizeErr               = -6,    /**< Wrong value of data size */
    ipxStsBadArgErr             = -5,    /**< Function arg/param is bad */
    ipxStsNoMemErr              = -4,    /**< Not enough memory for the operation */
    ipxStsSAReservedErr3        = -3,    /**< Unknown/unspecified error, -3 */
    ipxStsErr                   = -2,    /**< Unknown/unspecified error, -2 */
    ipxStsSAReservedErr1        = -1,    /**< Unknown/unspecified error, -1 */

     /// Success: 
    ipxStsNoErr                 =   0,   /**< No error, it's OK */

     /// Warnings: 
    ipxStsNoOperation       =   1,       /**< No operation has been executed */
    ipxStsMisalignedBuf     =   2,       /**< Misaligned pointer in operation in which it must be aligned */
    ipxStsSqrtNegArg        =   3,       /**< Negative value(s) of the argument in the function Sqrt */
    ipxStsInvZero           =   4,       /**< INF result. Zero value was met by InvThresh with zero level */
    ipxStsEvenMedianMaskSize=   5,       /**< Even size of the Median Filter mask was replaced by the odd one */
    ipxStsDivByZero         =   6,       /**< Zero value(s) of the divisor in the function Div */
    ipxStsLnZeroArg         =   7,       /**< Zero value(s) of the argument in the function Ln     */
    ipxStsLnNegArg          =   8,       /**< Negative value(s) of the argument in the function Ln */
    ipxStsNanArg            =   9,       /**< Not a Number argument value warning                  */
    ipxStsJPEGMarker        =   10,      /**< JPEG marker was met in the bitstream                 */
    ipxStsResFloor          =   11,      /**< All result values are floored                        */
    ipxStsOverflow          =   12,      /**< Overflow occurred in the operation                   */
    ipxStsLSFLow            =   13,      /**< Quantized LP syntethis filter stability check is applied at the low boundary of [0,pi] */
    ipxStsLSFHigh           =   14,      /**< Quantized LP syntethis filter stability check is applied at the high boundary of [0,pi] */
    ipxStsLSFLowAndHigh     =   15,      /**< Quantized LP syntethis filter stability check is applied at both boundaries of [0,pi] */
    ipxStsZeroOcc           =   16,      /**< Zero occupation count */
    ipxStsUnderflow         =   17,      /**< Underflow occurred in the operation */
    ipxStsSingularity       =   18,      /**< Singularity occurred in the operation                                       */
    ipxStsDomain            =   19,      /**< Argument is out of the function domain                                      */
    ipxStsNonIntelCpu       =   20,      /**< The target cpu is not Genuine Intel                                         */
    ipxStsCpuMismatch       =   21,      /**< The library for given cpu cannot be set                                     */
    ipxStsNoIppFunctionFound =  22,      /**< Application does not contain IPP functions calls                            */
    ipxStsDllNotFoundBestUsed = 23,      /**< The newest version of IPP dll was not found by dispatcher                   */
    ipxStsNoOperationInDll  =   24,      /**< The function does nothing in the dynamic version of the library             */
    ipxStsInsufficientEntropy=  25,      /**< Insufficient entropy in the random seed and stimulus bit string caused the prime/key generation to fail */
    ipxStsOvermuchStrings   =   26,      /**< Number of destination strings is more than expected                         */
    ipxStsOverlongString    =   27,      /**< Length of one of the destination strings is more than expected              */
    ipxStsAffineQuadChanged =   28,      /**< 4th vertex of destination quad is not equal to customer's one               */
    ipxStsWrongIntersectROI =   29,      /**< Wrong ROI that has no intersection with the source or destination ROI. No operation */
    ipxStsWrongIntersectQuad =  30,      /**< Wrong quadrangle that has no intersection with the source or destination ROI. No operation */
    ipxStsSmallerCodebook   =   31,      /**< Size of created codebook is less than cdbkSize argument */
    ipxStsSrcSizeLessExpected = 32,      /**< DC: The size of source buffer is less than expected one */
    ipxStsDstSizeLessExpected = 33,      /**< DC: The size of destination buffer is less than expected one */
    ipxStsStreamEnd           = 34,      /**< DC: The end of stream processed */
    ipxStsDoubleSize        =   35,      /**< Sizes of image are not multiples of 2 */
    ipxStsNotSupportedCpu   =   36,      /**< The cpu is not supported */
    ipxStsUnknownCacheSize  =   37,      /**< The cpu is supported, but the size of the cache is unknown */
    ipxStsSymKernelExpected =   38,      /**< The Kernel is not symmetric*/
    ipxStsEvenMedianWeight  =   39,      /**< Even weight of the Weighted Median Filter was replaced by the odd one */
    ipxStsWrongIntersectVOI =   40,      /**< VOI has no intersection with the source or destination volume. No operation                             */
    ipxStsI18nMsgCatalogInvalid=41,      /**< Message Catalog is invalid, English message returned                                                    */
    ipxStsI18nGetMessageFail  = 42,      /**< Failed to fetch a localized message, English message returned. For extended information use errno in Linux and GetLastError in Windows* */
    ipxStsWaterfall           = 43,      /**< Cannot load required library, waterfall is used */
    ipxStsPrevLibraryUsed     = 44       /**< Cannot load required library, previous dynamic library is used */
} IpxStatus;

///@}

/////////////////////////////////////////////////////////
/// \defgroup ipxdemosaicing_struct Demosaicing Types and Macros
/// The following Types and Macros are used in Demosaicing:
///@{
/////////////////////////////////////////////////////////

/// Success return code
#define ipxStsOk ipxStsNoErr

/// 8 bits per pixel (8bpp) image data
typedef unsigned char   Ipx8u;

/// 16 bits per pixel (16bpp) image data
typedef unsigned short  Ipx16u;

///@}

/////////////////////////////////////////////////////////
/// \defgroup ipxts_struct Demosaicing Structures
/// The following structures are used in Demosaicing:
///@{
/////////////////////////////////////////////////////////

/// The IpxRect structure defines a rectangle by the coordinates of its upper-left corner and width, height.
typedef struct _IpxRect
{
    int x; /**< Specifies the x-coordinate of the upper-left corner of the rectangle. */ 
    int y; /**< Specifies the y-coordinate of the upper-left corner of the rectangle. */ 
    int width; /**< Specifies the width of the rectangle. */ 
    int height; /**< Specifies the height of the rectangle. */ 
} IpxRect;

/// The IpxSize structure specifies the width and height of a rectangle.
typedef struct _IpxSize 
{
    int width;/**< Specifies the width of the rectangle. */ 
    int height;/**< Specifies the height of the rectangle. */ 
} IpxSize;

///@}

#endif // _IPXDEMOSAICINGDEF_H_
