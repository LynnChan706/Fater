// *****************************************************************************
//
//     Copyright (c) 2014, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#include "IGVTrueSenseUISampleDlg.h"

#include "pvmessagebox.h"
#include "displaythread.h"
#include <string>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QDockWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QtGui/QShortcut>
#include <QtCore/QThread>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include <PvRegisterInterfaceWnd.h>
#include <PvDeviceFinderWnd.h>
#include <PvGenStateStack.h>
#include <PvStreamInfo.h>
#include <PvSystem.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>


#ifdef WIN32
#include <shlobj.h>
#include <io.h>
#define PtAccess _access
#else
#define PtAccess access
#include <pwd.h>
#include <sys/stat.h>
#endif //WIN32


#include <sys/time.h>

#define TAG_COMMUNICATIONBROWSER ( "communicationbrowser" )
#define TAG_DEVICEBROWSER ( "devicebrowser" )
#define TAG_STREAMBROWSER ( "streambrowser" )
#define TAG_ACTIVESOURCE ( "activesource" )r
#define TAG_DISPLAYOPTIONS ( "displayoptions" )



// Simple event taking two integer parameters like messages on Win32
class QWindowsEvent : public QEvent
{
public:

	QWindowsEvent( QEvent::Type aEventID, uint64_t aWParam = 0, int64_t aLParam = 0 )
		: QEvent( aEventID )
		, mWParam( aWParam )
		, mLParam( aLParam )
	{
	}

	uint64_t GetWParam() const { return mWParam; }
	int64_t GetLParam() const { return mLParam; }

private:

    uint64_t mWParam;

	int64_t mLParam;
};


///
/// \brief Constructor
///

IGVTrueSenseUISampleDlg::IGVTrueSenseUISampleDlg( QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
    , mDeviceConnected( false )
    , mDisplayThread( NULL )
  	, mDisplay( NULL )
  	, mTrueSenseConv(NULL)
    , mNeedInit(true)
    , mDeviceWnd(NULL)
    , mCommunicationWnd(NULL)
    , mStreamParametersWnd(NULL)
    , mAcquiringImages(false)
{

    mStream = new PvStreamGEV;
    mPipeline = new PvPipeline( mStream );
    mPipeline->RegisterEventSink( this );
    mDisplay = new PvDisplayWnd;
    mTrueSenseConv = new TrueSenseConverter;
    mDeviceWnd = new PvGenBrowserWnd();
    mCommunicationWnd = new PvGenBrowserWnd();
    mStreamParametersWnd = new PvGenBrowserWnd();
    mDisplayThread = new DisplayThread(mDisplay, mTrueSenseConv);
    CreateLayout();
}

///

/// \brief Destructor
///


IGVTrueSenseUISampleDlg::~IGVTrueSenseUISampleDlg()
{
    Disconnect();

    if ( mDeviceWnd != NULL )
    {
        delete mDeviceWnd;
        mDeviceWnd = NULL;
    }

    if ( mCommunicationWnd != NULL )
    {
        delete mCommunicationWnd;
        mCommunicationWnd = NULL;
    }

    if ( mStreamParametersWnd != NULL )
    {
        delete mStreamParametersWnd;
        mStreamParametersWnd = NULL;
    }

    if ( mDisplayThread != NULL )
    {
        delete mDisplayThread;
    	mDisplayThread = NULL;
    }
    if ( mTrueSenseConv != NULL )
    {
        delete mTrueSenseConv;
    	mTrueSenseConv = NULL;
    }
    if ( mPipeline != NULL )
    {
        assert( !mPipeline->IsStarted() );

        mPipeline->UnregisterEventSink( this );

        delete mPipeline;
        mPipeline = NULL;
    }

    if ( mStream != NULL )
    {
        assert( !mStream->IsOpen() );

        delete mStream;
        mStream = NULL;
    }

    mDevice.UnregisterEventSink(this);
}



///
/// \brief Create dialog layout
///

void IGVTrueSenseUISampleDlg::CreateLayout()
{
    QGroupBox *lConnectionBox = CreateConnectGroup();
    CreateGain();
    QGroupBox *lAcqBox = CreateAcquisition();
    QGroupBox *lParametersBox = CreateParameters();
    QGroupBox *lDisplayBox = CreateDisplay();

    QVBoxLayout *lLayoutLeft = new QVBoxLayout();
    lLayoutLeft->addWidget( lConnectionBox, Qt::AlignTop );
    lLayoutLeft->addStretch();
    lLayoutLeft->addWidget( m_GainBox, Qt::AlignTop);
    lLayoutLeft->addStretch();
    lLayoutLeft->addWidget( lAcqBox, Qt::AlignTop );
    lLayoutLeft->addStretch();    
    lLayoutLeft->addWidget( lParametersBox, Qt::AlignTop );

    QHBoxLayout *lMainLayout = new QHBoxLayout;
    lMainLayout->addLayout( lLayoutLeft, Qt::AlignLeft );
    lMainLayout->addWidget( lDisplayBox );

    QFrame *lMainBox = new QFrame;
    lMainBox->setLayout( lMainLayout );

    setCentralWidget( lMainBox );
    setWindowTitle( tr( "IGVTrueSenseUISampleDlg" ) );

    EnableInterface();
	
}

///
/// \brief Create connect group box
///

QGroupBox *IGVTrueSenseUISampleDlg::CreateConnectGroup()
{
    mConnectButton = new QPushButton( tr( "Select/&Connect" ) );
    mConnectButton->setMinimumHeight( 27 );
    QObject::connect( mConnectButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedConnectButton() ) );

    mDisconnectButton = new QPushButton( tr( "&Disconnect" ) );
    mDisconnectButton->setMinimumHeight( 27 );
    QObject::connect( mDisconnectButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedDisconnectButton() ) );

    QHBoxLayout *lButtonsLayout = new QHBoxLayout;
    lButtonsLayout->addWidget( mConnectButton );
    lButtonsLayout->addWidget( mDisconnectButton );
    
    QLabel *lIpAddressLabel = new QLabel( tr( "IP Address" ) );
    mIPEdit = new QLineEdit;
    mIPEdit->setReadOnly( true );
    mIPEdit->setEnabled( false );

    QLabel *lMacAddressLabel = new QLabel( tr( "MAC Address" ) );
    mMACEdit = new QLineEdit;
    mMACEdit->setReadOnly( true );
    mMACEdit->setEnabled( false );

    QLabel *lManufacturerLabel = new QLabel( tr( "Manufacturer" ) );
    mManufacturerEdit = new QLineEdit;
    mManufacturerEdit->setReadOnly( true );
    mManufacturerEdit->setEnabled( false );

    QLabel *lModelLabel = new QLabel( tr( "Model" ) );
    mModelEdit = new QLineEdit;
    mModelEdit->setReadOnly( true );
    mModelEdit->setEnabled( false );

    QLabel *lNameLabel = new QLabel( tr( "Name" ) );
    mNameEdit = new QLineEdit;
    mNameEdit->setReadOnly( true );
    mNameEdit->setEnabled( false );

    QGridLayout *lGridLayout = new QGridLayout;
    lGridLayout->addWidget( lIpAddressLabel, 0, 0 );
    lGridLayout->addWidget( mIPEdit, 0, 1, 1, 2 );
    lGridLayout->addWidget( lMacAddressLabel, 1, 0 );
    lGridLayout->addWidget( mMACEdit, 1, 1, 1, 2 );
    lGridLayout->addWidget( lManufacturerLabel, 2, 0 );
    lGridLayout->addWidget( mManufacturerEdit, 2, 1, 1, 2 );
    lGridLayout->addWidget( lModelLabel, 3, 0 );
    lGridLayout->addWidget( mModelEdit, 3, 1, 1, 2 );
    lGridLayout->addWidget( lNameLabel, 4, 0 );
    lGridLayout->addWidget( mNameEdit, 4, 1, 1, 2 );

    QVBoxLayout *lBoxLayout = new QVBoxLayout;
    lBoxLayout->addLayout( lButtonsLayout );
    lBoxLayout->addLayout( lGridLayout );
    lBoxLayout->addStretch();

    QGroupBox *lConnectionBox = new QGroupBox( tr( "Connection" ) );
    lConnectionBox->setLayout( lBoxLayout );
    lConnectionBox->setMinimumWidth( 300);
    lConnectionBox->setMaximumWidth( 340 );
    lConnectionBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lConnectionBox;
}


