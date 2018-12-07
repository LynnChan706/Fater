// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"

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
#include <QtCore/QSettings>


#define LEFT_WIDTH ( 400 )


///
/// \brief Create dialog layout
///

void eBUSPlayer::CreateLayout()
{
    QGroupBox *lConnectionBox = CreateConnectGroup();
    QGroupBox *lAcqBox = CreateAcquisition();
    QGroupBox *lParametersBox = CreateParameters();
    QGroupBox *lDisplayBox = CreateDisplay();

    QVBoxLayout *lLayoutLeft = new QVBoxLayout();
    lLayoutLeft->addWidget( lConnectionBox, Qt::AlignTop );
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
    setWindowTitle( tr( "eBUS Player" ) );
    setWindowIcon( QIcon( QPixmap( ":eBUSPlayer/res/eBUSPlayer.ico" ) ) );

    CreateMenu();

    LoadMRUFromRegistry();
    UpdateMRUMenu();

    EnableInterface();

    mTimer = new QTimer( this );
    mTimer->setInterval( 250 );
    connect( mTimer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
}


///
/// \brief Create connect group box
///

QGroupBox *eBUSPlayer::CreateConnectGroup()
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

    QLabel *lGUIDLabel = new QLabel( tr( "GUID" ) );
    mGUIDLine = new QLineEdit;
    mGUIDLine->setReadOnly( true );
    mGUIDLine->setEnabled( false );

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

    int lLine = 0;
    QGridLayout *lGridLayout = new QGridLayout;
    lGridLayout->addWidget( lIpAddressLabel, lLine, 0 );
    lGridLayout->addWidget( mIpAddressLine, lLine, 1, 1, 5 ); lLine++;
    lGridLayout->addWidget( lMacAddressLabel, lLine, 0 );
    lGridLayout->addWidget( mMacAddressLine, lLine, 1, 1, 5 ); lLine++;
    lGridLayout->addWidget( lGUIDLabel, lLine, 0 );
    lGridLayout->addWidget( mGUIDLine, lLine, 1, 1, 5 ); lLine++;
    lGridLayout->addWidget( lManufacturerLabel, lLine, 0 );
    lGridLayout->addWidget( mManufacturerLine, lLine, 1, 1, 5 ); lLine++;
    lGridLayout->addWidget( lModelLabel, lLine, 0 );
    lGridLayout->addWidget( mModelLine, lLine, 1, 1, 5 ); lLine++;
    lGridLayout->addWidget( lNameLabel, lLine, 0 );
    lGridLayout->addWidget( mNameLine, lLine, 1, 1, 5 ); lLine++;

    QVBoxLayout *lBoxLayout = new QVBoxLayout;
    lBoxLayout->addLayout( lButtonsLayout );
    lBoxLayout->addLayout( lGridLayout );
    lBoxLayout->addStretch();

    QGroupBox *lConnectionBox = new QGroupBox( tr( "Connection" ) );
    lConnectionBox->setLayout( lBoxLayout );
    lConnectionBox->setMinimumWidth( LEFT_WIDTH );
    lConnectionBox->setMaximumWidth( LEFT_WIDTH );
    lConnectionBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lConnectionBox;
}


///
/// \brief Create acquisition group box
///

