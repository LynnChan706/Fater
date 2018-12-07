#include "TrueSenseConverter.h"

#include <PvPixelType.h>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>
TrueSenseConverter::TrueSenseConverter():
	m_patStart(0)
	, m_algo(TSASimpleF)
	, m_bEnabled(true)
{
};

TrueSenseConverter::TrueSenseConverter(int patternStart, TrueSenseAlgType algo):
	m_patStart(patternStart)
	, m_algo(algo)
	, m_bEnabled(true)

{
};

TrueSenseConverter::~TrueSenseConverter()//void)
{
};

PvBuffer& TrueSenseConverter::ConvertForDisplay(PvBuffer& inBuff)
{
	PvResult ret = 0;
	const PvImage *srcImg = inBuff.GetImage();
	PvPixelType srcType = srcImg->GetPixelType();

	// Update internal output buffer
	UpdateInternalBuffer(inBuff);

	// Convert to 8bpp if necessary
	if(!Is8bpp(srcType))
	{
		if(IsPackedBobcatPixType(srcType) )
		{
			Unpack(&inBuff, &m_tmpBuffMono8, true);
		}
		else 
		{
			TruncateTo8b(&inBuff, &m_tmpBuffMono8);
		}

		// set unpacked source buffer
		srcImg = m_tmpBuffMono8.GetImage();
	}
	
  
	// Output image bitdepth
	unsigned int dstBytesPerPix = (m_outBuffer.GetImage()->GetBitsPerPixel()+7)/8;
	// Calculate pattern grid start
	unsigned char grid = m_patStart; 
	// Source buffer
	const unsigned char* pSrc = (unsigned char*)srcImg->GetDataPointer();
	// Source ROI and size
	IpxRect srcRoi;
	IpxSize srcSize;
	srcRoi.x = srcImg->GetOffsetX();
	srcRoi.y = srcImg->GetOffsetY();
	srcRoi.width = srcSize.width = srcImg->GetWidth();
	srcRoi.height = srcSize.height = srcImg->GetHeight();
	// Source stride
	int srcStep = ((srcImg->GetWidth()*srcImg->GetBitsPerPixel()+7)/8) + srcImg->GetPaddingX();
	// Source bitDepth
	int bitDepth = srcImg->GetBitsPerPixel();
	// Destination buffer
	unsigned char* pDst = (unsigned char*)m_outBuffer.GetDataPointer();
	// Destination stride
	int dstStep = (srcImg->GetWidth()*dstBytesPerPix) ;
	
	TrueSenseParam param;
	TrueSense_DefaultValues(&param);

	IpxStatus convRes = TrueSenseToBGR_8u_C1C4R(pSrc, srcRoi, srcSize, srcStep, bitDepth, pDst, dstStep, grid, &param, m_algo, false);
  
	return m_outBuffer;
	

};

void TrueSenseConverter::UpdateInternalBuffer(PvBuffer& inBuff)
{
	// Get input image parameters
	int width = inBuff.GetImage()->GetWidth();
	int height = inBuff.GetImage()->GetHeight();
	PvImage *pImg= m_outBuffer.GetImage();
	const PvImage *srcImg = inBuff.GetImage();
	PvPixelType srcType = srcImg->GetPixelType();

	// Check if buffer size was changed from last call
	if(pImg->GetWidth() != width ||  pImg->GetHeight() != height)
	{
		// Rellocate output buffer
		pImg->Free();
		pImg->Alloc( width, height, PvPixelBGRa8);
	}

	// Check if buffer size was changed from last call for 10/12/16bpp source video
	if(!Is8bpp(srcType))
	{
		if(m_tmpBuffMono8.GetImage()->GetWidth() != width ||  m_tmpBuffMono8.GetImage()->GetHeight() != height)
		{
			// Rellocate temp Mono8 buffer
			m_tmpBuffMono8.GetImage()->Free();
			m_tmpBuffMono8.GetImage()->Alloc( width, height, PvPixelMono8 );
		}
	}

};

bool TrueSenseConverter::Unpack( const PvBuffer *aSource, PvBuffer *aDestination, bool truncate )
{
	// Get source and destination imnage data
	const PvImage *pSrcImg = aSource->GetImage();
	PvImage *pDstImg = aDestination->GetImage();

	unsigned char *pSrc = (unsigned char *)pSrcImg->GetDataPointer();
	int width = pSrcImg->GetWidth();
	int height = pSrcImg->GetHeight();
	int lineSize = (3*width+1)/2;

	int srcLineData = 0; 
	int dstLineData = 0;

	// Unpack the pixel data
	if(truncate && pDstImg->GetBitsPerPixel()==8)
	{
		// Unpack 10/12bit image to 8bits with truncation
		unsigned char *pDst = (unsigned char *)pDstImg->GetDataPointer();
		for( int y=0; y<height; y++)
		{
			for( int x=1; x< lineSize; x+=3)
			{
				pDst[dstLineData++] = pSrc[srcLineData+x-1];
				pDst[dstLineData++] = pSrc[srcLineData+x+1];
			}
			srcLineData += lineSize;
		}
	}
	else
	{
		// Unpack 10/12bit image to 16bits without truncation
		unsigned short *pDst = (unsigned short *)pDstImg->GetDataPointer();
		unsigned short shift1 = (pSrcImg->GetPixelType()==PvPixelMono10Packed)?2:4;
		for( int y=0; y<height; y++)
		{
			for( int x=1; x< lineSize; x+=3)
			{
				unsigned short v1  = pSrc[srcLineData+x-1];
				unsigned short v2  = pSrc[srcLineData+x+1];
				unsigned short v12 = pSrc[srcLineData+x];

				pDst[dstLineData++] = v12|(v1<<shift1);
				pDst[dstLineData++] = (v12>>4)|(v2<<shift1);
			}
			srcLineData += lineSize;
		}

	}
	return true;
};

bool TrueSenseConverter::TruncateTo8b( const PvBuffer *aSource, PvBuffer *aDestination )
{
	// Get input and output image parameters
	const PvImage *pSrcImg = aSource->GetImage();
	PvImage *pDstImg = aDestination->GetImage();
	int bpp = pSrcImg->GetBitsPerPixel();
	PvPixelType pixType = pSrcImg->GetPixelType();
	unsigned short *pSrc = (unsigned short *)pSrcImg->GetDataPointer();
	unsigned char *pDst = (unsigned char *)pDstImg->GetDataPointer();
	int width = pSrcImg->GetWidth();
	int height = pSrcImg->GetHeight();
	
	// Get shift value 
	int shift = (Is12bpp(pixType)) ? 4 : (Is16bpp(pixType)? 8 : 2);

	// Trancate pixel data
	for( int i=0; i<height*width; i++)
	{
		*pDst = (unsigned char)((*pSrc)>>shift);
		pSrc++;
		pDst++;
	}

	return true;
};
