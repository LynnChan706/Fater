// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"

#include "pvmessagebox.h"
#include "errorsdlg.h"
#include "warningdlg.h"
#include "actioncommanddlg.h"
#include "filteringdlg.h"
#include "eventmonitordlg.h"
#include "setupdlg.h"
#include "imagesavedlg.h"
#include "displayoptionsdlg.h"
#include "windowsevent.h"
#include "progressdlg.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>

#include <assert.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>

#include <PvRegisterInterfaceWnd.h>
#include <PvDeviceInfoGEV.h>
#include <PvDeviceInfoU3V.h>


#define TAG_APP ( "ebusplayerconfig" )
#define TAG_DEFAULTCOMMGEVBROWSER ( "defaultcommgevbrowser" )
#define TAG_DEFAULTCOMMU3VBROWSER ( "defaultcommu3vbrowser" )
#define TAG_COMMUNICATIONBROWSER ( "communicationbrowser" )
#define TAG_DEVICEBROWSER ( "devicebrowser" )
#define TAG_STREAMBROWSER ( "streambrowser" )


///
/// \brief Constructor
///

eBUSPlayer::eBUSPlayer( QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
    , mStatusColor( SCDefault )
	, mDisplay( NULL )
	, mShowAcquisitionWarning( true )
    , mPlayer( NULL )
    , mSetupDlg( NULL )
    , mEventMonitorDlg( NULL )
    , mActionCommandDlg( NULL )
    , mFilteringDlg( NULL )
    , mDeviceWnd( NULL )
    , mCommunicationWnd( NULL )
    , mStreamWnd( NULL )
{
    mDisplay = new IPvDisplayAdapter;
    mPlayer = new Player( this, mDisplay );

    mFilteringDlg = new FilteringDlg( mPlayer->GetImageFiltering(), mPlayer->GetDisplayThread(), this );

    mDeviceWnd = new PvGenBrowserWnd;
    mCommunicationWnd = new PvGenBrowserWnd;
    mStreamWnd = new PvGenBrowserWnd;

    CreateLayout();

    SetPreferences();
}


///
/// \brief Destructor
///

eBUSPlayer::~eBUSPlayer()
{
    Disconnect();

    if ( mActionCommandDlg != NULL )
    {
        delete mActionCommandDlg;
        mActionCommandDlg = NULL;
    }

    if ( mFilteringDlg != NULL )
    {
        delete mFilteringDlg;
        mFilteringDlg = NULL;
    }

    if ( mPlayer != NULL )
    {
        delete mPlayer;
        mPlayer = NULL;
    }

    if ( mEventMonitorDlg != NULL )
    {
        delete mEventMonitorDlg;
        mEventMonitorDlg = NULL;
    }

    if ( mTimer != NULL )
    {
        delete mTimer;
        mTimer = NULL;
    }

    if ( mDisplay != NULL )
    {
        delete mDisplay;
        mDisplay = NULL;
    }

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
}


void eBUSPlayer::SetPreferences()
{
    // Create a defaults persistence file if it doesn't already exist
    QString lDefaultPath = GetDefaultPath();
    SaveConfig( lDefaultPath, false );

    if ( !mFileName.isEmpty() )
    {
        OpenConfig( mFileName );
    }
    else
    {
        // check for the existence of the sticky configuration file. If it exists, load it.
        QString lStickyPath = GetStickyPath();
        if( access( lStickyPath.toAscii().data(), 0 ) == 0 )
        {
            OpenConfig( lStickyPath );
        }
    }
}


///
/// \brief Syncs the UI enabled state
///

void eBUSPlayer::EnableInterface()
{
    mConnectButton->setEnabled( !mPlayer->IsDeviceConnected() && !mPlayer->IsStreamOpened() );
    mDisconnectButton->setEnabled( mPlayer->IsDeviceConnected() || mPlayer->IsStreamOpened() );

	EnableGenBrowsers( true );
	EnableControls( mPlayer->IsDeviceConnected() );
}


///
/// \brief Enables Start, Stop controls
///

void eBUSPlayer::EnableControls( bool aEnabled )
{
    ControlsState lState;
    mPlayer->GetControlsEnabled( lState );

    mStartButton->setEnabled( aEnabled && lState.mStart );
    mStopButton->setEnabled( aEnabled && lState.mStop );
    mModeCombo->setEnabled( aEnabled && lState.mAcquisitionMode );
    mSourceCombo->setEnabled( aEnabled && lState.mSource );
}


///
/// \brief Enables the buttons used to open the browsers
///

void eBUSPlayer::EnableGenBrowsers( bool aEnabled )
{
    mDeviceButton->setEnabled( aEnabled && mPlayer->IsDeviceConnected() );
    mStreamButton->setEnabled( aEnabled && mPlayer->IsStreamOpened() );
    mCommunicationButton->setEnabled( aEnabled && mPlayer->IsDeviceConnected() );
}


///
/// \brief Shows one of the GenICam browsers
///

void eBUSPlayer::ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aArray, const QString &aTitle )
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
    aWnd->SetGenParameterArray( aArray );
}


