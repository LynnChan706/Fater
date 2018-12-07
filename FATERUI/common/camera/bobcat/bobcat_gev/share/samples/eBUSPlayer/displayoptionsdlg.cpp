// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "displayoptionsdlg.h"

#include <DisplayThread.h>

#include <PvDisplayWnd.h>

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>

#include <assert.h>
#include <limits>


DisplayOptionsDlg::DisplayOptionsDlg( QWidget* aParent )
    : QDialog( aParent )
{
    CreateLayout();
}


DisplayOptionsDlg::~DisplayOptionsDlg()
{
}


void DisplayOptionsDlg::CreateLayout()
{
    mGLDisabled = new QRadioButton( "Disabled" );
    mGLEnabled = new QRadioButton( "Enabled (uses OpenGL)" );
    mGLFull = new QRadioButton( "Full (uses OpenGL and video memory)" );

    QObject::connect( mGLDisabled, SIGNAL( clicked() ), this, SLOT( OnRendererSelected() ) );
    QObject::connect( mGLEnabled, SIGNAL( clicked() ), this, SLOT( OnRendererSelected() ) );
    QObject::connect( mGLFull, SIGNAL( clicked() ), this, SLOT( OnRendererSelected() ) );

    mRenderer = new QLabel( "" );
    mRendererVersion = new QLabel( "" );
    mVSyncCheck = new QCheckBox( "Vertical synchronization" );

    QVBoxLayout *lOpenGLLayout = new QVBoxLayout;
    lOpenGLLayout->addWidget( mGLDisabled );
    lOpenGLLayout->addWidget( mGLEnabled );
    lOpenGLLayout->addWidget( mGLFull );
    lOpenGLLayout->addSpacing( 5 );
    lOpenGLLayout->addWidget( mRenderer );
    lOpenGLLayout->addWidget( mRendererVersion );
    lOpenGLLayout->addSpacing( 5 );
    lOpenGLLayout->addWidget( mVSyncCheck );
    QGroupBox *lOpenGL = new QGroupBox( "Hardware acceleration" );
    lOpenGL->setLayout( lOpenGLLayout );

    mFPSDisabled = new QRadioButton( "Display disabled" );
    mFPS10 = new QRadioButton( "10 frames per second" );
    mFPS30 = new QRadioButton( "30 frames per second" );
    mFPS60 = new QRadioButton( "60 frames per second" );

    QVBoxLayout *lFPSLayout = new QVBoxLayout;
    lFPSLayout->addWidget( mFPSDisabled );
    lFPSLayout->addWidget( mFPS10 );
    lFPSLayout->addWidget( mFPS30 );
    lFPSLayout->addWidget( mFPS60 );
    QGroupBox *lFPS = new QGroupBox( "Maximum display frame rate" );
    lFPS->setLayout( lFPSLayout );

    QVBoxLayout *lAdditionalOptionsLayout = new QVBoxLayout;
    QGroupBox *lAdditionalOptionsGroup = new QGroupBox( "Additional options" );
    mKeepPartialImages = new QCheckBox( "Keep partial images" );
    lAdditionalOptionsLayout->addWidget( mKeepPartialImages );
    mDisplayChunkData = new QCheckBox( "Display chunk data" );
    lAdditionalOptionsLayout->addWidget( mDisplayChunkData );
    lAdditionalOptionsGroup->setLayout( lAdditionalOptionsLayout );

    QHBoxLayout *lButtons = new QHBoxLayout;
    mOKButton = new QPushButton( tr( "OK" ) );
    mCancelButton = new QPushButton( tr( "Cancel" ) );
    lButtons->addStretch();
    lButtons->addWidget( mOKButton );
    lButtons->addWidget( mCancelButton );

    QObject::connect( mOKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    QObject::connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addWidget( lOpenGL );
    lMain->addWidget( lFPS );
    lMain->addWidget( lAdditionalOptionsGroup );
    lMain->addStretch();
    lMain->addLayout( lButtons );

    setLayout( lMain );
    setFixedSize( 400, 480 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;

    lFlags |= Qt::WindowStaysOnTopHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Display Options" ) );

    // Set the defaults appropriately
    mFPS30->setChecked( true );

    PvRendererMode lSupported = PvDisplayWnd::GetHighestRendererMode();

    // Hardware acceleration
    mGLDisabled->setEnabled( lSupported >= PvRendererModeDefault );
    mGLEnabled->setEnabled( lSupported >= PvRendererModeAccelerated );
    mGLFull->setEnabled( lSupported >= PvRendererModeHardwareAccelerated );
    mRenderer->setText( QString( "Renderer: " ) + PvDisplayWnd::GetRenderer().GetAscii() );
    mRendererVersion->setText( QString( "Version: " ) + PvDisplayWnd::GetRendererVersion().GetAscii() );

    mGLDisabled->setChecked( true );
    mVSyncCheck->setChecked( false ); // VSync not supported for Non-OpenGL
    mVSyncCheck->setDisabled( true );
}


void DisplayOptionsDlg::OnRendererSelected()
{
    if ( mGLEnabled->isChecked() || mGLFull->isChecked() )
    {
        mVSyncCheck->setDisabled( false );
    }
    else
    {
        assert( mGLDisabled->isChecked() );
        mVSyncCheck->setChecked( false ); // VSync not supported for Non-OpenGL
        mVSyncCheck->setDisabled( true );
    }
}


void DisplayOptionsDlg::Init( DisplayThread *aDisplayThread )
{
    // Frames per seconds
    switch ( aDisplayThread->GetTargetFPS() )
    {
    case 10:
        mFPS10->setChecked( true );
        break;

    default:
        assert( 0 );
        // Fallthrough

    case 30:
        mFPS30->setChecked( true );
        break;

    case 60:
        mFPS60->setChecked( true );
        break;

    case 0:
        mFPSDisabled->setChecked( true );
        break;
    }

    PvRendererMode lSupported = PvDisplayWnd::GetHighestRendererMode();

    // Hardware acceleration
    mGLDisabled->setEnabled( lSupported >= PvRendererModeDefault );
    mGLEnabled->setEnabled( lSupported >= PvRendererModeAccelerated );
    mGLFull->setEnabled( lSupported >= PvRendererModeHardwareAccelerated );
    mRenderer->setText( QString( "Renderer: " ) + PvDisplayWnd::GetRenderer().GetAscii() );
    mRendererVersion->setText( QString( "Version: " ) + PvDisplayWnd::GetRendererVersion().GetAscii() );
    switch ( aDisplayThread->GetRendererMode() )
    {
    default:
        assert( 0 );
        // Fallthrough

    case PvRendererModeDefault:
        mGLDisabled->setChecked( true );
        mVSyncCheck->setChecked( false ); // VSync not supported for Non-OpenGL
        mVSyncCheck->setDisabled( true );
        break;

    case PvRendererModeAccelerated:
        mGLEnabled->setChecked( true );
        mVSyncCheck->setDisabled( false );
        mVSyncCheck->setChecked( aDisplayThread->GetVSyncEnabled() );
        break;

    case PvRendererModeHardwareAccelerated:
        mGLFull->setChecked( true );
        mVSyncCheck->setDisabled( false );
        mVSyncCheck->setChecked( aDisplayThread->GetVSyncEnabled() );
        break;
    }

    mKeepPartialImages->setChecked( aDisplayThread->GetKeepPartialImagesEnabled() );
    mDisplayChunkData->setChecked( aDisplayThread->GetDisplayChunkDataEnabled() );
}


void DisplayOptionsDlg::Apply( DisplayThread *aDisplayThread )
{
    // Frames per seconds
    if ( mFPSDisabled->isChecked() )
    {
        aDisplayThread->SetTargetFPS( 0 ); // Disabled
    }
    else if ( mFPS10->isChecked() )
    {
        aDisplayThread->SetTargetFPS( 10 );
    }
    else if ( mFPS60->isChecked() )
    {
        aDisplayThread->SetTargetFPS( 60 );
    }
    else
    {
        assert( mFPS30->isChecked() );
        aDisplayThread->SetTargetFPS( 30 );
    }

    // Hardware acceleration
    if ( mGLEnabled->isChecked() )
    {
        aDisplayThread->SetRendererMode( PvRendererModeAccelerated );
    }
    else if ( mGLFull->isChecked() )
    {
        aDisplayThread->SetRendererMode( PvRendererModeHardwareAccelerated );
    }
    else
    {
        assert( mGLDisabled->isChecked() );
        aDisplayThread->SetRendererMode( PvRendererModeDefault );
    }

    aDisplayThread->SetVSyncEnabled( mVSyncCheck->checkState() == Qt::Checked );
    aDisplayThread->SetKeepPartialImagesEnabled( mKeepPartialImages->checkState() == Qt::Checked );
    aDisplayThread->SetDisplayChunkDataEnabled( mDisplayChunkData->checkState() == Qt::Checked );
}


