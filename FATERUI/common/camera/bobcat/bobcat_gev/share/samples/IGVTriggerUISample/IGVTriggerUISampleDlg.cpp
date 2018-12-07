// *****************************************************************************
//
//     Copyright (c) 2013, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#include "IGVTriggerUISampleDlg.h"

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
#define TAG_ACTIVESOURCE ( "activesource" )
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

IGVTriggerUISampleDlg::IGVTriggerUISampleDlg( QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
    , mDeviceConnected( false )
    , mDisplayThread( NULL )
  	, mDisplay( NULL )
    , mAcquiringImages(false)
{

    mStream = new PvStreamGEV;
    mPipeline = new PvPipeline( mStream );
    mPipeline->RegisterEventSink( this );
    mDisplay = new PvDisplayWnd;
    mDeviceWnd = new PvGenBrowserWnd();
    mCommunicationWnd = new PvGenBrowserWnd();
    mStreamWnd = new PvGenBrowserWnd();
    mDisplayThread = new DisplayThread(mDisplay);
 
    CreateLayout();
}

///

/// \brief Destructor
///


IGVTriggerUISampleDlg::~IGVTriggerUISampleDlg()
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

    if ( mStreamWnd != NULL )
    {
        delete mStreamWnd;
        mStreamWnd = NULL;
    }

    if ( mDisplayThread != NULL )
    {
        delete mDisplayThread;
    	mDisplayThread = NULL;
    }

    if ( mPipeline != NULL )
    {
        assert( !mPipeline->IsStarted() );

        ///
        mPipeline->UnregisterEventSink( this );

        delete mPipeline;
        mPipeline = NULL;
    }

    if ( mStream != NULL )
    {
        assert( !mStream->IsOpen() );

        delete mStream;
        ///
        mStream = NULL;
    }

    mDevice.UnregisterEventSink(this);
}


///
/// \brief Create dialog layout
///

void IGVTriggerUISampleDlg::CreateLayout()
{
    QGroupBox *lConnectionBox = CreateConnectGroup();
    QGroupBox *lTriggerBox = CreateTrigger();
    QGroupBox *lAcqBox = CreateAcquisition();
    QGroupBox *lParametersBox = CreateParameters();
    QGroupBox *lDisplayBox = CreateDisplay();

    QVBoxLayout *lLayoutLeft = new QVBoxLayout();
    lLayoutLeft->addWidget( lConnectionBox, Qt::AlignTop );
    lLayoutLeft->addStretch();
    lLayoutLeft->addWidget( lTriggerBox, Qt::AlignTop );
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
    setWindowTitle( tr( "IGVTriggerUISampleDlg" ) );

    EnableInterface();

}

///
/// \brief Create connect group box
///