///
/// \brief Closes a GenICam tree browser.
///

void eBUSPlayer::CloseGenWindow( PvGenBrowserWnd *aWnd )
{
    if ( aWnd->GetQWidget()->isVisible() )
    {
    	aWnd->SetGenParameterArray( NULL );
        aWnd->Close();
    }
}


///
/// \brief Connects the device/stream
///

void eBUSPlayer::Connect( const PvDeviceInfo *aDI, PvConfigurationReader *aCR )
{
    assert( aDI != NULL );
    if ( aDI == NULL )
    {
        return;
    }

    if ( aDI != NULL )
    {
        PvDeviceClass lClass = PvDeviceClassTransmitter;

        const PvDeviceInfoGEV *lDeviceInfoGEV = dynamic_cast<const PvDeviceInfoGEV *>( aDI );
        if ( lDeviceInfoGEV != NULL )
        {
            lClass = lDeviceInfoGEV->GetClass();
        }

        Setup::Role lRole = mPlayer->GetSetupRole();

        if ( ( lClass == PvDeviceClassReceiver ) ||
             ( lClass == PvDeviceClassPeripheral ) ||
             ( lClass == PvDeviceClassTransceiver ) )
        {
            // For receiver and peripheral, we make sure  role is controller only
            if ( lRole == Setup::RoleCtrlData || lRole == Setup::RoleData )
            {
                QString lMessage;

                if ( ( lClass == PvDeviceClassReceiver ) ||
                     ( lClass == PvDeviceClassPeripheral ) )
                {
                    // Receiver and peripheral message
                    lMessage = "You have chosen to connect to a ";
                    lMessage += ( lClass == PvDeviceClassReceiver ) ? "receiver" : "peripheral";
                    lMessage += " however eBUS Player is currently set up to connect as a ";
                    lMessage += ( lRole == Setup::RoleCtrlData ) ? "controller and data receiver" : "data receiver";
                    lMessage += ". \r\n\r\nRe-attempt connection as a controller?";
                }
                else if ( lClass == PvDeviceClassTransceiver )
                {
                    // Transceiver message
                    lMessage = "You have chosen to connect to a transceiver. eBUS Player does not fully\r\n";
                    lMessage += "support transceivers in this version. Re-attempt connection as a \r\n";
                    lMessage += "controller?";
                }
                else
                {
                    assert( 0 );
                }

                QMessageBox::StandardButton lResult = QMessageBox::warning( this, "eBUS Player", lMessage, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
                if ( lResult == QMessageBox::No )
                {
                    return;
                }
                else
                {
                    mPlayer->ResetSetup();
                    mPlayer->GetSetup()->SetRole( Setup::RoleCtrl );
                }
            }
        }
    }

    // Connect the device and/or stream
    PvResult lResult = mPlayer->Connect( aDI, NULL );
    if ( !lResult.IsOK() )
    {
    	PvMessageBox( this, lResult );
    }

    CreateEventMonitor();
    UpdateDeviceAttributes();
    EnableInterface();
}


///
/// \brief Disconnects the device/stream
///

void eBUSPlayer::Disconnect()
{
    // Close all configuration child windows
    CloseGenWindow( mCommunicationWnd );
    CloseGenWindow( mDeviceWnd );
    CloseGenWindow( mStreamWnd );
    mFilteringDlg->close();
    mSerialTerminalWnd.Close();
    if ( mEventMonitorDlg != NULL )
    {
        mEventMonitorDlg->close();

        delete mEventMonitorDlg;
        mEventMonitorDlg = NULL;
    }

    // Call the model's disconnect method
    mPlayer->Disconnect();

    // Clear the display
    mDisplay->Clear();

    // Blank out UI
    mManufacturerLine->setText( "" );
    mModelLine->setText( "" );
    mIpAddressLine->setText( "" );
    mMacAddressLine->setText( "" );
    mNameLine->setText( "" );
    mGUIDLine->setText( "" );
    mStatusLine->setText( "" );
    SetStatusColor( SCDefault );

    // Sync the UI with our new status
    EnableInterface();
    setWindowTitle( "eBUS Player" );

    mSourceCombo->clear();
    mModeCombo->clear();
}


///
/// \brief Changes the status color (text and/or background)
///

void eBUSPlayer::SetStatusColor( StatusColor aColor )
{
    switch( aColor )
    {
    case SCRed:
        mStatusLine->setPalette( QPalette(  Qt::red ) );
        break;
    case SCYellow:
        mStatusLine->setPalette( QPalette( Qt::yellow ) );
        break;
    default:
        mStatusLine->setPalette( palette() );
        break;
    }
}


///
/// \brief Open a configuration file and restore previous state from it
///

void eBUSPlayer::OpenConfig( const QString &aFileName )
{
    if ( mCommunicationWnd->GetQWidget()->isVisible() )
    {
        QMessageBox::warning( this, "Error", "Please close the communication control window before loading a new configuration." );
        return;
    }
    if ( mStreamWnd->GetQWidget()->isVisible() )
    {
        QMessageBox::warning( this, "Error", "Please close the image stream control window before loading a new configuration." );
        return;
    }
    if ( mDeviceWnd->GetQWidget()->isVisible() )
    {
        QMessageBox::warning( this, "Error", "Please close the Device control window before loading a new configuration." );
        return;
    }

    // Create reader, load file
    PvConfigurationReader lReader;
    PvResult lResult = lReader.Load( aFileName.toAscii().data() );
    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
        return;
    }

    // We're loading a new persistence file. Report it now so that it's visible in the title bar while we're loading.
    if ( !mFileName.isEmpty() && ( aFileName == mFileName ) )
    {
        ReportMRU( mFileName );
    }

    // Check if roles are the same if not, we must disconnect first
    if ( ( mPlayer->IsDeviceConnected() || mPlayer->IsStreamOpened() ) && mPlayer->IsSameAsCurrent( &lReader ) )
    {
        QMessageBox::StandardButton lRetVal = QMessageBox::question( this, "Warning",
            "Setup of this configuration file does not match the current connection.\r\n\r\nIn order to proceed with the operation, "
            "the GigE Vision Device will be disconnected.\r\n\r\nContinue restoring configuration?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
        if ( lRetVal == QMessageBox::No )
        {
            return;
        }

        Disconnect();
    }

    bool lContextOnly = ( lReader.GetDeviceCount() <= 0 ) && ( lReader.GetStreamCount() <= 0 );
    if ( !lContextOnly )
    {
        // If we're connected, just apply the settings. Otherwise connect from the settings in the persistence file.
        if ( mPlayer->IsDeviceConnected() )
        {
            QMessageBox::StandardButton lRetVal = QMessageBox::question( this, "Warning",
                "Opening a file now will apply the settings to the currently connected device. \r\n\r\nIf you wish to connect to a different device associated "
                "with the file, disconnect first, then open the file. \r\n\r\nContinue opening file?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
            if ( lRetVal == QMessageBox::No )
            {
                return;
            }
        }
    }

    PvStringList lErrorList;
    mPlayer->OpenConfig( &lReader, lErrorList, !lContextOnly );

    if ( lErrorList.GetSize() > 0 )
    {
        ErrorsDlg lDlg( this, &lErrorList );
        lDlg.exec();
    }

    PvPropertyList lCommunicationBrowserOptions;
    if ( lReader.Restore( TAG_COMMUNICATIONBROWSER, &lCommunicationBrowserOptions ).IsOK() )
    {
        mCommunicationWnd->Load( lCommunicationBrowserOptions );
    }
    PvPropertyList lDeviceBrowserOptions;
    if ( lReader.Restore( TAG_DEVICEBROWSER, &lDeviceBrowserOptions ).IsOK() )
    {
        mDeviceWnd->Load( lDeviceBrowserOptions );
    }
    PvPropertyList lStreamBrowserOptions;
    if ( lReader.Restore( TAG_STREAMBROWSER, &lStreamBrowserOptions ).IsOK() )
    {
        mStreamWnd->Load( lStreamBrowserOptions );
    }
    PvPropertyList lDefaultCommU3VBrowserOptions;
    if ( lReader.Restore( TAG_DEFAULTCOMMU3VBROWSER, &lDefaultCommU3VBrowserOptions ).IsOK() )
    {
        mDefaultCommU3VParametersWnd.Load( lDefaultCommU3VBrowserOptions );
    }
    PvPropertyList lDefaultCommGEVBrowserOptions;
    if ( lReader.Restore( TAG_DEFAULTCOMMGEVBROWSER, &lDefaultCommGEVBrowserOptions ).IsOK() )
    {
        mDefaultCommGEVParametersWnd.Load( lDefaultCommGEVBrowserOptions );
    }

    CreateEventMonitor();
    UpdateDeviceAttributes();
    mFilteringDlg->ToDialog();
    EnableInterface();
}


///
/// \brief Saves the eBUS Player configuration
///

void eBUSPlayer::SaveConfig( const QString &aLocation, bool aSaveConnectedDevice )
{
    // Create configuration writer
    PvConfigurationWriter lWriter;
    lWriter.Store( "1", TAG_APP );

    // Save GenICam browsers configuration
    PvPropertyList lPropertyList;
    mCommunicationWnd->Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_COMMUNICATIONBROWSER );
    mDeviceWnd->Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_DEVICEBROWSER );
    mStreamWnd->Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_STREAMBROWSER );
    mDefaultCommU3VParametersWnd.Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_DEFAULTCOMMU3VBROWSER );
    mDefaultCommGEVParametersWnd.Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_DEFAULTCOMMGEVBROWSER );

    // Save application model
    PvStringList lErrorList;
    mPlayer->SaveConfig( &lWriter, lErrorList, aSaveConnectedDevice );

    // Save the data contained in the writer to file
    lWriter.Save( aLocation.toAscii().data() );

    // If the error list is not empty, show the errors
    if ( lErrorList.GetSize() > 0 )
    {
        ErrorsDlg lDlg( this, &lErrorList );
        lDlg.exec();
    }
}