///
/// \brief Create acquisition group box
///

QGroupBox *IGVTrueSenseUISampleDlg::CreateAcquisition()
{

    QLabel *lModeLabel = new QLabel( tr( "Mode" ) );
    mModeCombo = new QComboBox;
    mModeCombo->setEnabled( false );

    QObject::connect( mModeCombo, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeMode( int ) ) );

    QGridLayout *lAcqGridLayout = new QGridLayout;
    lAcqGridLayout->addWidget( lModeLabel, 0, 0 );
    lAcqGridLayout->addWidget( mModeCombo, 0, 1, 1, 4 );

    ///
    mPlayButton = new QPushButton( tr( "Play" ) );
    mPlayButton->setMinimumHeight( 45 );
    mPlayButton->setEnabled( false );

    QPixmap lPlayPixmap(":IGVTrueSenseUISample/res/play.bmp");
    lPlayPixmap.setMask(lPlayPixmap.createMaskFromColor(QColor(0xFF, 0xFF, 0xFF)));
    mPlayButton->setIcon(QIcon(lPlayPixmap));

    mStopButton = new QPushButton( tr( "Stop" ) );
    mStopButton->setMinimumHeight( 45 );
    mStopButton->setEnabled( false );

    QPixmap lStopPixmap(":IGVTrueSenseUISample/res/stop.bmp");
    lStopPixmap.setMask(lStopPixmap.createMaskFromColor(QColor(0xFF, 0xFF, 0xFF)));
    mStopButton->setIcon(QIcon(lStopPixmap));

    QObject::connect( mPlayButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedStart() ) );
    QObject::connect( mStopButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedStop() ) );

    QHBoxLayout *lButtonsLayout = new QHBoxLayout;
    lButtonsLayout->addWidget( mPlayButton );
    lButtonsLayout->addWidget( mStopButton );

    QVBoxLayout *lAcqLayout = new QVBoxLayout;
    lAcqLayout->addLayout( lAcqGridLayout );
    lAcqLayout->addLayout( lButtonsLayout );
    lAcqLayout->addStretch();

    QGroupBox *lAcqBox = new QGroupBox( tr( "Acquisition Control" ) );
    lAcqBox->setLayout( lAcqLayout );
    lAcqBox->setMinimumWidth( 300 );
    lAcqBox->setMaximumWidth( 340 );
    lAcqBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lAcqBox;
}


