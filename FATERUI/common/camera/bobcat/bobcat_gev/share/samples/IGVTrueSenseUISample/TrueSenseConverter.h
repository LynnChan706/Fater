#pragma once
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QCheckBox>
#include <QtCore/QMutex>
#include <QtCore/QEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <PvGenBrowserWnd.h>
#include <PvDisplayWnd.h>

#include <PvDevice.h>
#include <PvStream.h>
#include <PvPixelType.h>
#include <PvBuffer.h>
#include <PvPipeline.h>
#include <IpxTrueSense.h>

class TrueSenseConverter
{
public:
	TrueSenseConverter();//void);
	TrueSenseConverter(int patternStart, TrueSenseAlgType algo);
	virtual ~TrueSenseConverter();//void);
	PvBuffer& ConvertForDisplay(PvBuffer& inBuff);
	void Enable(bool en){ m_bEnabled=en; };
	bool Enabled(){ return m_bEnabled; };
	void SetPatternStart(int st) { m_patStart=st; };
	void SetAlgo(TrueSenseAlgType alg) { m_algo=alg; };
protected:
	void UpdateInternalBuffer(PvBuffer& inBuff);
	bool Unpack( const PvBuffer *aSource, PvBuffer *aDestination, bool truncate );
	bool TruncateTo8b( const PvBuffer *aSource, PvBuffer *aDestination );
	
	PvBuffer m_outBuffer;
	PvBuffer m_tmpBuffMono8;
	int m_patStart;
	TrueSenseAlgType m_algo;
	bool m_bEnabled;


};

inline bool IsPackedBobcatPixType(PvPixelType in)
{
	return ( 
		PvPixelMono10Packed == in ||
		PvPixelMono12Packed == in ||
		PvPixelBayerGR10Packed == in ||
		PvPixelBayerRG10Packed == in ||
		PvPixelBayerGB10Packed == in ||
		PvPixelBayerBG10Packed == in ||
		PvPixelBayerGR12Packed == in ||
		PvPixelBayerRG12Packed == in ||
		PvPixelBayerGB12Packed == in ||
		PvPixelBayerBG12Packed == in );

}

inline bool IsMonoBobcatPixType(PvPixelType in)
{
	return ( 
		PvPixelMono8 == in ||
		PvPixelMono10 == in ||
		PvPixelMono12 == in ||
		PvPixelMono10Packed == in ||
		PvPixelMono12Packed == in ||
		PvPixelMono14 == in ||
		PvPixelMono16 == in );

}

inline bool Is10bpp(PvPixelType in)
{
	return ( 
		PvPixelMono10 == in ||
		PvPixelMono10Packed == in ||
		PvPixelBayerGR10 == in ||
		PvPixelBayerRG10 == in ||
		PvPixelBayerGB10 == in ||
		PvPixelBayerBG10 == in ||
		PvPixelBayerGR10Packed== in ||
		PvPixelBayerRG10Packed== in ||
		PvPixelBayerGB10Packed== in ||
		PvPixelBayerBG10Packed== in );
}

inline bool Is12bpp(PvPixelType in)
{
	return ( 
		PvPixelMono12 == in ||
		PvPixelMono12Packed == in ||
		PvPixelBayerGR12 == in ||
		PvPixelBayerRG12 == in ||
		PvPixelBayerGB12 == in ||
		PvPixelBayerBG12 == in ||
		PvPixelBayerGR12Packed== in ||
		PvPixelBayerRG12Packed== in ||
		PvPixelBayerGB12Packed== in ||
		PvPixelBayerBG12Packed== in );
}

inline bool Is8bpp(PvPixelType in)
{
	return ( 
		PvPixelMono8 == in ||
		PvPixelBayerGR8 == in ||
		PvPixelBayerRG8 == in ||
		PvPixelBayerGB8 == in ||
		PvPixelBayerBG8 == in );
}

inline bool Is16bpp(PvPixelType in)
{
	return ( PvPixelMono16 == in);
}



inline bool IsBayerBobcatPixType(PvPixelType in)
{
	return ( 
		PvPixelBayerGR8 == in ||
		PvPixelBayerRG8 == in ||
		PvPixelBayerGB8 == in ||
		PvPixelBayerBG8 == in ||
		PvPixelBayerGR10 == in ||
		PvPixelBayerRG10 == in ||
		PvPixelBayerGB10 == in ||
		PvPixelBayerBG10 == in ||
		PvPixelBayerGR12 == in ||
		PvPixelBayerRG12 == in ||
		PvPixelBayerGB12 == in ||
		PvPixelBayerBG12== in ||
		PvPixelBayerGR10Packed == in ||
		PvPixelBayerRG10Packed == in ||
		PvPixelBayerGB10Packed == in ||
		PvPixelBayerBG10Packed == in ||
		PvPixelBayerGR12Packed == in ||
		PvPixelBayerRG12Packed == in ||
		PvPixelBayerGB12Packed == in ||
		PvPixelBayerBG12Packed == in ||
		PvPixelBayerGR16 == in ||
		PvPixelBayerRG16 == in ||
		PvPixelBayerGB16 == in ||
		PvPixelBayerBG16 == in );
	
}