///
/// \brief Returns the path for default parameters
///

QString eBUSPlayer::GetStickyPath()
{
    return GetAppDataPath() + "eBUSPlayer.pvcfg";
}


///
/// \brief Returns the application path
///

QString eBUSPlayer::GetAppDataPath()
{
    QString lLocation;
#ifdef WIN32
    TCHAR lAppData[MAX_PATH];
    if ( SHGetSpecialFolderPath(      
        GetDesktopWindow(),
        lAppData,
        CSIDL_APPDATA,
        TRUE ) )
    {
        lLocation += lAppData;
    }
    lLocation += "\\Pleora\\";
    // create the folder if it doesn't exist
    if ( access( lLocation.toAscii().data(), 0 ) != 0 )
    {
        // default security attributes should suffice
        CreateDirectory( lLocation.toAscii().data(), NULL );
    }
#endif // WIN32
    
#ifdef _UNIX_
    passwd *lPW = getpwuid( getuid() );
    if ( lPW )
    {
        lLocation += lPW->pw_dir;
    }
    lLocation += "/.config";
    
    // create the folder if it doesn't exist
    if ( access( lLocation.toAscii().data(), 0 ) != 0 )
    {
        mkdir( lLocation.toAscii().data(), S_IRWXU | S_IRGRP | S_IXGRP );
    }
    
    
    lLocation += "/Pleora/";

    // create the folder if it doesn't exist
    if ( access( lLocation.toAscii().data(), 0 ) != 0 )
    {
        mkdir( lLocation.toAscii().data(), S_IRWXU | S_IRGRP | S_IXGRP );
    }
#endif

    return lLocation;
}


