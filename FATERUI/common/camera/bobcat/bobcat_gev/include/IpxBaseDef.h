// IpxBaseDef.h: Definition of basic types for the IIPL.
//
//////////////////////////////////////////////////////////////////////

#if !defined(IpxBaseDef_H__INCLUDED_)
#define IpxBaseDef_H__INCLUDED_

//////////////////////////////////////////////////////////////////////
//////////     Common macros and inline functions
//////////////////////////////////////////////////////////////////////

#ifndef II_INLINE
	#if defined __cplusplus
		#define II_INLINE inline
	#else
		#define II_INLINE static
	#endif
#endif /* II_INLINE */

#define IIPL_ORIGIN_TL 0
#define IIPL_ORIGIN_BL 1
#define	MAX_IMAGE_CHANNELS		4

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

// default image row align (in bytes)
#define  II_DEFAULT_IMAGE_ROW_ALIGN  4

typedef	unsigned long II_RESULT;

/////////////////////////////////////////////////////////////////////////////
/////      Core image functions                                        //////    
/////////////////////////////////////////////////////////////////////////////
/// \defgroup ref_data
/// \ingroup iil_ref
///@{

/// Base type of data for description images in IIL.
//================================================================================
/**
* \note
* IIData is used to unify interface of IIL.
* IIL recognize data type and data structure.
* \req
* \table2{Header, IIDef.h}
* \endtable
* \see IIDataList<BR/>IIMask<BR/>IIImage<BR/>IIMatrix<BR/>IIMatrixC
*/
typedef struct _IIData
{
	_IIData(){nSize = sizeof(_IIData);};
    unsigned long nSize;    /**<  Size of appropriate structure (IIImage, IIMatrix, etc.). */
}IIData;

//////////////////////////////////////////////////////////////////////
/////////////       IICOI and IIROI

typedef struct _IICOI
{
	_IICOI()	{indNumber = 0;};
    int		indexes[MAX_IMAGE_CHANNELS];	// indexes of color channels 
    int		indNumber;						// the number of indexes
}IICOI;

//==================================================================
II_INLINE  bool IIGetIndexesFromCOI(const IICOI* coi, int* indexes, int* size)
{
	int ii, i;
	if (!indexes || !size)
		return false;
	ii = coi->indNumber;
	if (*size < ii)
	{
		*size = ii;
		return false;
	}
	for (i=0; i<ii; i++)
	{
		indexes[i] = coi->indexes[i];
	}
	*size = ii;

	return true;
}
//==================================================================
II_INLINE  bool IICOIFrom(IICOI* coi, int* indexes, int size)
{
	int ii, i;
	if (!indexes || !size || !coi)
		return false;
	ii = MIN(size, MAX_IMAGE_CHANNELS);
	for (i=0; i<ii; i++)
	{
		if (indexes[i] >= MAX_IMAGE_CHANNELS)
		{
			coi->indNumber = 0;
			return false;
		}
		else
		{
			coi->indexes[i] = indexes[i];
		}
	}
	coi->indNumber = ii;
	
	return true;
}

typedef struct _IIROI
{
	_IIROI()	{xOffset = yOffset = width = height = 0;};
    int		xOffset;
    int		yOffset;
    int		width;
    int		height;
}IIROI;

  

#endif // !defined(IpxBaseDef_H__INCLUDED_)