///
/// \brief Create Gain group box
///
void IGVTrueSenseUISampleDlg::CreateGain()
{
    QLabel *lDemosaicingLabel = new QLabel( tr( "Demosaicing Method:" ) );

    m_cbMethod = new QComboBox;
    m_cbMethod->setEnabled( false );
	m_cbMethod->addItem(tr("TSASimpleF"));
	m_cbMethod->addItem(tr("TSASimpleS"));
	m_cbMethod->addItem(tr("TSABayerLike"));
	m_cbMethod->addItem(tr("TSAMedium"));
	m_cbMethod->addItem(tr("TSAQuality"));

    QObject::connect( m_cbMethod, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeCbMethod( int ) ) );

    mIDC_TS_PAT_11 = new QRadioButton(tr("P"));
    mIDC_TS_PAT_11->setChecked(true);
	mIDC_TS_PAT_12 = new QRadioButton(tr("B"));
	mIDC_TS_PAT_13 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_14 = new QRadioButton(tr("G"));
	mIDC_TS_PAT_21 = new QRadioButton(tr("B"));
	mIDC_TS_PAT_22 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_23 = new QRadioButton(tr("G"));
	mIDC_TS_PAT_24 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_31 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_32 = new QRadioButton(tr("G"));
	mIDC_TS_PAT_33 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_34 = new QRadioButton(tr("R"));
	mIDC_TS_PAT_41 = new QRadioButton(tr("G"));
	mIDC_TS_PAT_42 = new QRadioButton(tr("P"));
	mIDC_TS_PAT_43 = new QRadioButton(tr("R"));
	mIDC_TS_PAT_44 = new QRadioButton(tr("G"));
 
    connect(mIDC_TS_PAT_11, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_12, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_13, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_14, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
    connect(mIDC_TS_PAT_21, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_22, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_23, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_24, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
    connect(mIDC_TS_PAT_31, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_32, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_33, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_34, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
    connect(mIDC_TS_PAT_41, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_42, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_43, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));
	connect(mIDC_TS_PAT_44, SIGNAL(clicked()), this, SLOT(OnBnClickedTsPattern()));


    QGridLayout *lGainGridLayout = new QGridLayout;
    lGainGridLayout->addWidget(lDemosaicingLabel, 0, 0);
    lGainGridLayout->addWidget( m_cbMethod,  0, 1, 1, 3, Qt::AlignRight);
   
    lGainGridLayout->addWidget(mIDC_TS_PAT_11, 1, 1);
    lGainGridLayout->addWidget(mIDC_TS_PAT_12, 1, 2);
    lGainGridLayout->addWidget(mIDC_TS_PAT_13, 1, 3);
    lGainGridLayout->addWidget(mIDC_TS_PAT_14, 1, 4);
    lGainGridLayout->addWidget(mIDC_TS_PAT_21, 2, 1);
    lGainGridLayout->addWidget(mIDC_TS_PAT_22, 2, 2);
    lGainGridLayout->addWidget(mIDC_TS_PAT_23, 2, 3);
    lGainGridLayout->addWidget(mIDC_TS_PAT_24, 2, 4);
    lGainGridLayout->addWidget(mIDC_TS_PAT_31, 3, 1);
    lGainGridLayout->addWidget(mIDC_TS_PAT_32, 3, 2);
    lGainGridLayout->addWidget(mIDC_TS_PAT_33, 3, 3);
    lGainGridLayout->addWidget(mIDC_TS_PAT_34, 3, 4);
    lGainGridLayout->addWidget(mIDC_TS_PAT_41, 4, 1);
    lGainGridLayout->addWidget(mIDC_TS_PAT_42, 4, 2);
    lGainGridLayout->addWidget(mIDC_TS_PAT_43, 4, 3);
    lGainGridLayout->addWidget(mIDC_TS_PAT_44, 4, 4);
   
    QVBoxLayout *lGainLayout = new QVBoxLayout;
    lGainLayout->addLayout( lGainGridLayout );
    lGainLayout->addStretch();

    m_GainBox = new QGroupBox( tr( "True Sense" ) );

	m_GainBox->setCheckable(true);
    m_GainBox->setChecked(true);
    
    m_GainBox->setLayout( lGainLayout );
    m_GainBox->setMinimumWidth( 300 );
    m_GainBox->setMaximumWidth( 340 );
    m_GainBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
 
	QObject::connect( m_GainBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedTrueSense() ) );

}


///
///
/// \brief Create parameters group box
///