QGroupBox *IGVTriggerUISampleDlg::CreateConnectGroup()
{
    mConnectButton = new QPushButton( tr( "Select/&Connect" ) );
    mConnectButton->setMinimumHeight( 27 );
    QObject::connect( mConnectButton, SIGNAL( clicked() ), this, SLOT( OnConnectButton() ) );

    mDisconnectButton = new QPushButton( tr( "&Disconnect" ) );
    mDisconnectButton->setMinimumHeight( 27 );
    QObject::connect( mDisconnectButton, SIGNAL( clicked() ), this, SLOT( OnDisconnectButton() ) );

    QHBoxLayout *lButtonsLayout = new QHBoxLayout;
    lButtonsLayout->addWidget( mConnectButton );
    lButtonsLayout->addWidget( mDisconnectButton );

    QLabel *lIpAddressLabel = new QLabel( tr( "IP Address" ) );
    mIpAddressLine = new QLineEdit;
    mIpAddressLine->setReadOnly( true );
    mIpAddressLine->setEnabled( false );

    QLabel *lMacAddressLabel = new QLabel( tr( "MAC Address" ) );
    mMacAddressLine = new QLineEdit;
    mMacAddressLine->setReadOnly( true );
    mMacAddressLine->setEnabled( false );

    QLabel *lManufacturerLabel = new QLabel( tr( "Manufacturer" ) );
    mManufacturerLine = new QLineEdit;
    mManufacturerLine->setReadOnly( true );
    mManufacturerLine->setEnabled( false );

    QLabel *lModelLabel = new QLabel( tr( "Model" ) );
    mModelLine = new QLineEdit;
    mModelLine->setReadOnly( true );
    mModelLine->setEnabled( false );

    QLabel *lNameLabel = new QLabel( tr( "Name" ) );
    mNameLine = new QLineEdit;
    mNameLine->setReadOnly( true );
    mNameLine->setEnabled( false );

    QGridLayout *lGridLayout = new QGridLayout;
    lGridLayout->addWidget( lIpAddressLabel, 0, 0 );
    lGridLayout->addWidget( mIpAddressLine, 0, 1, 1, 2 );
    lGridLayout->addWidget( lMacAddressLabel, 1, 0 );
    lGridLayout->addWidget( mMacAddressLine, 1, 1, 1, 2 );
    lGridLayout->addWidget( lManufacturerLabel, 2, 0 );
    lGridLayout->addWidget( mManufacturerLine, 2, 1, 1, 2 );
    lGridLayout->addWidget( lModelLabel, 3, 0 );
    lGridLayout->addWidget( mModelLine, 3, 1, 1, 2 );
    lGridLayout->addWidget( lNameLabel, 4, 0 );
    lGridLayout->addWidget( mNameLine, 4, 1, 1, 2 );

    QVBoxLayout *lBoxLayout = new QVBoxLayout;
    lBoxLayout->addLayout( lButtonsLayout );
    lBoxLayout->addLayout( lGridLayout );
    lBoxLayout->addStretch();

    QGroupBox *lConnectionBox = new QGroupBox( tr( "Connection" ) );
    lConnectionBox->setLayout( lBoxLayout );
    lConnectionBox->setMinimumWidth( 300);
    lConnectionBox->setMaximumWidth( 300 );
    lConnectionBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lConnectionBox;
}


///
/// \brief Create acquisition group box
///

QGroupBox *IGVTriggerUISampleDlg::CreateAcquisition()
{

    QLabel *lModeLabel = new QLabel( tr( "Mode" ) );
    mModeCombo = new QComboBox;
    mModeCombo->setEnabled( false );

    QObject::connect( mModeCombo, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeMode( int ) ) );

    QGridLayout *lAcqGridLayout = new QGridLayout;
    lAcqGridLayout->addWidget( lModeLabel, 0, 0 );
    lAcqGridLayout->addWidget( mModeCombo, 0, 1, 1, 4 );

    ///
    mStartButton = new QPushButton( tr( "Play" ) );
    mStartButton->setMinimumHeight( 45 );
    mStartButton->setEnabled( false );

    QPixmap lPlayPixmap(":IGVTriggerUISample/res/play.bmp");
    lPlayPixmap.setMask(lPlayPixmap.createMaskFromColor(QColor(0xFF, 0xFF, 0xFF)));
    mStartButton->setIcon(QIcon(lPlayPixmap));

    mStopButton = new QPushButton( tr( "Stop" ) );
    mStopButton->setMinimumHeight( 45 );
    mStopButton->setEnabled( false );

    QPixmap lStopPixmap(":IGVTriggerUISample/res/stop.bmp");
    lStopPixmap.setMask(lStopPixmap.createMaskFromColor(QColor(0xFF, 0xFF, 0xFF)));
    mStopButton->setIcon(QIcon(lStopPixmap));

    QObject::connect( mStartButton, SIGNAL( clicked() ), this, SLOT( OnStart() ) );
    QObject::connect( mStopButton, SIGNAL( clicked() ), this, SLOT( OnStop() ) );

    QHBoxLayout *lButtonsLayout = new QHBoxLayout;
    lButtonsLayout->addWidget( mStartButton );
    lButtonsLayout->addWidget( mStopButton );

    QVBoxLayout *lAcqLayout = new QVBoxLayout;
    lAcqLayout->addLayout( lAcqGridLayout );
    lAcqLayout->addLayout( lButtonsLayout );
    lAcqLayout->addStretch();

    QGroupBox *lAcqBox = new QGroupBox( tr( "Acquisition Control" ) );
    lAcqBox->setLayout( lAcqLayout );
    lAcqBox->setMinimumWidth( 300 );
    lAcqBox->setMaximumWidth( 300 );
    lAcqBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lAcqBox;
}


