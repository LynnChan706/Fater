// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "actioncommanddlg.h"
#include "ebusplayer.h"
#include "pvmessagebox.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include <assert.h>
#include <limits>


///
/// \brief Constructor
///

ActionCommandDlg::ActionCommandDlg( eBUSPlayer *aEBUSPlayer )
    : QDialog( aEBUSPlayer )
	, mForceACKFlag( false )
{
	CreateLayout();
	FillNetworkList();
}


///
/// \brief Destructor
///

ActionCommandDlg::~ActionCommandDlg()
{
}


///
/// \brief Dynamic dialog layout creation
///

void ActionCommandDlg::CreateLayout()
{
	// Network interfaces group box

	QVBoxLayout *lNetworkVLayout = new QVBoxLayout;

	mNetworkList = new QListWidget;
	lNetworkVLayout->addWidget( mNetworkList );

	QGroupBox *lNetworkBox = new QGroupBox( "Network Interfaces" );
	lNetworkBox->setLayout( lNetworkVLayout );

	// Action command group box

    QGridLayout *lActionGLayout = new QGridLayout;
    int lRow = 0;
    int lColumn = 0;

    QLabel *lLabel = new QLabel( "Device Key" );
	lActionGLayout->addWidget( lLabel, lRow, lColumn );
	mDeviceKeyEdit = new QLineEdit( "0x00000000" );
	lActionGLayout->addWidget( mDeviceKeyEdit, lRow++, lColumn + 1 );

    lLabel = new QLabel( "Group Key" );
	lActionGLayout->addWidget( lLabel, lRow, lColumn );
	mGroupKeyEdit = new QLineEdit( "0x00000000" );
	lActionGLayout->addWidget( mGroupKeyEdit, lRow++, lColumn + 1 );

    lLabel = new QLabel( "Group Mask" );
	lActionGLayout->addWidget( lLabel, lRow, lColumn );
	mGroupMaskEdit = new QLineEdit( "0xFFFFFFFF" );
	lActionGLayout->addWidget( mGroupMaskEdit, lRow++, lColumn + 1 );

	lRow = 0;
	lColumn = 2;

    lLabel = new QLabel( "Request Acknowledgements" );
	lActionGLayout->addWidget( lLabel, lRow, lColumn );
	mForceACKFlagCheck = new QCheckBox;
	mForceACKFlagCheck->setChecked( false );
	lActionGLayout->addWidget( mForceACKFlagCheck, lRow++, lColumn + 1 );

    lLabel = new QLabel( "Scheduled" );
	lActionGLayout->addWidget( lLabel, lRow, lColumn );
	mScheduledCheck = new QCheckBox;
    QObject::connect( mScheduledCheck, SIGNAL( stateChanged( int ) ), this, SLOT( OnScheduledStateChanged( int ) ) );
	mScheduledCheck->setChecked( false );
	lActionGLayout->addWidget( mScheduledCheck, lRow++, lColumn + 1 );

    mTimestampLabel = new QLabel( "Scheduled Time" );
	lActionGLayout->addWidget( mTimestampLabel, lRow, lColumn );
	mTimestampEdit = new QLineEdit("0");
	lActionGLayout->addWidget( mTimestampEdit, lRow++, lColumn + 1 );

	mSendButton = new QPushButton( "Send" );
    QObject::connect( mSendButton, SIGNAL( clicked() ), this, SLOT( OnSendClick() ) );
	lActionGLayout->addWidget( mSendButton, lRow, 0, 1, 4 );

	QGroupBox *lActionBox = new QGroupBox( "Action Command" );
	lActionBox->setLayout( lActionGLayout );

	// Acknowledgements group box

	QVBoxLayout *lAckVLayout = new QVBoxLayout;

	mAcknowledgementsEdit = new QTextEdit;
	mAcknowledgementsEdit->setReadOnly( true );
	lAckVLayout->addWidget( mAcknowledgementsEdit );

	QGroupBox *lAckBox = new QGroupBox( "Acknowledgements" );
	lAckBox->setLayout( lAckVLayout );

    // Take everything above and build the dialog layout

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addWidget( lNetworkBox );
    lMain->addWidget( lActionBox );
    lMain->addWidget( lAckBox );

    // Configure dialog

    setLayout( lMain );
    setFixedSize( 700, 520 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    lFlags |= Qt::WindowStaysOnTopHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Action Command" ) );

	EnableInterface();
}


///
/// \brief Syncs the UI enabled state
///

void ActionCommandDlg::EnableInterface()
{
	bool lScheduled = mScheduledCheck->isChecked();
	mTimestampLabel->setEnabled( lScheduled );
	mTimestampEdit->setEnabled( lScheduled );
}


///
/// \brief Fills the network checked list
///

void ActionCommandDlg::FillNetworkList()
{
	mNetworkList->clear();

	// Go through all items
    uint32_t lCount = mActionCommand.GetInterfaceCount();
    for ( uint32_t i = 0; i < lCount; i++ )
    {
    	// Get description, IP, MAC
    	PvString lDescription, lIP, lMAC;
    	mActionCommand.GetInterfaceDescription( i, lDescription );
    	mActionCommand.GetInterfaceIPAddress( i, lIP );
    	mActionCommand.GetInterfaceMACAddress( i, lMAC );

    	// Add item
    	QString lText = lDescription.GetAscii();
    	lText += ", ";
    	lText += lIP.GetAscii();
    	lText += " ";
    	lText += lMAC.GetAscii();
		mNetworkList->addItem( lText );

		// Make item checkable, checked
		QListWidgetItem *lItem = mNetworkList->item( mNetworkList->count() - 1 );
		lItem->setFlags( lItem->flags() | Qt::ItemIsUserCheckable );
		lItem->setCheckState( Qt::Checked );
    }
}


///
/// \brief Send button click handler
///

void ActionCommandDlg::OnSendClick()
{
	if ( !Configure() )
	{
		return;
	}

	if ( !Send() )
	{
		return;
	}

	mAcknowledgementsEdit->setText( "" );
	MonitorAcknowledgements();
}


///
/// \brief Scheduled check box handler
///

void ActionCommandDlg::OnScheduledStateChanged( int aState )
{
	EnableInterface();
}


///
/// \brief Configures the action command from the dialog state
///

bool ActionCommandDlg::Configure()
{
	// Network interfaces
	int lItemCount = mNetworkList->count();
	if ( lItemCount == mActionCommand.GetInterfaceCount() )
	{
		for ( int i = 0; i < lItemCount; i++ )
		{
			QListWidgetItem *lItem = mNetworkList->item( i );
			bool lChecked = lItem->checkState() == Qt::Checked;

			mActionCommand.SetInterfaceEnabled( i, lChecked );
		}
	}

	// Device key
	uint64_t lDeviceKey = 0;
	if ( EditToInt( mDeviceKeyEdit, std::numeric_limits<uint32_t>::max(), lDeviceKey ) )
	{
		mActionCommand.SetDeviceKey( lDeviceKey );
	}
	else
	{
        QMessageBox::warning( this, "Error", "Unable to parse the Device Key parameter." );
		return false;
	}

	// Group key
	uint64_t lGroupKey = 0;
	if ( EditToInt( mGroupKeyEdit, std::numeric_limits<uint32_t>::max(), lGroupKey ) )
	{
		mActionCommand.SetGroupKey( lGroupKey );
	}
	else
	{
        QMessageBox::warning( this, "Error", "Unable to parse the Group Key parameter." );
		return false;
	}

	// Group mask
	uint64_t lGroupMask = 0;
	if ( EditToInt( mGroupMaskEdit, std::numeric_limits<uint32_t>::max(), lGroupMask ) )
	{
		mActionCommand.SetGroupMask( lGroupMask );
	}
	else
	{
        QMessageBox::warning( this, "Error", "Unable to parse the Group Mask parameter." );
		return false;
	}

	// Scheduled
	bool lScheduled = mScheduledCheck->isChecked();

	// Scheduled time
	mActionCommand.SetScheduledTimeEnable( lScheduled );
	if ( lScheduled )
	{
		uint64_t lScheduledTime = 0;
		if ( EditToInt( mTimestampEdit, std::numeric_limits<uint64_t>::max(), lScheduledTime ) )
		{
			mActionCommand.SetScheduledTime( lScheduledTime );
		}
		else
		{
	        QMessageBox::warning( this, "Error", "Unable to parse the Scheduled Time parameter." );
			return false;
		}
	}

	// Request acknowledgements
	mForceACKFlag = mForceACKFlagCheck->isChecked();

	return true;
}


///
/// \brief Send the action command
///

bool ActionCommandDlg::Send()
{
	PvResult lResult;
	lResult = mActionCommand.Send( 250, 0, mForceACKFlag );
	if ( !lResult.IsOK() )
	{
		PvMessageBox( this, lResult );
		return false;
	}

	return true;
}


///
/// \brief Monitors the acknowledgements after sending an action command
///

void ActionCommandDlg::MonitorAcknowledgements()
{
	QString lAcks;

	uint32_t lCount = mActionCommand.GetAcknowledgementCount();
	for ( uint32_t i = 0; i < lCount; i++ )
	{
		PvActionAckStatusEnum lStatus;
		mActionCommand.GetAcknowledgementStatus( i, lStatus );
		switch ( lStatus )
		{
		case PvActionAckStatusOK:
			lAcks += "[OK]";
			break;

		case PvActionAckStatusLate:
			lAcks += "[Late]";
			break;

		case PvActionAckStatusOverflow:
			lAcks += "[Overflow]";
			break;

		case PvActionAckStatusNoRefTime:
			lAcks += "[NoRefTime]";
			break;

		}

		lAcks += " from ";

		PvString lIPAddress;
		mActionCommand.GetAcknowledgementIPAddress( i, lIPAddress );
		lAcks += lIPAddress.GetAscii();

		lAcks += "\r\n";
	}

	mAcknowledgementsEdit->setText( lAcks );
}


///
/// \brief Parse a QLineEdit to int
///

bool ActionCommandDlg::EditToInt( QLineEdit *aEdit, uint64_t aMax, uint64_t &aValue )
{
	bool lOK = false;
	QString lString = aEdit->text().trimmed().toUpper();

	if ( ( lString.length() > 2 ) && ( lString.indexOf( "0X" ) == 0 ) )
	{
		// Starts by 0X: hexadecimal
		lString = lString.right( lString.length() - 2 );
		aValue = lString.toULongLong( &lOK, 16 );
	}
	else if ( ( lString.length() > 1 ) && ( lString[ lString.length() - 1 ] == 'H' ) )
	{
		// Ends by H: hexadecimal
		lString = lString.left( lString.length() - 1 );
		aValue = lString.toULongLong( &lOK, 16 );
	}
	else
	{
		// Decimal
		aValue = lString.toULongLong( &lOK, 10 );
	}

	if ( lOK && ( aValue <= aMax ) )
	{
		return true;
	}

	return false;
}