///
/// \brief Returns the path where we save the default parameters
///

QString eBUSPlayer::GetDefaultPath()
{
    return GetAppDataPath() + "eBUSPlayerDefaults.pvcfg";
}


///
/// \brief Creates the event monitor dialog
///

void eBUSPlayer::CreateEventMonitor()
{
    if ( !mPlayer->IsDeviceConnected() && !mPlayer->IsStreamOpened() )
    {
        return;
    }

    if ( mEventMonitorDlg == NULL )
    {
        mEventMonitorDlg = new EventMonitorDlg( this, mPlayer->GetLogBuffer() );
    }

    mEventMonitorDlg->LoadData();
}


///
/// \brief Updates the device attributes
///

void eBUSPlayer::UpdateDeviceAttributes()
{
    if ( mPlayer->IsDeviceConnected() || mPlayer->IsStreamOpened() )
    {
        DeviceAttributes &lAttributes = mPlayer->GetDeviceAttributes();

        mManufacturerLine->setText( lAttributes.mVendor.GetAscii() );
        mModelLine->setText( lAttributes.mModel.GetAscii() );
        mNameLine->setText( lAttributes.mName.GetAscii() );

        mIpAddressLine->setText( lAttributes.mIP.GetAscii() );
        mMacAddressLine->setText( lAttributes.mMAC.GetAscii() );
        mGUIDLine->setText( lAttributes.mGUID.GetAscii() );
    }
}