///
/// \brief Create Trigger group box
///

QGroupBox *IGVTriggerUISampleDlg::CreateTrigger()
{
    QLabel *lTriggerModeLabel = new QLabel( tr( "Mode" ) );
    mTriggerMode = new QComboBox;
    mTriggerMode->setEnabled( false );

    QObject::connect( mTriggerMode, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeTriggerMode( int ) ) );

    QLabel *lTriggerSourceLabel = new QLabel( tr( "Source" ) );
    mTriggerSource = new QComboBox;
    mTriggerSource->setEnabled( false );

    QObject::connect( mTriggerSource, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeTriggerSource( int ) ) );

    QLabel *lTriggerTypeLabel = new QLabel( tr( "Type" ) );
    mTriggerType = new QComboBox;
    mTriggerType->setEnabled( false );

    QObject::connect( mTriggerType, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeTriggerType( int ) ) );

    QLabel *lTriggerActivationLabel = new QLabel( tr( "Activation" ) );
    mTriggerActivation = new QComboBox;
    mTriggerActivation->setEnabled( false );

    QObject::connect( mTriggerActivation, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeTriggerActivation( int ) ) );

    QLabel *lTriggerDebounceLabel = new QLabel( tr( "Debounce" ) );
    mTriggerDebounce = new QComboBox;
    mTriggerDebounce->setEnabled( false );

    QObject::connect( mTriggerDebounce, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeTriggerDebounce( int ) ) );

    ///
   
    QGridLayout *lTriggerGridLayout = new QGridLayout;
    lTriggerGridLayout->addWidget( lTriggerModeLabel, 0, 0 );
    lTriggerGridLayout->addWidget( mTriggerMode, 0, 1, 1, 1 );
    lTriggerGridLayout->addWidget( lTriggerSourceLabel, 0, 2 );
    lTriggerGridLayout->addWidget( mTriggerSource, 0, 3, 1, 2 );
    lTriggerGridLayout->addWidget( lTriggerTypeLabel, 1, 0);
    lTriggerGridLayout->addWidget( mTriggerType, 1, 1, 1, 4);
    lTriggerGridLayout->addWidget( lTriggerActivationLabel, 2,0,1,1);
    lTriggerGridLayout->addWidget( mTriggerActivation, 2, 1, 1, 3);
    lTriggerGridLayout->addWidget( lTriggerDebounceLabel, 3, 0,1,1);
    lTriggerGridLayout->addWidget( mTriggerDebounce, 3, 1, 1, 3);

    mFlashSWTrigger= new QPushButton( tr( "Flash Software Trigger" ) );
    mFlashSWTrigger->setMinimumHeight( 45 );
    mFlashSWTrigger->setEnabled( false );

    QObject::connect( mFlashSWTrigger, SIGNAL( clicked() ), this, SLOT( OnBnClickedTriggerSwCommand() ) );

    QHBoxLayout *lButtonsLayout = new QHBoxLayout;
    lButtonsLayout->addWidget( mFlashSWTrigger );

    QVBoxLayout *lTriggerLayout = new QVBoxLayout;
    lTriggerLayout->addLayout( lTriggerGridLayout );
    lTriggerLayout->addLayout( lButtonsLayout );
    lTriggerLayout->addStretch();

    QGroupBox *lTriggerBox = new QGroupBox( tr( "TriggerControl" ) );
    lTriggerBox->setLayout( lTriggerLayout );
    lTriggerBox->setMinimumWidth( 300 );
    lTriggerBox->setMaximumWidth( 300 );
    lTriggerBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lTriggerBox;

}

///
///
/// \brief Create parameters group box
///

