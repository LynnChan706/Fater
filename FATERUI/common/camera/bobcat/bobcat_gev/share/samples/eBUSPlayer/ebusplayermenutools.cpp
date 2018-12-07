// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "actioncommanddlg.h"
#include "eventmonitordlg.h"
#include "filteringdlg.h"
#include "imagesavedlg.h"
#include "setupdlg.h"
#include "displayoptionsdlg.h"
#include "bufferoptionsdlg.h"
#include "filetransferdlg.h"

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>

#include <PvGenFile.h>
#include <PvRegisterInterfaceWnd.h>

#include "assert.h"


///
/// \brief Tools menu about to show. Update what needs to be updated.
///

void eBUSPlayer::OnToolsMenuAboutToShow()
{
    mToolsGEVCommunicationParametersAction->setEnabled( mPlayer->IsSetupRoleController() && !mPlayer->IsDeviceConnected() );
    mToolsU3VCommunicationParametersAction->setEnabled( mPlayer->IsSetupRoleController() && !mPlayer->IsDeviceConnected() );

    mToolsEventMonitorAction->setEnabled( ( mEventMonitorDlg != NULL ) && !mPlayer->IsInRecovery() );
    mToolsEventMonitorAction->setChecked( ( ( mEventMonitorDlg != NULL ) && mEventMonitorDlg->isVisible() ) );

    mToolsDisplayOptionsAction->setEnabled( mPlayer->IsStreamStarted() );

    mToolsSaveImagesAction->setEnabled( mPlayer->IsStreamStarted() );
    mToolsSaveCurrentImageAction->setEnabled( mPlayer->IsStreamStarted() );

    mToolsFilteringAction->setEnabled( mPlayer->IsStreamOpened() );
    mToolsFilteringAction->setChecked( mFilteringDlg->isVisible() );

    mToolsSerialAction->setEnabled( mPlayer->IsSerialSupported() && !mPlayer->IsInRecovery() );
    mToolsSerialAction->setChecked( mSerialTerminalWnd.GetQWidget()->isVisible() );

    mToolsSaveXMLAction->setEnabled( mPlayer->IsDeviceConnected() && !mPlayer->IsInRecovery() );

    mToolsActionCommandAction->setChecked( ( mActionCommandDlg != NULL ) && mActionCommandDlg->isVisible() );

    mToolsDeinterlacingDisabled->setEnabled( mPlayer->IsStreamStarted() );
    mToolsDeinterlacingDisabled->setChecked( false );
    mToolsDeinterlacingWeavingHalf->setEnabled( mPlayer->IsStreamStarted() );
    mToolsDeinterlacingWeavingHalf->setChecked( false );
    mToolsDeinterlacingWeavingFull->setEnabled( mPlayer->IsStreamStarted() );
    mToolsDeinterlacingWeavingFull->setChecked( false );
    mToolsDeinterlacingBlending->setEnabled( mPlayer->IsStreamStarted() );
    mToolsDeinterlacingBlending->setChecked( false );
    mToolsDeinterlacingLineDoubling->setEnabled( mPlayer->IsStreamStarted() );
    mToolsDeinterlacingLineDoubling->setChecked( false );

    switch ( mPlayer->GetDisplayThread()->GetDeinterlacing() )
    {
    case PvDeinterlacingDisabled:
        mToolsDeinterlacingDisabled->setChecked( true );
        break;

    case PvDeinterlacingWeavingHalf:
        mToolsDeinterlacingWeavingHalf->setChecked( true );
        break;

    case PvDeinterlacingWeavingFull:
        mToolsDeinterlacingWeavingFull->setChecked( true );
        break;

    case PvDeinterlacingBlending:
        mToolsDeinterlacingBlending->setChecked( true );
        break;

    case PvDeinterlacingLineDoubling:
        mToolsDeinterlacingLineDoubling->setChecked( true );
        break;

    default:
        assert( 0 );
        break;
    }

    mToolsSavePreferencesAction->setEnabled( !mPlayer->IsInRecovery() );
    mToolsRestoreDefaultAction->setEnabled( !mPlayer->IsInRecovery() );

    mToolsFileTransferAction->setEnabled( mPlayer->IsDeviceConnected() && PvGenFile::IsSupported( mPlayer->GetDeviceParameters() ) );
}


///
/// \brief Register interface menu handler
///

void eBUSPlayer::OnRegisterInterface()
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    CloseGenWindow( mCommunicationWnd );
    CloseGenWindow( mDeviceWnd );
    CloseGenWindow( mStreamWnd );

    PvDeviceGEV *lDevice = dynamic_cast<PvDeviceGEV *>( mPlayer->GetPvDevice() );
    if ( lDevice == NULL )
    {
        return;
    }

    PvRegisterInterfaceWnd lInterface;
    lInterface.SetDevice( lDevice );
    lInterface.ShowModal( this );
}


///
/// \brief Deinterlacing disabled menu action handler
///

void eBUSPlayer::OnToolsDeinterlacingDisabledAction()
{
    mPlayer->GetDisplayThread()->SetDeinterlacing( PvDeinterlacingDisabled );
}


///
/// \brief Deinterlacing weaving half menu action handler
///

void eBUSPlayer::OnToolsDeinterlacingWeavingHalfAction()
{
    mPlayer->GetDisplayThread()->SetDeinterlacing( PvDeinterlacingWeavingHalf );
}


///
/// \brief Deinterlacing weaving full menu action handler
///

void eBUSPlayer::OnToolsDeinterlacingWeavingFullAction()
{
    mPlayer->GetDisplayThread()->SetDeinterlacing( PvDeinterlacingWeavingFull );
}


///
/// \brief Deinterlacing blending menu action handler
///

