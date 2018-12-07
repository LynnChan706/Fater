// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "windowsevent.h"
#include "pvmessagebox.h"

#include <PvDeviceFinderWnd.h>

#include <assert.h>


#define WM_IMAGEDISPLAYED ( WM_USER + 0x4431 )


///
/// \brief Generic event handler/dispatcher
///

bool eBUSPlayer::event( QEvent *aEvent )
{
    switch ( aEvent->type() )
    {
    case WM_IMAGEDISPLAYED:
        OnImageDisplayed( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_LINKDISCONNECTED:
        OnLinkDisconnected( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_LINKRECONNECTED:
        OnLinkReconnected( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_UPDATESOURCE:
        OnUpdateSource( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_UPDATESOURCES:
        OnUpdateSources( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_UPDATEACQUISITIONMODE:
        OnUpdateAcquisitionMode( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_UPDATEACQUISITIONMODES:
        OnUpdateAcquisitionModes( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;

    case WM_ACQUISITIONSTATECHANGED:
        OnAcquisitionStateChanged( dynamic_cast<QWindowsEvent *>( aEvent ) );
        break;
    }

    return QWidget::event( aEvent );
}


///
/// \brief Connect button click handler
///

void eBUSPlayer::OnConnectButton()
{
    // create a device finder wnd and open the select device dialog
    PvDeviceFinderWnd lFinder;
    lFinder.SetTitle( "Device Selection" );

    PvResult lResult = lFinder.ShowModal();
    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
    {
        return;
    }

    Connect( lFinder.GetSelected() );
}


///
/// \brief Disconnect button click handler
///

void eBUSPlayer::OnDisconnectButton()
{
    Disconnect();
}


///
/// \brief Start button click handler
///

void eBUSPlayer::OnStart()
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    mPlayer->Play();
}


///
/// \brief Stop button click handler
///

void eBUSPlayer::OnStop()
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    SetStatusColor( SCDefault );
    mStatusLine->setText( "" );

    mPlayer->Stop();
}


///
/// \brief Acquisition mode combo box change handler
///

void eBUSPlayer::OnCbnSelchangeMode( int aIndex )
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    // No selection?
    if ( mModeCombo->currentIndex() < 0 )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to acquisition mode parameter
    QVariant lData = mModeCombo->itemData( aIndex );
    if ( lData.canConvert<int64_t>() )
    {
        int64_t lValue = lData.value<int64_t>();
        PvResult lResult = mPlayer->SetAcquisitionMode( lValue );
        if ( !lResult.IsOK() )
        {
            PvMessageBox( this, lResult );
        }
    }
}


///
/// \brief Image displayed async handler.
///
/// Typically called from the display thread.
///

void eBUSPlayer::OnImageDisplayed( QWindowsEvent *aEvent )
{
    // Stream opened, image save dialog exists, thread display is up
    if ( !mPlayer->IsStreamStarted() )
    {
        return;
    }

    PvString lText;
    bool lRecording = false;
    mPlayer->GetStatusText( lText, lRecording );
    if ( lRecording )
    {
        SetStatusColor( SCRed );
    }
    else
    {
        SetStatusColor( SCDefault );
    }

    mStatusLine->setText( lText.GetAscii() );
}


///
/// \brief Displays communication parameters
///

void eBUSPlayer::OnShowCommParameters()
{
    ShowGenWindow(
        mCommunicationWnd,
        mPlayer->GetCommunicationParameters(),
        "Communication Control" );
}


///
/// \brief Displays device parameters
///

void eBUSPlayer::OnShowDeviceParameters()
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    ShowGenWindow(
        mDeviceWnd,
        mPlayer->GetPvDevice()->GetParameters(),
        "Device Control" );
}


///
/// \brief Display stream parameters
///

void eBUSPlayer::OnShowStreamParameters()
{
    if ( !mPlayer->IsStreamOpened() )
    {
        return;
    }

    ShowGenWindow(
        mStreamWnd,
        mPlayer->GetStreamParameters(),
        "Image Stream Control" );
}


///
/// \brief Event handler to complete async disconnect in UI thread
///

void eBUSPlayer::OnLinkDisconnected( QWindowsEvent *aEvent )
{
    mPlayer->LinkDisconnected();

    if ( mPlayer->IsLinkRecoveryEnabled() )
    {
        SetStatusColor( SCYellow );
        mStatusLine->setText( tr( "Connection to device lost..." ) );

        CloseGenWindow( mDeviceWnd );
        CloseGenWindow( mCommunicationWnd );
        CloseGenWindow( mStreamWnd );

        EnableGenBrowsers( false );
        EnableControls( false );
    }
    else
    {
        SetStatusColor( SCDefault );
        mStatusLine->setText( "" );

        Disconnect();

        QMessageBox::warning( this, "eBUS Player", "Connection to device lost." );
    }
}


///
/// \brief Event handler to complete link reconnect in UI thread
///

void eBUSPlayer::OnLinkReconnected( QWindowsEvent *aEvent )
{
    mPlayer->Recover();

    SetStatusColor( SCDefault );
    mStatusLine->setText("" );

    EnableInterface();
}


///
/// \brief Timer event. Refresh stream status bar
///

void eBUSPlayer::OnTimer()
{
    SendMsg( WM_IMAGEDISPLAYED );
}


///
/// \brief Source combo box selection changed
///

void eBUSPlayer::OnCbnSelchangeSource( int aIndex )
{
    SendMsg( WM_UPDATESOURCE );
}


///
/// \brief Update sources combo box
///

void eBUSPlayer::OnUpdateSource( QWindowsEvent *aEvent )
{
    // Since we get here through the message queue, things may have happened since the message was posted
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    int lIndex = mSourceCombo->currentIndex();

    int64_t lNewSource = 0;
    QVariant lData = mSourceCombo->itemData( lIndex );
    if ( lData.canConvert<int64_t>() )
    {
        lNewSource = lData.value<int64_t>();
    }

    // Change source
    PvResult lResult = mPlayer->ChangeSource( lNewSource );
    if ( lResult.IsOK() )
    {
        // Clear the display
        mDisplay->Clear();

        EnableControls( true );

        // Update acquisition mode combo box, which may have changed on a new source
        SendMsg( WM_UPDATEACQUISITIONMODE );
    }
}


///
/// \brief Update sources
///

void eBUSPlayer::OnUpdateSources( QWindowsEvent *aEvent )
{
    assert( aEvent != NULL );
    if ( aEvent == NULL )
    {
        return;
    }

    int64_t lPreferredSource = static_cast<int64_t>( aEvent->GetLParam() );

    if ( !mPlayer->IsControlledTransmitter() )
    {
        mPlayer->ResetUpdatingSources();
        return;
    }

    ComboItemVector lSources;
    mPlayer->GetSources( lSources );

    mSourceCombo->clear();
    ComboItemVector::iterator lIt = lSources.begin();
    while ( lIt != lSources.end() )
    {
        mSourceCombo->addItem( lIt->mName.GetAscii(), (qlonglong)lIt->mValue );
        if ( lIt->mSelected )
        {
            mSourceCombo->setCurrentIndex( mSourceCombo->count() - 1 );
        }

        lIt++;
    }

    // If we have a preferred selection, attempt to find, select it
    if ( ( lPreferredSource >= 0 ) && ( mSourceCombo->count() > 0 ) )
    {
        for ( int i = 0; i < mSourceCombo->count(); i++ )
        {
            QVariant lData = mSourceCombo->itemData( i );
            if ( lData.canConvert<int64_t>() )
            {
                int64_t lValue = lData.value<int64_t>();
                if ( lValue == lPreferredSource )
                {
                    mSourceCombo->setCurrentIndex( i );
                    SendMsg( WM_UPDATESOURCE );
                }
            }
        }
    }

    // If no selection, select first available
    if ( ( mSourceCombo->count() > 0 ) && ( mSourceCombo->currentIndex() < 0 ) )
    {
        mSourceCombo->setCurrentIndex( 0 );
        SendMsg( WM_UPDATESOURCE );
    }

    mPlayer->ResetUpdatingSources();
}


///
/// \brief Acquisition modes collection changed
///

void eBUSPlayer::OnUpdateAcquisitionModes( QWindowsEvent *aEvent )
{
    if ( !mPlayer->IsControlledTransmitter() )
    {
        return;
    }

    // Get acquisition modes from model
    ComboItemVector lModes;
    bool lWritable = false;
    mPlayer->GetAcquisitionModes( lModes, lWritable );

    // Reset combo content
    mModeCombo->clear();

    // Add acquisition modes, select current
    ComboItemVector::const_iterator lIt = lModes.begin();
    while ( lIt != lModes.end() )
    {
        mModeCombo->addItem( lIt->mName.GetAscii(), (qlonglong)lIt->mValue );
        if ( lIt->mSelected )
        {
            mModeCombo->setCurrentIndex( mModeCombo->count() - 1 );
        }

        lIt++;
    }

    // Is it writable?
    mModeCombo->setEnabled( lWritable );
}


///
/// \brief Acquisition mode updated
///
/// wParam is a flag that when set to one resets the BOOL used to prevent
/// re-entry with the GenApi OnParameterUpdate.
///
/// When not called from GenApi OnParameterUpdate (Send or PostMessage) the
/// flag is left alone (not reset) in case there is a OnParameterUpdate message
/// already in the message queue that would depend on it being set.
///

void eBUSPlayer::OnUpdateAcquisitionMode( QWindowsEvent *aEvent )
{
    assert( aEvent != NULL );
    if ( aEvent == NULL )
    {
        return;
    }

    if ( !mPlayer->IsControlledTransmitter() )
    {
        // Not ready yet...
        if ( aEvent->GetWParam() == 1 )
        {
            mPlayer->ResetUpdatingAcquisitionMode();
        }

        return;
    }

    ComboItem lMode;
    bool lWritable = false;
    mPlayer->GetCurrentAcquisitionMode( lMode, lWritable );

    for ( int i = 0; i < mModeCombo->count(); i++ )
    {
        QVariant lData = mModeCombo->itemData( i );
        if ( lData.canConvert<int64_t>() )
        {
            int64_t lComboValue = lData.value<int64_t>();
            if ( lMode.mValue == lComboValue )
            {
                mModeCombo->setCurrentIndex( i );
                break;
            }
        }
    }

    mModeCombo->setEnabled( lWritable );

    if ( aEvent->GetWParam() == 1 )
    {
        mPlayer->ResetUpdatingAcquisitionMode();
    }
}


///
/// \brief Notification that the acquisition state has changed
///

void eBUSPlayer::OnAcquisitionStateChanged( QWindowsEvent *aEvent )
{
    // In case the event has been received after the object has been released
    if ( mPlayer->IsControlledTransmitter() )
    {
        EnableControls( mPlayer->IsDeviceConnected() );
    }
}