QGroupBox *IGVTriggerUISampleDlg::CreateParameters()
{
    mCommunicationButton = new QPushButton( tr( "Communication control" ) );
    mCommunicationButton->setMinimumHeight( 27 );

    mDeviceButton = new QPushButton( tr( "Device control" ) );
    mDeviceButton->setMinimumHeight( 27 );

    mStreamButton = new QPushButton( tr( "Image stream control" ) );
    mStreamButton->setMinimumHeight( 27 );

    QObject::connect( mCommunicationButton, SIGNAL( clicked() ), this, SLOT( OnShowCommParameters() ) );
    QObject::connect( mDeviceButton, SIGNAL( clicked() ), this, SLOT( OnShowDeviceParameters() ) );
    QObject::connect( mStreamButton, SIGNAL( clicked() ), this, SLOT( OnShowStreamParameters() ) );

    QVBoxLayout *layoutParameters = new QVBoxLayout;
    layoutParameters->addWidget( mCommunicationButton );
    layoutParameters->addWidget( mDeviceButton );
    layoutParameters->addWidget( mStreamButton );
    layoutParameters->addStretch();

    QGroupBox *parametersBox = new QGroupBox( tr( "Parameters and Control" ) );
    parametersBox->setLayout( layoutParameters );
    parametersBox->setMinimumWidth( 300 );
    parametersBox->setMaximumWidth( 300 );

    return parametersBox;
}

///
/// \brief Menu action handler
///

void IGVTriggerUISampleDlg::OnFileExitAction()
{
    Disconnect();
    close();
}
///
///
/// \brief Create display
///


QGroupBox *IGVTriggerUISampleDlg::CreateDisplay()
{
    QVBoxLayout *displayLayout = new QVBoxLayout;
    displayLayout->addWidget( mDisplay->GetQWidget() );

    QGroupBox *lDisplayBox = new QGroupBox( tr( "Display" ) );
    lDisplayBox->setLayout( displayLayout );
    lDisplayBox->setMinimumWidth( 600 );

    return lDisplayBox;
}

///


void IGVTriggerUISampleDlg::CreateEnumComboBox(QComboBox* paramComboBox, char* paramName)
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
        int64_t  lComboValue = lData.value<int64_t>();
        
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

void IGVTriggerUISampleDlg::OnRegisterInterface()
{
    PvRegisterInterfaceWnd lInterface;
    lInterface.SetDevice( &mDevice );
    lInterface.ShowModal( this );
}


///
/// \brief Connect button click handler
///

void IGVTriggerUISampleDlg::OnConnectButton()
{
    // create a device finder wnd and open the select device dialog
    PvDeviceFinderWnd lFinder;
    PvResult lResult = lFinder.ShowModal( this );
    if ( ( !lResult.IsOK() ) || ( lFinder.GetSelected() == NULL ) )
    {
        return;
    }

    Connect( (PvDeviceInfoGEV*)lFinder.GetSelected());
}


///
/// \brief Disconnect button click handler
///

void IGVTriggerUISampleDlg::OnDisconnectButton()
{
    Disconnect();
}


///
/// \brief Start button click handler
///

void IGVTriggerUISampleDlg::OnStart()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    StartAcquisition();

}


///
/// \brief Stop button click handler
///

void IGVTriggerUISampleDlg::OnStop()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    PvGenCommand *lStop = mDevice.GetParameters()->GetCommand( "AcquisitionStop" );
    if ( lStop->IsAvailable() )
    {
        StopAcquisition();
    }

}


///
/// \brief Syncs the UI enabled state
///
void IGVTriggerUISampleDlg::EnableInterface()
{

    mDeviceConnected = mDevice.IsConnected();

    mConnectButton->setEnabled( !mDeviceConnected && !mStream->IsOpen() );
    mDisconnectButton->setEnabled( mDeviceConnected || mStream->IsOpen() );

	EnableGenBrowsers( true );
	EnableControls( mDeviceConnected );
}


///
/// \brief Enables Start, Stop contro mMainDlg->IsMultiSourceTransmitter( &mMainDlg->mDevice);
///