QGroupBox *IGVTrueSenseUISampleDlg::CreateParameters()
{
    mCommunicationButton = new QPushButton( tr( "Communication control" ) );
    mCommunicationButton->setMinimumHeight( 27 );

    mDeviceButton = new QPushButton( tr( "Device control" ) );
    mDeviceButton->setMinimumHeight( 27 );

    mStreamButton = new QPushButton( tr( "Image stream control" ) );
    mStreamButton->setMinimumHeight( 27 );

    QObject::connect( mCommunicationButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedLinkButton() ) );
    QObject::connect( mDeviceButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedDeviceButton() ) );
    QObject::connect( mStreamButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedStreamparamsButton() ) );

    QVBoxLayout *layoutParameters = new QVBoxLayout;
    layoutParameters->addWidget( mCommunicationButton );
    layoutParameters->addWidget( mDeviceButton );
    layoutParameters->addWidget( mStreamButton );
    layoutParameters->addStretch();

    QGroupBox *parametersBox = new QGroupBox( tr( "Parameters and Control" ) );
    parametersBox->setLayout( layoutParameters );
    parametersBox->setMinimumWidth( 300 );
    parametersBox->setMaximumWidth( 340 );

    return parametersBox;
}

///
/// \brief Menu action handler
///

void IGVTrueSenseUISampleDlg::OnFileExitAction()
{
    Disconnect();
    close();
}
void IGVTrueSenseUISampleDlg::OnBnClickedTsPattern()
{
    uint32_t value;
    uint32_t patternStart;
    
    value = 		mIDC_TS_PAT_11->isChecked() |
					mIDC_TS_PAT_12->isChecked() << 1 |
					mIDC_TS_PAT_13->isChecked() << 2 |
					mIDC_TS_PAT_14->isChecked() << 3 |
					mIDC_TS_PAT_21->isChecked() << 4 |
					mIDC_TS_PAT_22->isChecked() << 5 |
					mIDC_TS_PAT_23->isChecked() << 6 |
					mIDC_TS_PAT_24->isChecked() << 7 |
					mIDC_TS_PAT_31->isChecked() << 8 |
					mIDC_TS_PAT_32->isChecked() << 9 |
					mIDC_TS_PAT_33->isChecked() << 10 |
					mIDC_TS_PAT_34->isChecked() << 11 |
					mIDC_TS_PAT_41->isChecked() << 12 |
					mIDC_TS_PAT_42->isChecked() << 13 |
					mIDC_TS_PAT_43->isChecked() << 14 |
					mIDC_TS_PAT_44->isChecked() << 15 ;
					
	switch ( value)
    {

     case 0x1:
        patternStart = 0;
        break;
     case 0x2:
		patternStart = 1;
		break;
     case 0x4:
        patternStart = 2;
        break;
     case 0x8:
		patternStart = 3;
		break;
     case 0x10:
        patternStart = 4;
        break;
     case 0x20:
		patternStart = 5;
		break;
     case 0x40:
        patternStart = 6;
        break;
     case 0x80:
		patternStart = 7;
		break;			
	 case 0x100:
        patternStart = 8;
        break;
     case 0x200:
		patternStart = 9;
		break;
     case 0x400:
        patternStart = 10;
        break;
     case 0x800:
		patternStart = 11;
		break;				
	 case 0x1000:
        patternStart = 12;
        break;
     case 0x2000:
		patternStart = 13;
		break;
     case 0x4000:
        patternStart = 14;
        break;
     case 0x8000:
		patternStart = 15;
		break;							
	
    }									
	mTrueSenseConv->SetPatternStart(patternStart);	

}
void IGVTrueSenseUISampleDlg::OnBnClickedTrueSense()
{
	int check = m_GainBox->isChecked();
	
	mTrueSenseConv->Enable(check==1);
	
}
///
///
/// \brief Create display
///


QGroupBox *IGVTrueSenseUISampleDlg::CreateDisplay()
{
    QVBoxLayout *displayLayout = new QVBoxLayout;
    displayLayout->addWidget( mDisplay->GetQWidget() );

    QGroupBox *lDisplayBox = new QGroupBox( tr( "Display" ) );
    lDisplayBox->setLayout( displayLayout );
    lDisplayBox->setMinimumWidth( 600 );

    return lDisplayBox;
}

///


void IGVTrueSenseUISampleDlg::CreateEnumComboBox(QComboBox* paramComboBox, char* paramName)
{
    paramComboBox->clear();
    PvGenEnum *lEnum = dynamic_cast<PvGenEnum*> (mDevice.GetParameters()->Get(paramName));
    
    if(lEnum == NULL)
      return;
    
    int64_t lEntriesCount = 0;
    lEnum->GetEntriesCount(lEntriesCount); //Failure here would be totally unexpected

    for(uint32_t i = 0; i < lEntriesCount; i++)
    {
        const PvGenEnumEntry *lEntry = NULL;
        lEnum->GetEntryByIndex(i, &lEntry); //Not expecting any failure on this one

        bool lAvailable = false;
        lEntry->IsAvailable(lAvailable);
        if(lAvailable)
        {
            ///
            PvString lEEName;
            lEntry->GetName(lEEName);
            int64_t lEEValue;
            lEntry->GetValue(lEEValue);

            paramComboBox->addItem(lEEName.GetAscii(),(qlonglong)lEEValue);
        }
    }

    //Set mode combo box to value currently used by the device
    int64_t lValue = 0;
    lEnum->GetValue(lValue);
    for(int i = 0; i < paramComboBox->count(); i++)
    {

        QVariant lData = paramComboBox->itemData( i);
        int64_t lComboValue = lData.value<int64_t>();
        
        if(lValue == lComboValue)
        {
            paramComboBox->setCurrentIndex(i);
            break;
        }

    }
    paramComboBox->setEnabled(true);

}