QGroupBox *eBUSPlayer::CreateAcquisition()
{
    QLabel *lSourceLabel = new QLabel( tr( "Source" ) );
    mSourceCombo = new QComboBox;
    mSourceCombo->setEnabled( false );

    QObject::connect( mSourceCombo, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeSource( int ) ) );

    QLabel *lModeLabel = new QLabel( tr( "Mode" ) );
    mModeCombo = new QComboBox;
    mModeCombo->setEnabled( false );

    QObject::connect( mModeCombo, SIGNAL( activated( int ) ), this, SLOT( OnCbnSelchangeMode( int ) ) );

    QGridLayout *lAcqGridLayout = new QGridLayout;
    lAcqGridLayout->addWidget( lSourceLabel, 0, 0 );
    lAcqGridLayout->addWidget( mSourceCombo, 0, 1, 1, 2 );
    lAcqGridLayout->addWidget( lModeLabel, 1, 0 );
    lAcqGridLayout->addWidget( mModeCombo, 1, 1, 1, 2 );

    mStartButton = new QPushButton( tr( "Play" ) );
    mStartButton->setMinimumHeight( 45 );
    mStartButton->setEnabled( false );

    QPixmap lPlayPixmap(":eBUSPlayer/res/play.bmp");
    lPlayPixmap.setMask(lPlayPixmap.createMaskFromColor(QColor(0xFF, 0xFF, 0xFF)));
    mStartButton->setIcon(QIcon(lPlayPixmap));

    mStopButton = new QPushButton( tr( "Stop" ) );
    mStopButton->setMinimumHeight( 45 );
    mStopButton->setEnabled( false );

    QPixmap lStopPixmap(":eBUSPlayer/res/stop.bmp");
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
    lAcqBox->setMinimumWidth( LEFT_WIDTH );
    lAcqBox->setMaximumWidth( LEFT_WIDTH );
    lAcqBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

    return lAcqBox;
}


///
/// \brief Create parameters group box
///

QGroupBox *eBUSPlayer::CreateParameters()
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
    parametersBox->setMinimumWidth( LEFT_WIDTH );
    parametersBox->setMaximumWidth( LEFT_WIDTH );

    return parametersBox;
}


///
/// \brief Create display
///

QGroupBox *eBUSPlayer::CreateDisplay()
{
    mStatusLine = new QTextEdit;
    mStatusLine->setReadOnly( true );
    mStatusLine->setEnabled( false );
    mStatusLine->setReadOnly( true );
    mStatusLine->setBackgroundRole( QPalette::Base );
    mStatusLine->setWordWrapMode( QTextOption::NoWrap );
    mStatusLine->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mStatusLine->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    QFontMetrics lFM( mStatusLine->font() );
    mStatusLine->setMaximumHeight( lFM.height() * 3 + lFM.lineSpacing() );

    QVBoxLayout *displayLayout = new QVBoxLayout;
    displayLayout->addWidget( mDisplay->GetQWidget() );
    displayLayout->addWidget( mStatusLine, Qt::AlignBottom );

    QGroupBox *lDisplayBox = new QGroupBox( tr( "Display" ) );
    lDisplayBox->setLayout( displayLayout );
    lDisplayBox->setMinimumWidth( 600 );

    return lDisplayBox;
}


///
/// \brief Create menu
///

