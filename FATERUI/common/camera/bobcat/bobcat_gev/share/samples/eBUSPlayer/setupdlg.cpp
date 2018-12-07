// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "setupdlg.h"

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>

#include <assert.h>
#include <cstdio>


#define LAYOUT_IDENT ( 20 )


///
/// \brief Constructor
///

SetupDlg::SetupDlg( ::Setup *aSetup, QWidget* aParent )
    : QDialog( aParent )
    , mSetup( aSetup )
{
    CreateLayout();
}


///
/// \brief Destructor
///

SetupDlg::~SetupDlg()
{
}


///
/// \brief Creates the form's layout
///

void
SetupDlg::CreateLayout()
{
    setWindowTitle( tr( "Setup" ) );
    mOKButton = new QPushButton( tr( "OK" ) );
    mCancelButton = new QPushButton( tr( "Cancel" ) );

    QObject::connect( mOKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    QObject::connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QGroupBox *lRole = CreateRole();
    QGroupBox *lDefaultChannel = CreateDefaultChannel();
    QGroupBox *lDestination = CreateDestination();

    QVBoxLayout *lLeft = new QVBoxLayout;
    lLeft->addWidget( lRole );
    lLeft->addWidget( lDefaultChannel );
    lLeft->addStretch();

    QHBoxLayout *lGroups = new QHBoxLayout;
    lGroups->addLayout( lLeft );
    lGroups->addWidget( lDestination );

    QHBoxLayout *lButtons = new QHBoxLayout;
    lButtons->addStretch();
    lButtons->addWidget( mOKButton );
    lButtons->addWidget( mCancelButton );

    QVBoxLayout *lLayout = new QVBoxLayout;
    lLayout->addLayout( lGroups );
    lLayout->addStretch();
    lLayout->addLayout( lButtons );

    setLayout( lLayout );
    setFixedSize( 530, 420 );
    
    Qt::WindowFlags lFlags = windowFlags();
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    setWindowFlags( lFlags );
}


///
/// \brief Create role group box
///

QGroupBox *
SetupDlg::CreateRole()
{
    QGroupBox *lBox = new QGroupBox( tr( "eBUS Player Role" ) );

    mCtrlDataRadio = new QRadioButton( tr( "Controller and data receiver" ) );
    mCtrlRadio = new QRadioButton( tr( "Controller" ) );
    mDataRadio = new QRadioButton( tr( "Data receiver" ) );

    QObject::connect( mCtrlDataRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );
    QObject::connect( mCtrlRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );
    QObject::connect( mDataRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

    QVBoxLayout *lLayout = new QVBoxLayout;
    lLayout->addWidget( mCtrlDataRadio );
    lLayout->addWidget( mCtrlRadio );
    lLayout->addWidget( mDataRadio );
    lLayout->addStretch();

    lBox->setLayout( lLayout );

    return lBox;
}



///
/// \brief Create default channel group
///

QGroupBox *
SetupDlg::CreateDefaultChannel()
{
    QGroupBox *lBox = new QGroupBox( tr( "Stream Channel" ) );

    mDefaultChannelLabel = new QLabel( tr( "Default channel" ) );
    mDefaultChannelEdit = new QLineEdit( tr( "0" ) );

    QHBoxLayout *lDefaultChannelLayout = new QHBoxLayout;
    lDefaultChannelLayout->addWidget( mDefaultChannelLabel );
    lDefaultChannelLayout->addStretch();
    lDefaultChannelLayout->addWidget( mDefaultChannelEdit );

    QVBoxLayout *lVLayout = new QVBoxLayout;
    lVLayout->addLayout( lDefaultChannelLayout );
    lVLayout->addStretch();

    lBox->setLayout( lVLayout );

    return lBox;
}


///
/// \brief Create destination group box
///

QGroupBox *
SetupDlg::CreateDestination()
{
    QGroupBox *lBox = new QGroupBox( tr( "GigE Vision Stream Destination" ) );

    mUnicastAutoRadio = new QRadioButton( tr( "Unicast, automatic" ) );
    mUnicastSpecificRadio = new QRadioButton( tr( "Unicast, specific local port" ) );
    mUnicastOtherRadio = new QRadioButton( tr( "Unicast, other destination" ) );
    mUnicastSpecificPortLabel = new QLabel( tr( "Port" ) );
    mUnicastSpecificPortEdit = new QLineEdit( tr( "0" ) );
    mUnicastIPLabel = new QLabel( tr( "IP address" ) );
    mUnicastIPCtrl = new QLineEdit( tr( "0.0.0.0" ) );
    mUnicastPortLabel = new QLabel( tr( "Port" ) );
    mUnicastPortEdit = new QLineEdit( tr( "0" ) );
    mMulticastRadio = new QRadioButton( tr( "Multicast" ) );
    mMulticastIPLabel = new QLabel( tr( "IP address " ) );
    mMulticastIPCtrl = new QLineEdit( tr( "0.0.0.0" ) );
    mMulticastPortLabel = new QLabel( tr( "Port" ) );
    mMulticastPortEdit = new QLineEdit( tr( "0" ) );

    QObject::connect( mUnicastAutoRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );
    QObject::connect( mUnicastSpecificRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );
    QObject::connect( mUnicastOtherRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );
    QObject::connect( mMulticastRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

    QHBoxLayout *lUnicastSpecificPort = new QHBoxLayout;
    lUnicastSpecificPort->addSpacing( LAYOUT_IDENT );
    lUnicastSpecificPort->addWidget( mUnicastSpecificPortLabel );
    lUnicastSpecificPort->addStretch();
    lUnicastSpecificPort->addWidget( mUnicastSpecificPortEdit );

	QHBoxLayout *lUnicastIP = new QHBoxLayout;
    lUnicastIP->addSpacing( LAYOUT_IDENT );
    lUnicastIP->addWidget( mUnicastIPLabel );
    lUnicastIP->addStretch();
    lUnicastIP->addWidget( mUnicastIPCtrl );

    QHBoxLayout *lUnicastPort = new QHBoxLayout;
    lUnicastPort->addSpacing( LAYOUT_IDENT );
    lUnicastPort->addWidget( mUnicastPortLabel );
    lUnicastPort->addStretch();
    lUnicastPort->addWidget( mUnicastPortEdit );

    QHBoxLayout *lMulticastIP = new QHBoxLayout;
    lMulticastIP->addSpacing( LAYOUT_IDENT );
    lMulticastIP->addWidget( mMulticastIPLabel );
    lMulticastIP->addStretch();
    lMulticastIP->addWidget( mMulticastIPCtrl );

    QHBoxLayout *lMulticastPort = new QHBoxLayout;
    lMulticastPort->addSpacing( LAYOUT_IDENT );
    lMulticastPort->addWidget( mMulticastPortLabel );
    lMulticastPort->addStretch();
    lMulticastPort->addWidget( mMulticastPortEdit );

    QVBoxLayout *lLayout = new QVBoxLayout;
    lLayout->addWidget( mUnicastAutoRadio );
    lLayout->addWidget( mUnicastSpecificRadio );
    lLayout->addLayout( lUnicastSpecificPort );
    lLayout->addWidget( mUnicastOtherRadio );
    lLayout->addLayout( lUnicastIP );
    lLayout->addLayout( lUnicastPort );
    lLayout->addWidget( mMulticastRadio );
    lLayout->addLayout( lMulticastIP );
    lLayout->addLayout( lMulticastPort );
    lLayout->addStretch();

    lBox->setLayout( lLayout );

    return lBox;
}


///
/// \brief Start thread, call base class
///

int SetupDlg::exec()
{
    LoadToUI();
    return QDialog::exec();
}


///
/// \brief Loads data to the UI
///

void SetupDlg::LoadToUI()
{
    SetCurrentRole( mSetup->GetRole() );
    SetCurrentDestination( mSetup->GetDestination() );

    QString lStr;

    // Specific port
    lStr.sprintf( "%i", mSetup->GetUnicastSpecificPort() );
    mUnicastSpecificPortEdit->setText( lStr );

    // Default channel
    lStr.sprintf( "%i", mSetup->GetDefaultChannel() );
    mDefaultChannelEdit->setText( lStr );

    // IP
    IPStrToCtrl( mSetup->GetUnicastIP().GetAscii(), mUnicastIPCtrl );

    // Port
    lStr.sprintf( "%i", mSetup->GetUnicastPort() );
    mUnicastPortEdit->setText( lStr );

    // Multicast IP
    IPStrToCtrl( mSetup->GetMulticastIP().GetAscii(), mMulticastIPCtrl );

    // Multicast port
    lStr.sprintf( "%i", mSetup->GetMulticastPort() );
    mMulticastPortEdit->setText( lStr );

    EnableInterface();
}


///
/// \brief Sets a string IP address into an edit control
///

void SetupDlg::IPStrToCtrl( const QString &aIPStr, QLineEdit *aCtrl )
{
    aCtrl->setText( aIPStr );
}


///
/// \brief Sets the enabled state of the form's UI
///

void SetupDlg::EnableInterface()
{
    Setup::Role lRole = GetCurrentRole();
    Setup::Destination lDestination = GetCurrentDestination();

    mSetup->UpdateEnabled( lRole, lDestination );

    // Refresh destination UI selection (it may have changed in UpdateEnagled)
    SetCurrentDestination( lDestination );

    mCtrlDataRadio->setEnabled( mSetup->IsRoleCtrlDataEnabled() );
    mCtrlRadio->setEnabled( mSetup->IsRoleCtrlEnabled() );
    mDataRadio->setEnabled( mSetup->IsRoleDataEnabled() );
    mUnicastSpecificRadio->setEnabled( mSetup->IsDestUnicastSpecificEnabled() );
    mUnicastAutoRadio->setEnabled( mSetup->IsDestUnicastAutoEnabled() );
    mUnicastOtherRadio->setEnabled( mSetup->IsDestUnicastOtherEnabled() );
    mMulticastRadio->setEnabled( mSetup->IsDestMulticastEnabled() );
    mDefaultChannelEdit->setEnabled( mSetup->IsChannelEnabled() );

    mDefaultChannelLabel->setEnabled( mDefaultChannelEdit->isEnabled() );

    mUnicastSpecificPortEdit->setEnabled( ( lDestination == Setup::DestinationUnicastSpecific ) && mUnicastSpecificRadio->isEnabled() );
    mUnicastSpecificPortLabel->setEnabled( mUnicastSpecificPortEdit->isEnabled() );

    mUnicastIPCtrl->setEnabled( ( lDestination == Setup::DestinationUnicastOther ) && mUnicastOtherRadio->isEnabled() );
    mUnicastPortEdit->setEnabled( mUnicastIPCtrl->isEnabled() );
    mUnicastIPLabel->setEnabled( mUnicastIPCtrl->isEnabled() );
    mUnicastPortLabel->setEnabled( mUnicastIPCtrl->isEnabled() );

    mMulticastIPCtrl->setEnabled( ( lDestination == Setup::DestinationMulticast ) && mMulticastRadio->isEnabled() );
    mMulticastPortEdit->setEnabled( mMulticastIPCtrl->isEnabled() );
    mMulticastIPLabel->setEnabled( mMulticastIPCtrl->isEnabled() );
    mMulticastPortLabel->setEnabled( mMulticastIPCtrl->isEnabled() );
}


///
/// \brief Form accept handler: moves from UI to data
///

void SetupDlg::accept()
{
    if ( mSetup->IsEnabled() )
    {
        mSetup->SetRole( GetCurrentRole() );
        mSetup->SetDestination( GetCurrentDestination() );

        int lPort = 0;
        int lChannel = 0;
        int lCount = 0;
        QString lStr;

        lStr = mUnicastSpecificPortEdit->text();
        lCount = sscanf( lStr.toAscii(), "%d", &lPort );
        mSetup->SetUnicastSpecificPort( static_cast<unsigned short>( lPort ) );

        lStr = mDefaultChannelEdit->text();
        lCount = sscanf( lStr.toAscii(), "%i", &lChannel );
        if ( lCount != 1 )
        {
            QMessageBox::warning( this, "eBUS Player", "Error parsing default channel." );
            return;
        }
        if ( ( lChannel < 0 ) || ( lChannel > 511 ) )
        {
            QMessageBox::warning( this, "eBUS Player", "Default channel out of range. It must be between 0 and 511 inclusively." );
            return;
        }
        mSetup->SetDefaultChannel( static_cast<unsigned short>( lChannel ) );
        mSetup->SetUnicastIP( mUnicastIPCtrl->text().toAscii().data() );

        lStr = mUnicastPortEdit->text();
        lCount = sscanf( lStr.toAscii(), "%d", &lPort );
        if ( lCount != 1 )
        {
            QMessageBox::warning( this, "eBUS Player", "Error parsing unicast specific port." );
            return;
        }
        mSetup->SetUnicastPort( static_cast<unsigned short>( lPort ) );
        mSetup->SetMulticastIP( mMulticastIPCtrl->text().toAscii().data() );

        lStr = mMulticastPortEdit->text();
        lCount = sscanf( lStr.toAscii(), "%d", &lPort );
        if ( lCount != 1 )
        {
            QMessageBox::warning( this, "eBUS Player", "Error parsing multicast port." );
            return;
        }
        mSetup->SetMulticastPort( static_cast<unsigned short>( lPort ) );
    }

    QDialog::accept();
}


///
/// \brief Closes the form without saving the data.
///

void SetupDlg::reject()
{
    QDialog::reject();
}


///
/// \brief Handler for all radio buttons
///

void SetupDlg::OnRadioClicked()
{
    EnableInterface();
}


///
/// \brief Sets the role on the UI
///

void SetupDlg::SetCurrentRole( Setup::Role aRole )
{
    switch ( aRole )
    {
    case Setup::RoleCtrlData:
        mCtrlDataRadio->setChecked( true );
        break;

    case Setup::RoleCtrl:
        mCtrlRadio->setChecked( true );
        break;

    case Setup::RoleData:
        mDataRadio->setChecked( true );
        break;

    default:
        assert( 0 );
        break;
    }
}


///
/// \brief Sets the destination on the UI
///

void SetupDlg::SetCurrentDestination( Setup::Destination aDestination )
{
    switch ( aDestination )
    {
    case Setup::DestinationUnicastAuto:
        mUnicastAutoRadio->setChecked( true );
        break;

    case Setup::DestinationUnicastOther:
        mUnicastOtherRadio->setChecked( true );
        break;

    case Setup::DestinationMulticast:
        mMulticastRadio->setChecked( true );
        break;

    case Setup::DestinationUnicastSpecific:
        mUnicastSpecificRadio->setChecked( true );
        break;

    default:
        assert( 0 );
        break;
    }
}


///
/// \brief Returns the current role from the UI
///

Setup::Role SetupDlg::GetCurrentRole()
{
    if ( mCtrlDataRadio->isChecked() ) return Setup::RoleCtrlData;
    if ( mCtrlRadio->isChecked() ) return Setup::RoleCtrl;
    if ( mDataRadio->isChecked() ) return Setup::RoleData;

    return Setup::RoleInvalid;
}


///
/// \brief Returns the current destination from the UI
///

Setup::Destination SetupDlg::GetCurrentDestination()
{
    if ( mUnicastAutoRadio->isChecked() ) return Setup::DestinationUnicastAuto;
    if ( mUnicastOtherRadio->isChecked() ) return Setup::DestinationUnicastOther;
    if ( mMulticastRadio->isChecked() ) return Setup::DestinationMulticast;
    if ( mUnicastSpecificRadio->isChecked() ) return Setup::DestinationUnicastSpecific;

    return Setup::DestinationInvalid;
}