void IGVTriggerUISampleDlg::EnableControls( bool aEnabled )
{
    bool lEnabled = aEnabled;

    bool lModeEnabled = false;
	bool lStartEnabled = false;
	bool lStopEnabled = false;
    bool lTriggerModeEnabled = false;
    bool lTriggerSourceEnabled = false;
    bool lTriggerTypeEnabled = false;
    bool lTriggerActivationEnabled = false;
    bool lTriggerDebounceEnabled = false;
    bool lTriggerFlashSWEnabled = false;

	PvGenCommand *lStart = mDevice.GetParameters()->GetCommand( "AcquisitionStart" );
    PvGenCommand *lStop = mDevice.GetParameters()->GetCommand( "AcquisitionStop" );
	PvGenEnum *lMode = mDevice.GetParameters()->GetEnum( "AcquisitionMode" );
    PvGenEnum *lTriggerMode = mDevice.GetParameters()->GetEnum( "TriggerMode" );
	PvGenEnum *lTriggerSource = mDevice.GetParameters()->GetEnum( "TriggerSource" );
	PvGenEnum *lTriggerType = mDevice.GetParameters()->GetEnum( "TriggerType" );
	PvGenEnum *lTriggerActivation = mDevice.GetParameters()->GetEnum( "TriggerActivation" );
	PvGenEnum *lTriggerDebounce = mDevice.GetParameters()->GetEnum("TriggerDebounce");
    PvGenCommand *lTriggerFlashSW = mDevice.GetParameters()->GetCommand("TriggerSoftware");

    if ( lMode != NULL )
    {
        lModeEnabled = lEnabled &&
            lMode->IsWritable() ;
    }

    if ( lStart != NULL )
    {
        lStartEnabled = lEnabled;
    }

    if ( lStop != NULL )
    {
        lStopEnabled = lEnabled;
    }
    
    if ( lTriggerMode != NULL )
    {
        lTriggerModeEnabled = lEnabled &&
            lTriggerMode->IsWritable() ;
    }
    
    if ( lTriggerSource != NULL )
    {
        lTriggerSourceEnabled = lEnabled &&
                lTriggerSource->IsWritable();
    }
    
    
    if ( lTriggerType != NULL )
    {
        lTriggerTypeEnabled = lEnabled &&
            lTriggerType->IsWritable();
    }
    
    if ( lTriggerActivation != NULL )
    {
        lTriggerActivationEnabled = lEnabled &&
            lTriggerActivation->IsWritable();
    }

    if ( lTriggerDebounce != NULL )
    {
        lTriggerDebounceEnabled = lEnabled &&
            lTriggerDebounce->IsWritable();
    }

    if ( lTriggerFlashSW != NULL )
    {
        lTriggerFlashSWEnabled = lEnabled &&
            lTriggerFlashSW->IsWritable() ;
    }
	mStartButton->setEnabled( lStartEnabled );
	mStopButton->setEnabled( lStopEnabled );
	mModeCombo->setEnabled( lModeEnabled );
    mTriggerMode->setEnabled(lTriggerModeEnabled);
    mTriggerSource->setEnabled(lTriggerSourceEnabled);
    mTriggerType->setEnabled(lTriggerTypeEnabled);
    mTriggerActivation->setEnabled(lTriggerActivationEnabled);
    mTriggerDebounce->setEnabled(lTriggerDebounceEnabled);
    mFlashSWTrigger->setEnabled(lTriggerFlashSWEnabled);
}


///
/// \brief Enables the buttons used to open the browsers
///

void IGVTriggerUISampleDlg::EnableGenBrowsers( bool aEnabled )
{
    mDeviceButton->setEnabled( aEnabled && mDeviceConnected );
    mStreamButton->setEnabled( aEnabled && mStream->IsOpen() );
    mCommunicationButton->setEnabled( aEnabled );
}


///
/// \brief Shows one of the GenICam browsers
///

void IGVTriggerUISampleDlg::ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aArray, const QString &aTitle )
{
    if ( aWnd->GetQWidget()->isVisible() )
    {
        // If already open, just toggle to closed...
        CloseGenWindow( aWnd );
        return;
    }

    // Create, assign parameters, set title and show modelessPvDeviceInfoGEV
    aWnd->SetTitle( aTitle.toAscii().data() );
    
    aWnd->ShowModeless(this);
    aWnd->SetGenParameterArray( aArray );
}

///
/// \brief Closes a GenICam tree browser.
///