///
/// \brief Register interface menu handler
///

void IGVTrueSenseUISampleDlg::OnRegisterInterface()
{
    PvRegisterInterfaceWnd lInterface;
    lInterface.SetDevice( &mDevice );
    lInterface.ShowModal( this );
}


///
/// \brief Connect button click handler
///

void IGVTrueSenseUISampleDlg::OnBnClickedConnectButton()
{
    // create a device finder wnd and open the select device dialog
    PvDeviceFinderWnd lFinder;
    lFinder.SetTitle("GEV Device Selection");

    PvResult lResult = lFinder.ShowModal( this );


    if(lResult.GetCode() == PvResult::Code::ABORTED)
    {

        QMessageBox::critical(this, "", "Invalid selection. Please select a GEV Device connected through an eBUS network card configuration.\r\n" );
        return;
    }


    if ( ( !lResult.IsOK() ) || ( lFinder.GetSelected() == NULL ) )
    {
       return;
    }

    Connect( (PvDeviceInfoGEV*)lFinder.GetSelected());
}


///
/// \brief Disconnect button click handler
///

void IGVTrueSenseUISampleDlg::OnBnClickedDisconnectButton()
{
    Disconnect();
}


///
/// \brief Start button click handler
///

void IGVTrueSenseUISampleDlg::OnBnClickedStart()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    StartAcquisition();

    EnableInterface();
}


///
/// \brief Stop button click handler
///

void IGVTrueSenseUISampleDlg::OnBnClickedStop()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    mAcquiringImages = false;

    StopAcquisition();

    EnableInterface();

}


///
/// \brief Syncs the UI enabled state
///
void IGVTrueSenseUISampleDlg::EnableInterface()
{
    //this method can be called really early or late when the window is not created
    if (this->handle() == 0)
    {
        return;
    }
    mDeviceConnected = mDevice.IsConnected();

    //If not connected, disable the acquisition mode control. If enabled,
    //it will be managed automatically by events from the GenICam parameters

    mConnectButton->setEnabled( !mDeviceConnected && !mStream->IsOpen() );
    mDisconnectButton->setEnabled( mDeviceConnected || mStream->IsOpen() );

	EnableGenBrowsers( true );
	EnableControls( mDeviceConnected );
}


///
///
///

void IGVTrueSenseUISampleDlg::EnableControls( bool aEnabled )
{    
    mPlayButton->setEnabled( aEnabled );
    mModeCombo->setEnabled(aEnabled);
    mStopButton->setEnabled( aEnabled );

    //m_cbGainSel->setEnabled(aEnabled);
    //m_edGain->setEnabled(aEnabled);
    //m_cbOffsetSel->setEnabled(aEnabled);
    //m_edOffset->setEnabled(aEnabled);
    //m_cbPreAmpSel->setEnabled(aEnabled);
    //m_cbPreAmp->setEnabled(aEnabled);
    m_cbMethod->setEnabled(aEnabled);
    //m_bt_ed_gain_set->setEnabled(aEnabled);
    //m_bt_ed_offset_set->setEnabled(aEnabled);

}


///
/// \brief Enables the buttons used to open the browsers
///

void IGVTrueSenseUISampleDlg::EnableGenBrowsers( bool aEnabled )
{
    mDeviceButton->setEnabled( aEnabled && mDeviceConnected );
    mStreamButton->setEnabled( aEnabled && mStream->IsOpen() );
    //Special case, we want to allow setting communication parameters before we connect
    mCommunicationButton->setEnabled( true );
}


///
/// \brief Shows one of the GenICam browsers
///

void IGVTrueSenseUISampleDlg::ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aParams, const QString &aTitle )
{
    if ( aWnd->GetQWidget()->isVisible() )
    {
        // If already open, just toggle to closed...
        CloseGenWindow( aWnd );
        return;
    }

    // Create, assign parameters, set title and show modeless
    aWnd->SetTitle( aTitle.toAscii().data() );
    
    aWnd->ShowModeless(this);
    aWnd->SetGenParameterArray( aParams );
}

///
/// \brief Closes a GenICam tree browser.
///

void IGVTrueSenseUISampleDlg::CloseGenWindow( PvGenBrowserWnd *aWnd )
{
    if ( aWnd->GetQWidget()->isVisible() )
    {
        aWnd->Close();
    }
}


///
/// \brief Connects the device/stream
///