void eBUSPlayer::CreateMenu()
{
    //
    // File
    //

    mFileMenu = menuBar()->addMenu( tr( "&File" ) );
    QObject::connect( mFileMenu, SIGNAL( aboutToShow() ), this, SLOT( OnFileMenuAboutToShow() ) );

    mFileOpenAction = mFileMenu->addAction( tr( "&Open..." ) );
    mFileOpenRecentMenu = mFileMenu->addMenu( tr( "Open Recent" ) );
    mFileSaveAction = mFileMenu->addAction( tr( "&Save" ) );
    mFileSaveAsAction = mFileMenu->addAction( tr( "Save &As..." ) );
    mFileMenu->addSeparator();
    mFileExitAction = mFileMenu->addAction( tr( "E&xit" ) );

    QObject::connect( mFileOpenAction, SIGNAL( triggered() ), this, SLOT( OnFileOpenAction() ) );
    QObject::connect( mFileSaveAction, SIGNAL( triggered() ), this, SLOT( OnFileSaveAction() ) );
    QObject::connect( mFileSaveAsAction, SIGNAL( triggered() ), this, SLOT( OnFileSaveAsAction() ) );
    QObject::connect( mFileExitAction, SIGNAL( triggered() ), this, SLOT( OnFileExitAction() ) );

    QMenu *lDeinterlacingMenu = new QMenu( tr( "Deinterlacing" ) );

    QActionGroup *lDeinterlaceGroup = new QActionGroup( lDeinterlacingMenu );
    mToolsDeinterlacingDisabled = lDeinterlaceGroup->addAction( tr( "Off" ) );
    mToolsDeinterlacingDisabled->setCheckable( true );
    mToolsDeinterlacingWeavingHalf = lDeinterlaceGroup->addAction( tr( "Weave Half Temporal Resolution" ) );
    mToolsDeinterlacingWeavingHalf->setCheckable( true );
    mToolsDeinterlacingWeavingFull = lDeinterlaceGroup->addAction( tr( "Weave Full Temporal Resolution" ) );
    mToolsDeinterlacingWeavingFull->setCheckable( true );
    mToolsDeinterlacingBlending = lDeinterlaceGroup->addAction( tr( "Blending" ) );
    mToolsDeinterlacingBlending->setCheckable( true );
    mToolsDeinterlacingLineDoubling = lDeinterlaceGroup->addAction( tr( "Line Doubling" ) );
    mToolsDeinterlacingLineDoubling->setCheckable( true );
    lDeinterlacingMenu->addActions( lDeinterlaceGroup->actions() );

    //
    // Tools
    //

    QMenu *mToolsMenu = menuBar()->addMenu( tr( "&Tools" ) );
    QObject::connect( mToolsMenu, SIGNAL( aboutToShow() ), this, SLOT( OnToolsMenuAboutToShow() ) );

    mToolsSetupAction = mToolsMenu->addAction( tr( "&Setup..." ) );
    mToolsMenu->addSeparator();
    mToolsGEVCommunicationParametersAction = mToolsMenu->addAction( tr( "Default &GigE Vision Communication Parameters..." ) );
    mToolsU3VCommunicationParametersAction = mToolsMenu->addAction( tr( "Default &USB3 Vision Communication Parameters..." ) );
    mToolsMenu->addSeparator();
    mToolsEventMonitorAction = mToolsMenu->addAction( tr( "&Event Monitor" ) );
    mToolsSerialAction = mToolsMenu->addAction( tr( "Device Serial &Communication..." ) );
    mToolsActionCommandAction = mToolsMenu->addAction( tr( "Action Command..." ) );
    mToolsMenu->addSeparator();
    mToolsFilteringAction = mToolsMenu->addAction( tr( "Image &Filtering..." ) );
    mToolsMenu->addMenu( lDeinterlacingMenu );
    mToolsMenu->addSeparator();
    mToolsDisplayOptionsAction = mToolsMenu->addAction( tr( "Display Options..." ) );
    mToolsBufferOptionsAction = mToolsMenu->addAction( tr( "Buffer Options..." ) );
    mToolsResetStreamingStatisticsAction = mToolsMenu->addAction( tr( "Reset Streaming Statistics" ) );
    mToolsMenu->addSeparator();
    mToolsSaveImagesAction = mToolsMenu->addAction( tr( "Save Images..." ) );
    mToolsSaveCurrentImageAction = mToolsMenu->addAction( tr( "Save Current Image" ) );
    mToolsMenu->addSeparator();
    mToolsSavePreferencesAction = mToolsMenu->addAction( tr( "Save Preferences" ) );
    mToolsRestoreDefaultAction = mToolsMenu->addAction( tr( "Restore Default Preferences" ) );
    mToolsMenu->addSeparator();
    mToolsFileTransferAction = mToolsMenu->addAction( tr( "File Transfer..." ) );
    mToolsSaveXMLAction = mToolsMenu->addAction( tr( "Save XML..." ) );

    QObject::connect( mToolsSetupAction, SIGNAL( triggered() ), this, SLOT( OnToolsSetupAction() ) );
    QObject::connect( mToolsGEVCommunicationParametersAction, SIGNAL( triggered() ), this, SLOT( OnToolsGEVCommunicationParametersAction() ) );
    QObject::connect( mToolsU3VCommunicationParametersAction, SIGNAL( triggered() ), this, SLOT( OnToolsU3VCommunicationParametersAction() ) );
    QObject::connect( mToolsEventMonitorAction, SIGNAL( triggered() ), this, SLOT( OnToolsEventMonitorAction() ) );
    QObject::connect( mToolsSerialAction, SIGNAL( triggered() ), this, SLOT( OnToolsSerialAction() ) );
    QObject::connect( mToolsActionCommandAction, SIGNAL( triggered() ), this, SLOT( OnToolsActionCommandAction() ) );
    QObject::connect( mToolsFilteringAction, SIGNAL( triggered() ), this, SLOT( OnToolsFilteringAction() ) );
    QObject::connect( mToolsDisplayOptionsAction, SIGNAL( triggered() ), this, SLOT( OnToolsDisplayOptionsAction() ) );
    QObject::connect( mToolsBufferOptionsAction, SIGNAL( triggered() ), this, SLOT( OnToolsBufferOptionsAction() ) );
    QObject::connect( mToolsResetStreamingStatisticsAction, SIGNAL( triggered() ), this, SLOT( OnToolsResetStreamingStatisticsAction() ) );
    QObject::connect( mToolsSaveImagesAction, SIGNAL( triggered() ), this, SLOT( OnToolsSaveImagesAction() ) );
    QObject::connect( mToolsSaveCurrentImageAction, SIGNAL( triggered() ), this, SLOT( OnToolsSaveCurrentImageAction() ) );
    QObject::connect( mToolsSavePreferencesAction, SIGNAL( triggered() ), this, SLOT( OnToolsSavepreferences() ) );
    QObject::connect( mToolsRestoreDefaultAction, SIGNAL( triggered() ), this, SLOT( OnToolsRestoredefaultpreferences() ) );
    QObject::connect( mToolsSaveXMLAction, SIGNAL( triggered() ), this, SLOT( OnToolsSaveXML() ) );
    QObject::connect( mToolsFileTransferAction, SIGNAL( triggered() ), this, SLOT( OnToolsFileTransferAction() ) );
    QObject::connect( mToolsDeinterlacingDisabled, SIGNAL( triggered() ), this, SLOT( OnToolsDeinterlacingDisabledAction() ) );
    QObject::connect( mToolsDeinterlacingWeavingHalf, SIGNAL( triggered() ), this, SLOT( OnToolsDeinterlacingWeavingHalfAction() ) );
    QObject::connect( mToolsDeinterlacingWeavingFull, SIGNAL( triggered() ), this, SLOT( OnToolsDeinterlacingWeavingFullAction() ) );
    QObject::connect( mToolsDeinterlacingBlending, SIGNAL( triggered() ), this, SLOT( OnToolsDeinterlacingBlendingAction() ) );
    QObject::connect( mToolsDeinterlacingLineDoubling, SIGNAL( triggered() ), this, SLOT( OnToolsDeinterlacingLineDoublingAction() ) );

    //
    // Help
    //

    QMenu *mHelpMenu = menuBar()->addMenu( tr( "&Help" ) );
    QObject::connect( mHelpMenu, SIGNAL( aboutToShow() ), this, SLOT( OnHelpMenuAboutToShow() ) );

    mHelpAboutAction = mHelpMenu->addAction( tr( "About eBUS Player..." ) );
    QObject::connect( mHelpAboutAction, SIGNAL( triggered() ), this, SLOT( OnHelpAboutAction() ) );

    //
    // Register interface (only accessible by shortcut)
    //
    QShortcut* lRegisterInterfaceAction = new QShortcut( QKeySequence(Qt::ControlModifier + Qt::AltModifier + Qt::Key_R), this );
    QObject::connect( lRegisterInterfaceAction, SIGNAL( activated() ), this, SLOT( OnRegisterInterface() ) );
}