void IGVTriggerUISampleDlg::CloseGenWindow( PvGenBrowserWnd *aWnd )
{
    if ( aWnd->GetQWidget()->isVisible() )
    {
        aWnd->Close();
    }
}


///
/// \brief Connects the device/stream
///


void IGVTriggerUISampleDlg::Connect( PvDeviceInfoGEV *aDI )
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
       usleep(2500 * 1000);  //Windows Sleep(200) ::Sleep( 2500 );
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
    mDevice.RegisterEventSink(this);

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

    mManufacturerLine->setText( lManufacturerStr.GetAscii());
    mModelLine->setText( lModelNameStr.GetAscii());
    mIpAddressLine->setText( lIPStr.toAscii());
    mMacAddressLine->setText( lMACStr.toAscii());
    mNameLine->setText( lNameStr.GetAscii()); 

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

    // Initialize the trigger control
    CreateEnumComboBox(mTriggerMode, (char*)"TriggerMode");
    CreateEnumComboBox(mTriggerSource, (char*)"TriggerSource");
    CreateEnumComboBox(mTriggerType, (char*)"TriggerType");
    CreateEnumComboBox(mTriggerActivation, (char*)"TriggerActivation");
    CreateEnumComboBox(mTriggerDebounce, (char*)"TriggerDebounce");

    mFlashSWTrigger->setEnabled(true);

    // Ready image reception
    StartStreaming();

    // Sync up UI
    EnableInterface();
}
// =============================================================================
void IGVTriggerUISampleDlg::Disconnect()
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
    CloseGenWindow( mStreamWnd );

    // If streaming, stop streaming
    StopStreaming();

    // Reset device ID - can be called by the destructor when the window
    // no longer exists, be careful...
    if (this->handle() ) //!= NULL) // ! = 0)
    {

        // Reset device ID
        mManufacturerLine->setText( "" );
        mModelLine->setText( "" );
        mIpAddressLine->setText( "" );
        mMacAddressLine->setText( "" );
        mNameLine->setText( "" );

    }

    mDisplay->Clear();


    mDevice.Disconnect();

    mStream->Close();

    EnableInterface();

    mModeCombo->clear();
}


///
/// \brief Start display thread, arm pipeline
///

void IGVTriggerUISampleDlg::StartStreaming()
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

void IGVTriggerUISampleDlg::StopStreaming()
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
void IGVTriggerUISampleDlg::StartAcquisition()
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
void IGVTriggerUISampleDlg::StopAcquisition()
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

void IGVTriggerUISampleDlg::OnBnClickedTriggerSwCommand()
{
    // Get acquisition mode parameter
    PvGenCommand *lTriggerSoftware = mDevice.GetParameters()->GetCommand( "TriggerSoftware" );
    if ( lTriggerSoftware)
    {
        lTriggerSoftware->Execute();
        usleep(200 * 1000);  //Windows Sleep(200) (Windows uses milliseconds, Linux uses microseconds)
        lTriggerSoftware->Execute();
    }
    else
        assert(NULL); //return
}
///
/// \brief Acquisition mode combo box change handler
///

void IGVTriggerUISampleDlg::OnCbnSelchangeMode( int aIndex )
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
    // Retrieve item data value from combo box item, assign it to acquisition mode parameter
    QVariant lData = mModeCombo->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
           int64_t lValue = lData.value<int64_t>();
           PvResult lResult = lMode->SetValue( lValue );
           if ( !lResult.IsOK() )
           {
               PvMessageBox( this, lResult );
           }
    }
}

void IGVTriggerUISampleDlg::OnCbnSelchangeTriggerMode( int aIndex )
{
    if ( !mDeviceConnected )
    {
        return;
    }

    // No selection?
    if ( mTriggerMode->currentIndex() < 0 )
    {
        return;
    }

    PvGenEnum *lTriggerMode = dynamic_cast <PvGenEnum *>(mDevice.GetParameters()->GetEnum( "TriggerMode" ));

    if ( lTriggerMode == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to Trigger mode parameter
    QVariant lData = mTriggerMode->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = lTriggerMode->SetValue( lValue );
        if ( !lResult.IsOK() )
        {
			PvMessageBox( this, lResult );
        }
    }
}