void IGVTrueSenseUISampleDlg::Connect( PvDeviceInfoGEV *aDI )
{
    assert( aDI != NULL );
    if ( aDI == NULL )
    {
        return;
    }

    // Just in case we came here still connected...
    Disconnect();

    // Device connection, packet size negociation and stream opening
    PvResult lResult = PvResult::Code::NOT_CONNECTED;

    // Connect device
    lResult = mDevice.Connect( aDI, PvAccessControl );
    if ( !lResult.IsOK() )
    {
        Disconnect();
        return;
    }

    // Perform automatic packet size negociation
    lResult = mDevice.NegotiatePacketSize( 0, 1440 );
    if ( !lResult.IsOK() )
    {
       usleep(2500 * 1000);
    }

    // Open stream - and retry if it fails
    for ( ;; )
    {
        // Open stream
        lResult = mStream->Open( aDI->GetIPAddress() );
        if ( lResult.IsOK() )
        {
            break;
        }

        usleep(1000*1000);// ::Sleep( 1000 );
    }

    // Now that the stream is opened, set the destination on the device
    mDevice.SetStreamDestination( mStream->GetLocalIPAddress(), mStream->GetLocalPort() );

    if ( !lResult.IsOK() )
    {
        Disconnect();

        return;
    }

    // Register to all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = mDevice.GetParameters();
    for ( uint32_t i = 0; i < lGenDevice->GetCount(); i++ )
    {
        lGenDevice->Get( i )->RegisterEventSink( this );
    }


    PvGenInteger *lIPAddressParam = dynamic_cast<PvGenInteger *>( lGenDevice->Get( "GevCurrentIPAddress" ) );
    PvGenInteger *lMACAddressParam = dynamic_cast<PvGenInteger *>( lGenDevice->Get( "GevMACAddress" ) );
    PvGenString *lManufacturer = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceVendorName" ) );
    PvGenString *lModelName = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceModelName" ) );
    PvGenString *lDeviceVersion = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceVersion" ) );
    PvGenString *lNameParam = dynamic_cast<PvGenString *>( lGenDevice->Get( "GevDeviceUserID " ) );

    // Verify all params - except name which is optional
    assert( lIPAddressParam != NULL );
    assert( lMACAddressParam != NULL );
    assert( lManufacturer != NULL );
    assert( lModelName != NULL );

    PvString lManufacturerStr, lModelNameStr, lDeviceVersionStr = "N/A";
    lManufacturer->GetValue( lManufacturerStr );
    lModelName->GetValue( lModelNameStr );

    if ( lDeviceVersion != NULL )
    {
        lDeviceVersion->GetValue( lDeviceVersionStr );
    }

    // IP
    QString lIPStr;
    int64_t lIPAddress = 0;
    lIPAddressParam->GetValue( lIPAddress );
    lIPStr.sprintf( "%i.%i.%i.%i", //lld.%lld.%lld.%lld",
        (int)(( lIPAddress & 0xFF000000 ) >> 24),
        (int)(( lIPAddress & 0x00FF0000 ) >> 16),
        (int)(( lIPAddress & 0x0000FF00 ) >> 8),
        (int)(( lIPAddress & 0x000000FF ) ));

    // MAC address
    QString lMACStr;
    int64_t lMACAddress;
    lMACAddressParam->GetValue( lMACAddress );
    lMACStr.sprintf( "%02X:%02X:%02X:%02X:%02X:%02X",
    (uint)(( lMACAddress & 0xFF00000000 ) >> 40),
    (uint)(( lMACAddress & 0xFF00000000 ) >> 32),
    (uint)(( lMACAddress & 0xFF000000 ) >> 24),
    (uint)(( lMACAddress & 0x00FF0000 ) >> 16),
    (uint)(( lMACAddress & 0x0000FF00 ) >> 8),
    (uint)(( lMACAddress & 0x000000FF ) ));

    // Device name (User ID)
    PvString lNameStr;

    lNameStr = "N/A";

    if ( lNameParam != NULL )
    {
        PvString lStr;
        lNameParam->GetValue( lStr );
        lNameStr = lStr.GetUnicode();
    }

    mManufacturerEdit->setText( lManufacturerStr.GetAscii());
    mModelEdit->setText( lModelNameStr.GetAscii());
    mIPEdit->setText( lIPStr.toAscii());
    mMACEdit->setText( lMACStr.toAscii());
    mNameEdit->setText( lNameStr.GetAscii()); 

    // Fill acquisition mode combo box
    CreateEnumComboBox(mModeCombo, (char*)"AcquisitionMode");

    mAcquiringImages = false;

    PvGenParameter *lModeParameter =  dynamic_cast<PvGenParameter *>(lGenDevice->Get( "AcquisitionMode"  ));
    PvGenParameter *lStartParameter = dynamic_cast<PvGenParameter *>(lGenDevice->Get( "AcquisitionStart"  ));
    PvGenParameter *lStopParameter =  dynamic_cast<PvGenParameter *>(lGenDevice->Get( "AcquisitionStop"  ));

    assert( lModeParameter != NULL );
    assert( lStartParameter != NULL );
    assert( lStopParameter != NULL );

    // Force an update on all the parameters we have shortcuts to
    OnParameterUpdate(lModeParameter);
    OnParameterUpdate(lStartParameter);
    OnParameterUpdate(lStopParameter);

    m_cbMethod->setCurrentIndex(0);

    // Ready image reception
    StartStreaming();

    // Sync up UI
    EnableInterface();
}
// =============================================================================
void IGVTrueSenseUISampleDlg::Disconnect()
{
    // Unregister all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = mDevice.GetParameters();
    for ( uint32_t i = 0; i < lGenDevice->GetCount(); i++ )
    {
        lGenDevice->Get( i )->UnregisterEventSink( this );
    }

    // Close all configuration child windows
    CloseGenWindow( mDeviceWnd );
    CloseGenWindow( mCommunicationWnd );
    CloseGenWindow( mStreamParametersWnd );

    // If streaming, stop streaming
    StopStreaming();

    // Reset device ID - can be called by the destructor when the window
    // no longer exists, be careful...
    if (this->handle() != 0 ) //!= NULL) // ! = 0)
    {

        // Reset device ID
        mManufacturerEdit->setText( "" );
        mModelEdit->setText( "" );
        mIPEdit->setText( "" );
        mMACEdit->setText( "" );
        mNameEdit->setText( "" );

    }

    mDevice.Disconnect();

    mStream->Close();

    EnableInterface();

}