void eBUSPlayer::OnToolsDeinterlacingBlendingAction()
{
    mPlayer->GetDisplayThread()->SetDeinterlacing( PvDeinterlacingBlending );
}


///
/// \brief Deinterlacing line doubling menu action handler
///

void eBUSPlayer::OnToolsDeinterlacingLineDoublingAction()
{
    mPlayer->GetDisplayThread()->SetDeinterlacing( PvDeinterlacingLineDoubling );
}


///
/// \brief GEV communication parameters action handler
///

void eBUSPlayer::OnToolsGEVCommunicationParametersAction()
{
    mDefaultCommGEVParametersWnd.SetGenParameterArray( mPlayer->GetDefaultGEVCommunicationParameters() );
    mDefaultCommGEVParametersWnd.SetTitle( "Default GigE Vision Communication Parameters" );

    mDefaultCommGEVParametersWnd.ShowModal();
}


///
/// \brief GEV communication parameters action handler
///

void eBUSPlayer::OnToolsU3VCommunicationParametersAction()
{
    mDefaultCommU3VParametersWnd.SetGenParameterArray( mPlayer->GetDefaultU3VCommunicationParameters() );
    mDefaultCommU3VParametersWnd.SetTitle( "Default USB3 Vision Communication Parameters" );

    mDefaultCommU3VParametersWnd.ShowModal();
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsSaveImagesAction()
{
    if ( !mPlayer->IsStreamStarted() )
    {
        return;
    }

    ImageSaveDlg lDlg( mPlayer->GetImageSaving(), this );
    lDlg.exec();
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsSaveCurrentImageAction()
{
    assert( mPlayer->IsStreamStarted() );
    if ( !mPlayer->IsStreamStarted() )
    {
        return;
    }

    mPlayer->SaveCurrentImage();
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsSerialAction()
{
    if ( !mPlayer->IsSerialSupported() )
    {
        return;
    }

    if ( mSerialTerminalWnd.GetQWidget()->isVisible() )
    {
        mSerialTerminalWnd.Close();
    }
    else
    {
        mSerialTerminalWnd.SetDevice( mPlayer->GetPvDeviceAdapter() );
        mSerialTerminalWnd.ShowModeless( this );
    }
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsActionCommandAction()
{
    if ( mActionCommandDlg == NULL )
    {
        mActionCommandDlg = new ActionCommandDlg( this );
    }

    if ( mActionCommandDlg->isVisible() )
    {
        mActionCommandDlg->close();

        delete mActionCommandDlg;
        mActionCommandDlg = NULL;
        return;
    }

    mActionCommandDlg->show();
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsEventMonitorAction()
{
    assert( mEventMonitorDlg != NULL );
    if ( mEventMonitorDlg == NULL )
    {
        return;
    }

    if ( mEventMonitorDlg->isVisible() )
    {
        mEventMonitorDlg->close();
    }
    else
    {
        mEventMonitorDlg->show();
    }
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsSetupAction()
{
    Setup *lSetup = mPlayer->GetSetup();
    lSetup->SetEnabled( !mPlayer->IsDeviceConnected() && !mPlayer->IsStreamOpened() );

    SetupDlg lDlg( lSetup, this );
    lDlg.exec();

    EnableInterface();
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnToolsFilteringAction()
{
    if ( mFilteringDlg->isVisible() )
    {
        // If already open, just toggle to closed...
        mFilteringDlg->close();
        return;
    }

    mFilteringDlg->show();
}


///
/// \brief Display options menu handler
///

void eBUSPlayer::OnToolsDisplayOptionsAction()
{
    if ( !mPlayer->IsStreamStarted() )
    {
        return;
    }

    DisplayOptionsDlg lDlg( this );
    lDlg.Init( mPlayer->GetDisplayThread() );
    if ( lDlg.exec() == QDialog::Accepted )
    {
        lDlg.Apply( mPlayer->GetDisplayThread() );
    }
}


///
/// \brief Display buffer options
///

void eBUSPlayer::OnToolsBufferOptionsAction()
{
    BufferOptionsDlg lDlg( mPlayer, this );
    lDlg.exec();
}


///
/// \brief Resets the streaming statistics
///

void eBUSPlayer::OnToolsResetStreamingStatisticsAction()
{
    mPlayer->ResetStreamingStatistics();
}


///
/// \brief Save preferences action handler
///

void eBUSPlayer::OnToolsSavepreferences()
{
    SaveConfig( GetStickyPath(), false );
}


///
/// \brief Restore default preferences action handler
///

void eBUSPlayer::OnToolsRestoredefaultpreferences()
{
    // load the default file
    OpenConfig( GetDefaultPath() );

    // save the default settings so they are persisted next time
    SaveConfig( GetStickyPath(), false );
}


///
/// \brief Save device GenICam XML action handler
///

void eBUSPlayer::OnToolsSaveXML()
{
    if ( !mPlayer->IsDeviceConnected() )
    {
        return;
    }

    QString lFilename = mPlayer->GetDeviceXMLDefaultName().GetAscii();
    if ( lFilename.length() <= 0 )
    {
        lFilename = "Default.xml";
    }

    QString lFilter;
    if ( lFilename.toLower().endsWith(".zip") )
    {
    	lFilter = "Zip Files (*.zip)";
    }
    else
    {
    	lFilter = "Zip Files (*.xml)";
    }

    QString lFileName = QFileDialog::getSaveFileName( this, tr( "Save Device XML" ), lFilename, lFilter );
    if ( !lFileName.isNull() )
    {
        mPlayer->SaveDeviceXML( lFileName.toAscii().data() );
    }
}


///
/// \brief File transfer action handler
///

void eBUSPlayer::OnToolsFileTransferAction()
{
    FileTransferDlg lDlg( mPlayer->GetPvDevice(), this );
    lDlg.exec();
}



