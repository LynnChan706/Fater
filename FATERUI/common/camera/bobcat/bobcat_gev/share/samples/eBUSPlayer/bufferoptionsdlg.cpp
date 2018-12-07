// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "bufferoptionsdlg.h"

#include <PvPipeline.h>

#include <QtGui/QHBoxLayout> 
#include <QtGui/QGroupBox>
#include <QtGui/QMessageBox>

#include <assert.h>

#include "pvmessagebox.h"


///
/// \brief Constructor
///

BufferOptionsDlg::BufferOptionsDlg( Player *aPlayer, QWidget* aParent )
    : QDialog( aParent )
	, mPlayer( aPlayer )
{
	CreateLayout();
}


///
/// \brief Destructor
///

BufferOptionsDlg::~BufferOptionsDlg()
{
}


///
/// \brief UI creation
///

void BufferOptionsDlg::CreateLayout()
{
    // Buffer count

	mBufferCountEdit = new QLineEdit;
	mBufferCountEdit->setMinimumWidth( 80 );

	QHBoxLayout *lBufferCountLayout = new QHBoxLayout;
	lBufferCountLayout->addWidget( mBufferCountEdit );
	lBufferCountLayout->addWidget( new QLabel( "buffers" ) );

	QVBoxLayout *lBufferCountVLayout = new QVBoxLayout;
	lBufferCountVLayout->addLayout( lBufferCountLayout );
	lBufferCountVLayout->addStretch();

	mLabel1 = new QLabel;
	mLabel1->setAlignment( Qt::AlignTop | Qt::AlignLeft );
	mLabel1->setWordWrap( true );
	mLabel1->setMinimumWidth( 400 );

	QHBoxLayout *lHLayout1 = new QHBoxLayout;
	lHLayout1->addLayout( lBufferCountVLayout );
	lHLayout1->addStretch();
	lHLayout1->addWidget( mLabel1 );

	QGroupBox *lBufferCountBox = new QGroupBox( "Buffers used for streaming" );
	lBufferCountBox->setLayout( lHLayout1 );

	// Default buffer size

	mBufferSizeEdit = new QLineEdit;
	mBufferSizeEdit->setMinimumWidth( 80 );

	QHBoxLayout *lBufferSizeLayout = new QHBoxLayout;
	lBufferSizeLayout->addWidget( mBufferSizeEdit );
	lBufferSizeLayout->addWidget( new QLabel( "bytes" ) );

	QVBoxLayout *lBufferSizeVLayout = new QVBoxLayout;
	lBufferSizeVLayout->addLayout( lBufferSizeLayout );
	lBufferSizeVLayout->addStretch();

	mLabel2 = new QLabel;
	mLabel2->setAlignment( Qt::AlignTop | Qt::AlignLeft );
	mLabel2->setWordWrap( true );
	mLabel2->setMinimumWidth( 400 );

	QHBoxLayout *lHLayout2 = new QHBoxLayout;
	lHLayout2->addLayout( lBufferSizeVLayout );
	lHLayout2->addStretch();
	lHLayout2->addWidget( mLabel2 );

	QGroupBox *lBufferSizeBox = new QGroupBox( "Default buffer size" );
	lBufferSizeBox->setLayout( lHLayout2 );

	// Automatic buffer resizing

	mAutoResizeCheck = new QCheckBox( "Enabled" );

	QVBoxLayout *lAutoResizeVLayout = new QVBoxLayout;
	lAutoResizeVLayout->addWidget( mAutoResizeCheck );
	lAutoResizeVLayout->addStretch();

	mLabel3 = new QLabel;
	mLabel3->setAlignment( Qt::AlignTop | Qt::AlignLeft );
	mLabel3->setWordWrap( true );
	mLabel3->setMinimumWidth( 400 );

	QHBoxLayout *lHLayout3 = new QHBoxLayout;
	lHLayout3->addLayout( lAutoResizeVLayout );
	lHLayout3->addStretch();
	lHLayout3->addWidget( mLabel3 );

	QGroupBox *lAutoResizeBox = new QGroupBox( "Automatic buffer resizing" );
	lAutoResizeBox->setLayout( lHLayout3 );

	// OK and cancel buttons

	QHBoxLayout *lButtons = new QHBoxLayout;
    mOKButton = new QPushButton( tr( "OK" ) );
    mCancelButton = new QPushButton( tr( "Cancel" ) );
    lButtons->addStretch();
    lButtons->addWidget( mOKButton );
    lButtons->addWidget( mCancelButton );

    QObject::connect( mOKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    QObject::connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // Take everything above and build the dialog layout

    QVBoxLayout *lMain = new QVBoxLayout;
    lMain->addWidget( lBufferCountBox );
    lMain->addWidget( lBufferSizeBox );
    lMain->addWidget( lAutoResizeBox );
    lMain->addStretch();
    lMain->addLayout( lButtons );

    // Configure dialog

    setLayout( lMain );
    setFixedSize( 700, 520 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;

    lFlags |= Qt::WindowStaysOnTopHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Buffer Options" ) );
}


///
/// \brief Modal dialog startup
///

int BufferOptionsDlg::exec()
{
    BufferOptions *lBufferOptions = mPlayer->GetBufferOptions();

    mLabel1->setText( lBufferOptions->GetHelpBufferCount().GetAscii() );
    mLabel2->setText( lBufferOptions->GetHelpBufferSize().GetAscii() );
    mLabel3->setText( lBufferOptions->GetHelpAutoResizing().GetAscii() );

    QString lBufferCountString;
    lBufferCountString.setNum( lBufferOptions->GetBufferCount() );
    mBufferCountEdit->setText( lBufferCountString );

    QString lBufferSizeString;
    lBufferSizeString.setNum( lBufferOptions->GetBufferSize() );
    mBufferSizeEdit->setText( lBufferSizeString );

    mAutoResizeCheck->setChecked( lBufferOptions->GetAutoResize() );

    return QDialog::exec();
}


///
/// \brief QDialog OK override
///

void BufferOptionsDlg::accept()
{
    BufferOptions *lBufferOptions = mPlayer->GetBufferOptions();
    bool lOK = false;

    uint32_t lCount = mBufferCountEdit->text().toInt( &lOK );
    if ( !lOK )
    {
        // Parsing error
        QMessageBox::warning( this, "Error", "Invalid buffer count value." );
        return;
    }
    if ( lCount <= 0 )
    {
        QMessageBox::warning( this, "Error", "Buffer count cannot be zero." );
        return;
    }
    lBufferOptions->SetBufferCount( lCount );

    uint32_t lDefaultBufferSize = mBufferSizeEdit->text().toInt( &lOK );
    if ( !lOK )
    {
        // Parsing error
        QMessageBox::warning( this, "Error", "Invalid default buffer size value." );
        return;
    }
    if ( lDefaultBufferSize <= 0 )
    {
        QMessageBox::warning( this, "Error", "Default buffer size cannot be zero." );
        return;
    }
    lBufferOptions->SetBufferSize( lDefaultBufferSize );

    bool lAutoResize = true;
    if ( !mAutoResizeCheck->isChecked() )
    {
        lAutoResize = false;
    }
    lBufferOptions->SetAutoResize( lAutoResize );

    if ( !mPlayer->DoBufferOptionsRequireApply() )
    {
        // Nothing changed, nothing to do...
        QDialog::accept();
        return;
    }

    // If *A LOT* of memory is about to be allocated, ask the user first
    if ( lBufferOptions->ShowWarning() )
    {
        QString lMessage;
        lMessage.sprintf( "%.1f GB of memory will be required by the acquisition pipeline. Are you sure you want to apply these settings?",
                lBufferOptions->GetRequiredMemoryInGB() );

        QMessageBox::StandardButtons lRetVal = QMessageBox::question( this, "Warning", lMessage,
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );
        if ( lRetVal == QMessageBox::Cancel )
        {
            reject();
            return;
        }
        else if ( lRetVal != QMessageBox::Yes )
        {
            return;
        }
    }

    if ( mPlayer->IsStreamOpened() )
    {
		PvResult lResult = mPlayer->ApplyBufferOptions();
		if ( !lResult.IsOK() )
		{
			PvMessageBox( this, lResult );
		}
    }

    QDialog::accept();
}