///
/// \brief Start display thread, arm pipeline
///

void IGVTrueSenseUISampleDlg::StartStreaming()
{
    QMutexLocker lLock( &mStartStreamingMutex );

    if ( !mDisplayThread->IsRunning() )
    {
        PvGenParameterArray *lParameters = mDevice.IsConnected() ?
            mDevice.GetParameters() :
            NULL;

        // Start threads
        mDisplayThread->Start( mPipeline, lParameters );
        mDisplayThread->SetPriority( QThread::HighestPriority );

    }

    if ( !mPipeline->IsStarted() )
    {

        mPipeline->Start();
    }

}


///
/// \brief Stops display thread, pipeline
///

void IGVTrueSenseUISampleDlg::StopStreaming()
{
    QMutexLocker lLock( &mStartStreamingMutex );

    // Stop display thread
    if ( mDisplayThread->IsRunning() )
    {
        mDisplayThread->Stop( false );

        if ( mPipeline->IsStarted() )
        {
        	mPipeline->Stop();
        }

        mDisplayThread->WaitComplete();

    }

}

// =============================================================================
void IGVTrueSenseUISampleDlg::StartAcquisition()
{
    PvGenEnum *lMode = dynamic_cast<PvGenEnum *>( mDevice.GetParameters()->Get( "AcquisitionMode" ) );
    PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( mDevice.GetParameters()->Get( "AcquisitionStart" ) );
    PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetParameters()->Get( "TLParamsLocked" ) );
    PvGenInteger *lPayloadSize = dynamic_cast<PvGenInteger *>( mDevice.GetParameters()->Get( "PayloadSize" ) );
    PvGenCommand *lResetStats = dynamic_cast<PvGenCommand *>( mStream->GetParameters()->Get( "Reset" ) );

    // Try reading payload size from device
    int64_t lPayloadSizeValue = 0;
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );
    }

    // If payload size is valid, force buffers re-alloc - better than
    // adjusting as images are comming in
    if ( lPayloadSizeValue > 0 )
    {
        mPipeline->SetBufferSize( static_cast<uint32_t>( lPayloadSizeValue ) );
    }

    // Never hurts to start streaming on a fresh pipeline/stream...
    mPipeline->Reset();

    // Reset stream statistics
    lResetStats->Execute();

    PvResult lResult = PvResult::Code::NOT_INITIALIZED;

    PvString lStr;
    lMode->GetValue( lStr );

    if(strcmp(lStr.GetAscii(), "Continuous") >= 0)
    {
        // We are streaming, lock the TL parameters
        if ( lTLParamsLocked != NULL )
        {
            lResult = lTLParamsLocked->SetValue( 1 );
        }

        lStart->Execute();
        mAcquiringImages = true;
    }
    else if ( (strcmp(lStr.GetAscii(), "Multi") >= 0) || 
              (strcmp(lStr.GetAscii(), "Single") >= 0)) 
    {
        // We are streaming, lock the TL parameters
        if ( lTLParamsLocked != NULL )
        {
            lTLParamsLocked->SetValue( 1 );
        }

        lResult = lStart->Execute();

        // We are done streaming, unlock the TL parameters
        if ( lTLParamsLocked != NULL )
        {
            lTLParamsLocked->SetValue( 0 );
        }
    }
}


// =============================================================================
void IGVTrueSenseUISampleDlg::StopAcquisition()
{
    PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( mDevice.GetParameters()->Get( "AcquisitionStop" ) );
    assert( lStop != NULL ); // Mandatory parameter


    PvResult lResult = lStop->Execute();

    // TLParamsLocked - Optionnal
    PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetParameters()->Get( "TLParamsLocked" ) );
    if ( lTLParamsLocked != NULL )
    {
        lResult = lTLParamsLocked->SetValue( 0 );
    }

}


