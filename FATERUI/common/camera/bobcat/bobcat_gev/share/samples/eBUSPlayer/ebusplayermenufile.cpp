// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtCore/QSettings>


///
/// \brief File menu about to show. Update what needs to be updated.
///

void eBUSPlayer::OnFileMenuAboutToShow()
{
    mFileSaveAction->setEnabled( ( mPlayer->IsDeviceConnected() || mPlayer->IsStreamOpened() ) && !mPlayer->IsInRecovery() && !mPlayer->IsStreaming() );
    mFileSaveAsAction->setEnabled( ( mPlayer->IsDeviceConnected() || mPlayer->IsStreamOpened() ) && !mPlayer->IsInRecovery() && !mPlayer->IsStreaming() );
    mFileOpenAction->setEnabled( ( !mCommunicationWnd->GetQWidget()->isVisible() ) &&
            ( !mDeviceWnd->GetQWidget()->isVisible() ) &&
            ( !mStreamWnd->GetQWidget()->isVisible() ) && !mPlayer->IsInRecovery() && !mPlayer->IsStreaming() );
    mFileOpenRecentMenu->setEnabled( mFileOpenAction->isEnabled() );
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnFileOpenAction()
{
    if ( mPlayer->IsStreaming() )
    {
        return;
    }

    mFileName = QFileDialog::getOpenFileName(this, "Open eBUS SDK Project", NULL, "eBUS Player Configuration Files (*.pvcfg)");
    if ( !mFileName.isNull() )
    {
        OpenConfig( mFileName );
    }
}


///
/// \brief Menu action handler
///

void OpenRecent::OnFileOpenRecentAction()
{
    mEBUSPlayer->SetFileName( mFileName );
    mEBUSPlayer->OpenConfig( mFileName );
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnFileSaveAction()
{
    if ( mPlayer->IsStreaming() )
    {
        return;
    }

    if ( mFileName.isNull() )
    {
        OnFileSaveAsAction();
        return;
    }

    SaveConfig( mFileName, true );

    ReportMRU( mFileName );
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnFileSaveAsAction()
{
    if ( mPlayer->IsStreaming() )
    {
        return;
    }

    mFileName = QFileDialog::getSaveFileName(this, "Save eBUS SDK Project", NULL, "eBUS Player Configuration Files (*.pvcfg)");

    if ( !mFileName.isNull() )
    {
        if ( !mFileName.endsWith(".pvcfg") )
        {
            mFileName.append(".pvcfg");
        }

        OnFileSaveAction();
    }
}


///
/// \brief Menu action handler
///

void eBUSPlayer::OnFileExitAction()
{
    Disconnect();
    close();
}


///
/// \brief Reports the used of a most recent file
///

void eBUSPlayer::ReportMRU( const QString &aFileName )
{
    int curPos= 0;
    QString lName;
#ifdef WIN32
    QString lSlash = "\\";
#else
    QString lSlash = "/";
#endif
    lName = aFileName.section(lSlash, -1);

    setWindowTitle( lName + " - eBUS Player" );

    // Try to find the item in our current list
    std::list<QString>::iterator lIt;
    std::list<QString>::iterator lToRemove;

    lIt = mRecentList.begin();
    bool lRemove = false;
    while ( lIt != mRecentList.end() )
    {
        if ( *lIt == aFileName )
        {
            // If match, remove it, will be added as top latter
            lToRemove = lIt;
            lRemove = true;
        }
        lIt++;
    }

    if ( lRemove )
    {
        mRecentList.erase( lToRemove );
    }

    // Whether we removed already existing or not, insert at top
    mRecentList.push_front( aFileName );

    // trim the end (we don't want more than 10)
    if ( mRecentList.size() > 10 )
    {
        lIt = mRecentList.begin();
        std::list<QString>::iterator lLast;
        while ( lIt != mRecentList.end() )
        {
            lLast = lIt;
            lIt++;
        }
        // erase the back, not the front
        mRecentList.erase( lLast );
    }

    // Save current MRU
    SaveMRUToRegistry();
    UpdateMRUMenu();
}


///
/// \brief Save the most recently used files list
///

void eBUSPlayer::SaveMRUToRegistry()
{
    QSettings lSettings;
    std::list<QString>::iterator lIt;
    lIt = mRecentList.begin();
    uint32_t lCount = 0;
    while ( lIt != mRecentList.end() )
    {
        QString lKey = "MRU/" + QString::number(lCount);
        lSettings.setValue( lKey, *lIt);
        lIt++;
        lCount++;
    }
}


///
/// \biref Loads the most recently used file list
///

void eBUSPlayer::LoadMRUFromRegistry()
{
    QSettings lSettings;
    uint32_t lCount = 0;
    QString lValue;
    do
    {
        QString lKey = "MRU/" + QString::number( lCount );
        lValue = lSettings.value( lKey ).toString();
        if ( !lValue.isEmpty() )
        {
             mRecentList.push_back( lValue );
        }
        lCount++;

    } while ( !lValue.isEmpty() && lCount <= 10 );
}


///
/// \brief Updates the file menu with the most recently used file list
///

void eBUSPlayer::UpdateMRUMenu()
{
    mFileOpenRecentMenu->clear();
    std::list<QString>::iterator lIt;
    lIt = mRecentList.begin();

    unsigned int i = 0;
    while ( lIt != mRecentList.end() )
    {
        mOpenRecent[ i ].mFileOpenRecentAction = mFileOpenRecentMenu->addAction( *lIt );
        mOpenRecent[ i ].mFileName = *lIt;
        mOpenRecent[ i ].mEBUSPlayer = this;
        QObject::connect( mOpenRecent[i].mFileOpenRecentAction, SIGNAL( triggered() ), &mOpenRecent[i], SLOT( OnFileOpenRecentAction() ) );
        lIt++;
        i++;
    }
}