///
/// \brief If in application thread context, send event if not, post
///

intptr_t eBUSPlayer::SendMsgIfPossible( uint32_t Msg, uintptr_t wParam, intptr_t lParam )
{
    if ( QCoreApplication::instance()->thread() == QThread::currentThread() )
    {
        // Same thread, use send (ends up being a direct call)
        QCoreApplication::sendEvent( this, new QWindowsEvent( static_cast<QEvent::Type>( Msg ), wParam, lParam ) );
    }
    else
    {
        // Different thread, we want to use post to prevent deadlocks
        QCoreApplication::postEvent( this, new QWindowsEvent( static_cast<QEvent::Type>( Msg ), wParam, lParam ) );
    }

    return 0;
}


///
/// \brief If in application thread context, send event if not, post
///

intptr_t eBUSPlayer::PostMsg( uint32_t Msg, uintptr_t wParam, intptr_t lParam )
{
    QCoreApplication::postEvent( this, new QWindowsEvent( static_cast<QEvent::Type>( Msg ), wParam, lParam ) );

    return 0;
}


///
/// \brief If in application thread context, send event if not, post
///

intptr_t eBUSPlayer::SendMsg( uint32_t Msg, uintptr_t wParam, intptr_t lParam )
{
    QCoreApplication::sendEvent( this, new QWindowsEvent( static_cast<QEvent::Type>( Msg ), wParam, lParam ) );

    return 0;
}


///
/// \brief Player callback interface. Start the timer.
///

void eBUSPlayer::StartTimer()
{
    mTimer->start();
}


///
/// \brief Player callback interface. Stop the timer.
///

void eBUSPlayer::StopTimer()
{
    mTimer->stop();
}


///
/// \brief Player callback interface. Create and return a progress dialog. The caller is reponsible of releasing the object.
///

IProgress *eBUSPlayer::CreateProgressDialog()
{
    return new ProgressDlg( this );
}