//
/// \brief Stops the acquisition
///
void IGVTrueSenseUISampleDlg::OnCbnSelchangeMode( int aIndex )
{
    if ( !mDeviceConnected )
    {
        return;
    }

    // No selection?
    if ( mModeCombo->currentIndex() < 0 )
    {
        return;
    }

    // Get acquisition mode parameter
    PvGenEnum *lMode = mDevice.GetParameters()->GetEnum( "AcquisitionMode" );
    if ( lMode == NULL )
    {
       return;
    }

    // Retrieve item data value from combo box item, assign it to acquisition mode parameter
    QVariant lData = mModeCombo->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
           int64_t lValue = lData.value<int64_t>();
           PvResult lResult = lMode->SetValue( lValue );
           if ( !lResult.IsOK() )
              QMessageBox::critical( this, "Error", "Unable to set AcquisitionMode value.\r\n" );
    }
}

void IGVTrueSenseUISampleDlg::OnCbnSelchangeCbMethod( int aIndex )
{
	int sel = m_cbMethod->currentIndex();
	TrueSenseAlgType algo = (TrueSenseAlgType) sel;
	mTrueSenseConv->SetAlgo(algo);
}

void IGVTrueSenseUISampleDlg::OnParameterUpdate( PvGenParameter *aParameter )
{

    if ( !aParameter )
    {
        assert( 0 ); // No such parameter
        return;
    }

    PvString lName;
    if ( !aParameter->GetName( lName ).IsOK() )
    {
        assert( 0 ); // Totally unexpected
        return;
    }

    // If the AcquisitionStop node is updated, make sure to reflect its new state on the UI
    if ( ( lName == "AcquisitionStop" ) && ( mStopButton->handle() != 0 ) )
    {
        bool lAvailable = false;
        if ( aParameter->IsAvailable( lAvailable ).IsOK() )
        {
            mStopButton->setEnabled( lAvailable );
        }
    }

    if ( ( lName == "AcquisitionMode" ) && ( mModeCombo->handle() != 0))
    {
        bool lAvailable = false, lWritable = false;
        aParameter->IsAvailable( lAvailable ).IsOK() ;
        if ( lAvailable )
        {
          aParameter->IsWritable( lWritable ).IsOK() ;
        }

        mModeCombo->setEnabled( lAvailable && lWritable );

        PvGenEnum *lEnum = dynamic_cast<PvGenEnum *>( aParameter );
        assert( lEnum != NULL );

        if ( lEnum != NULL )
        {
            int64_t lEEValue = 0;
            lEnum->GetValue( lEEValue );

            for ( int i = 0; i < mModeCombo->count(); i++ )
            {
                QVariant lData = mModeCombo->itemData(i);
                if ( lData.canConvert<int64_t>() == lEEValue )
                {
                    mModeCombo->setCurrentIndex(i);
                    break;
                }
            }
        }
        SendEventIfPossible(WM_UPDATEACQUISITIONMODE);
    }
}

///
/// \brief Generic event handler
///

bool IGVTrueSenseUISampleDlg::event( QEvent *aEvent )
{
    switch ( aEvent->type() )
    {

     case WM_UPDATEACQUISITIONMODE:
        OnUpdateAcquisitionModeEvent();
        break;
    }

    return QWidget::event( aEvent );
}

///
/// \brief Displays communication parameters
///

void IGVTrueSenseUISampleDlg::OnBnClickedLinkButton()
{
    ShowGenWindow(
        mCommunicationWnd,
        mDevice.GetCommunicationParameters(),
        "Communication Control" );
}


///
/// \brief Displays device parameters
///

void IGVTrueSenseUISampleDlg::OnBnClickedDeviceButton()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    ShowGenWindow(
        mDeviceWnd,
        mDevice.GetParameters(),
        "GEV Device Control" );
}


///
/// \brief Display stream parameters
///

void IGVTrueSenseUISampleDlg::OnBnClickedStreamparamsButton() //ShowStreamParameters()
{
    if ( !mStream->IsOpen() )
    {
        return;
    }

    ShowGenWindow( mStreamParametersWnd,
        mStream->GetParameters(),
        "Image Stream Control" );
}


void IGVTrueSenseUISampleDlg::OnUpdateAcquisitionModeEvent()
{
    PvGenEnum *lEnum = mDevice.GetParameters()->GetEnum( "AcquisitionMode" );
    if ( lEnum != NULL )
    {

        // Set mode combo box to value currently used by the device
        int64_t lEnumValue = 0;
        lEnum->GetValue( lEnumValue );
        for ( int i = 0; i < mModeCombo->count(); i++ )
        {
            QVariant lData = mModeCombo->itemData( i );
            int64_t lComboValue = lData.value<int64_t>();

            if ( lEnumValue == lComboValue )
            {
                    mModeCombo->setCurrentIndex( i );
                    break;
            }  void OnBnClickedSetInteger(int ctlID, char* param);

        }

        mModeCombo->setEnabled(lEnum->IsWritable());
    }

    return;
}

///
/// \brief If in application thread context, send event if not, post
///

void IGVTrueSenseUISampleDlg::SendEventIfPossible( int aMsg )
{
    if ( QCoreApplication::instance()->thread() == QThread::currentThread() )
    {
        // Same thread, use send (ends up being a direct call)
        QCoreApplication::sendEvent( this, new QEvent( static_cast<QEvent::Type>( aMsg ) ) );
    }
    else
    {
    	// Different thread, we want to use post to prevent deadlocks
        QCoreApplication::postEvent( this, new QEvent( static_cast<QEvent::Type>( aMsg ) ) );
    }
}