void IGVTriggerUISampleDlg::OnCbnSelchangeTriggerSource( int aIndex )
{
    if ( !mDeviceConnected)
    {
        return;
    }

    // No selection?
    if ( mTriggerSource->currentIndex() < 0 )
    {
        return;
    }

    // Get Trigger Source parameter
    PvGenEnum *lTriggerSource = mDevice.GetParameters()->GetEnum( "TriggerSource" );
    if ( lTriggerSource == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to Trigger Source parameter
    QVariant lData = mTriggerSource->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = lTriggerSource->SetValue( lValue );
        if ( !lResult.IsOK() )
        {
			PvMessageBox( this, lResult );
        }
    }
}

void IGVTriggerUISampleDlg::OnCbnSelchangeTriggerType( int aIndex )
{
    if ( !mDeviceConnected )
    {
        return;
    }

    // No selection?
    if ( mTriggerType->currentIndex() < 0 )
    {
        return;
    }

    // Get Trigger Type parameter
    PvGenEnum *lTriggerType = mDevice.GetParameters()->GetEnum( "TriggerType" );
    if ( lTriggerType == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to Trigger Source parameter
    QVariant lData = mTriggerType->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = lTriggerType->SetValue( lValue );
        if ( !lResult.IsOK() )
        {
			PvMessageBox( this, lResult );
        }
    }
}
void IGVTriggerUISampleDlg::OnCbnSelchangeTriggerActivation( int aIndex )
{
    if ( !mDeviceConnected )
    {
        return;
    }

    // No selection?
    if ( mTriggerActivation->currentIndex() < 0 )
    {
        return;
    }

    // Get Trigger Activation parameter
    PvGenEnum *lTriggerActivation = mDevice.GetParameters()->GetEnum( "TriggerActivation" );
    if ( lTriggerActivation == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to Trigger Activation parameter
    QVariant lData = mTriggerActivation->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = lTriggerActivation->SetValue( lValue );
        if ( !lResult.IsOK() )
        {
			PvMessageBox( this, lResult );
        }
    }
}
void IGVTriggerUISampleDlg::OnCbnSelchangeTriggerDebounce( int aIndex )
{
    if ( !mDeviceConnected )
    {
        return;
    }

    // No selection?
    if ( mTriggerDebounce->currentIndex() < 0 )
    {
        return;
    }

    // Get Trigger Debounce parameter
    PvGenEnum *lTriggerDebounce = mDevice.GetParameters()->GetEnum( "TriggerDebounce" );
    if ( lTriggerDebounce == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to Trigger Debounce parameter
    QVariant lData = mTriggerDebounce->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = lTriggerDebounce->SetValue( lValue );
        if ( !lResult.IsOK() )
        {
			PvMessageBox( this, lResult );
        }
    }
}


void IGVTriggerUISampleDlg::OnParameterUpdate( PvGenParameter *aParameter )
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

bool IGVTriggerUISampleDlg::event( QEvent *aEvent )
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

void IGVTriggerUISampleDlg::OnShowCommParameters()
{
    ShowGenWindow(
        mCommunicationWnd,
        mDevice.GetCommunicationParameters(),
        "Communication Control" );
}


///
/// \brief Displays device parameters
///

void IGVTriggerUISampleDlg::OnShowDeviceParameters()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    ShowGenWindow(
        mDeviceWnd,
        mDevice.GetParameters(),
        "Device Control" );
}


///
/// \brief Display stream parameters
///

void IGVTriggerUISampleDlg::OnShowStreamParameters()
{
    if ( !mStream->IsOpen() )
    {
        return;
    }

    ShowGenWindow(
        mStreamWnd,
        mStream->GetParameters(),
        "Image Stream Control" );
}


void IGVTriggerUISampleDlg::OnUpdateAcquisitionModeEvent()
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
            }

        }

        mModeCombo->setEnabled(lEnum->IsWritable());
    }

    return;
}

///
/// \brief If in application thread context, send event if not, post
///

void IGVTriggerUISampleDlg::SendEventIfPossible( int aMsg )
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



