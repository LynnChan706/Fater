// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "eventmonitordlg.h"
#include <assert.h>

#include <QtGui/QFileDialog>

#include <sstream>


#define MAX_LOG_SIZE ( 256 * 1024 )


///
/// \brief Constructor
///

EventMonitorDlg::EventMonitorDlg( QWidget *aParent, LogBuffer *aLogBuffer )
	: QDialog( aParent )
	, mLogBuffer( aLogBuffer )
	, mLogEditTextLength( 0 )
{
	CreateLayout();
}


///
/// \brief Creates the form layout
///

void EventMonitorDlg::CreateLayout()
{
	mClearLogButton = new QPushButton( tr( "Clear log" ) );
    mLogEdit = new QTextEdit( this );
    mLogEdit->setReadOnly( true );
    QFont lTerminalFont( "Courier New", 8 );
    mLogEdit->setFont( lTerminalFont );
    mLogEdit->setUndoRedoEnabled( false );

	mGenICamCheckBox = new QCheckBox( tr( "GenICam parameter updates" ) );
	mEventsCheckBox = new QCheckBox( tr( "Messaging channel events" ) );
	mBufferErrorCheckBox = new QCheckBox( tr( "Buffer errors" ) );
	mBufferAllCheckBox = new QCheckBox( tr( "All buffer events" ) );

	mSaveLogCheckBox = new QCheckBox( tr( "Save to file" ) );
	mSaveLogLineEdit = new QLineEdit( tr( "" ) );
	mSaveLogButton = new QPushButton( tr( "..." ) );
	mSaveLogButton->setMaximumWidth( 32 );

    QHBoxLayout *lSaveLogLayout = new QHBoxLayout;
	lSaveLogLayout->addWidget( mSaveLogCheckBox );
	lSaveLogLayout->addWidget( mSaveLogLineEdit );
	lSaveLogLayout->addWidget( mSaveLogButton );

    QVBoxLayout *lLeft = new QVBoxLayout;
    lLeft->addWidget( mLogEdit );
	lLeft->addWidget( mClearLogButton );
	lLeft->addLayout( lSaveLogLayout );

    QVBoxLayout *lRight = new QVBoxLayout;
    lRight->addWidget( mGenICamCheckBox );
    lRight->addWidget( mEventsCheckBox );
    lRight->addWidget( mBufferErrorCheckBox );
    lRight->addWidget( mBufferAllCheckBox );
	lRight->addStretch();

	QHBoxLayout *lLayout = new QHBoxLayout;
	lLayout->addLayout( lLeft );
	lLayout->addLayout( lRight );

	setLayout( lLayout );
    setMinimumHeight( 400 );
    setMinimumWidth( 900 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Event Monitor" ) );

	QObject::connect( mSaveLogCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedSaveLogCheckBox() ) );
	QObject::connect( mSaveLogButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedSaveLogButton() ) );
	QObject::connect( mClearLogButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedClearButton() ) );
	QObject::connect( mGenICamCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedGenICamCheckBox() ) );
	QObject::connect( mEventsCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedEventsCheckBox() ) );
	QObject::connect( mBufferAllCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedBufferAllCheckBox() ) );
	QObject::connect( mBufferErrorCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedBufferErrorCheckBox() ) );

	mTimer = new QTimer(this);
    mTimer->setInterval( 200 );
	QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
	mTimer->start();

	LoadData();
}


///
/// \brief Destructor
///

EventMonitorDlg::~EventMonitorDlg()
{
	mTimer->stop();
}


///
/// \brief Clear button click handler
///

void EventMonitorDlg::OnBnClickedClearButton()
{
	mLogEdit->setText( "" );
	mLogEditTextLength = 0;
	mLogBuffer->Reset();
}


///
/// \brief Timer event handler
///

void EventMonitorDlg::OnTimer()
{
    std::list<std::string> &lEntries = mLogBuffer->Lock();
    if ( lEntries.size() <= 0 )
    {
        mLogBuffer->Unlock();
        return;
    }

    std::stringstream lSS;
    std::list<std::string>::iterator lIt =lEntries.begin();
    while ( lIt != lEntries.end() )
    {
        lSS << *( lIt++ ) << "\n";
    }

    std::string lStr = lSS.str();
    size_t lLength = lStr.length();

	mLogEditTextLength += lLength;
    mLogEdit->moveCursor( QTextCursor::End );
	mLogEdit->insertPlainText( lStr.c_str() );

    mLogBuffer->Unlock();

    if ( mLogEditTextLength > MAX_LOG_SIZE )
    {
    	QTextCursor lCursor = mLogEdit->textCursor();
    	lCursor.setPosition( 0, QTextCursor::MoveAnchor );
    	lCursor.setPosition( lLength - 1, QTextCursor::KeepAnchor );
    	lCursor.insertText( "" );

    	mLogEditTextLength -= lLength;
    }

    mLogEdit->moveCursor( QTextCursor::End );
}


///
/// \brief GenICam check box clicked handler
///

void EventMonitorDlg::OnBnClickedGenICamCheckBox()
{
	mLogBuffer->SetGenICamEnabled( mGenICamCheckBox->checkState() == Qt::Checked );
}


///
/// \brief Events check box clicked handler
///

void EventMonitorDlg::OnBnClickedEventsCheckBox()
{
	mLogBuffer->SetEventsEnabled( mEventsCheckBox->checkState() == Qt::Checked );
}


///
/// \brief All buffer events check box clicked handler
///

void EventMonitorDlg::OnBnClickedBufferAllCheckBox()
{
	mLogBuffer->SetBufferAllEnabled( mBufferAllCheckBox->checkState() == Qt::Checked );
}


///
/// \brief Buffer errors check box clicked handler
///

void EventMonitorDlg::OnBnClickedBufferErrorCheckBox()
{
	mLogBuffer->SetBufferErrorEnabled( mBufferErrorCheckBox->checkState() == Qt::Checked );
}


///
/// \brief Save log check box clicked handler
///

void EventMonitorDlg::OnBnClickedSaveLogCheckBox()
{
	mLogBuffer->SetWriteToFileEnabled( mSaveLogCheckBox->checkState() == Qt::Checked );
	EnableInterface();
}


///
/// \brief File selection button clicked handler
///

void EventMonitorDlg::OnBnClickedSaveLogButton()
{
	QFileDialog lDlg;
	lDlg.setAcceptMode( QFileDialog::AcceptSave );
	lDlg.setFileMode( QFileDialog::AnyFile );
	lDlg.setNameFilter( "Text files (*.txt);;All files (*.*)" );
	lDlg.selectFile( mLogBuffer->GetFilename().GetAscii() );

	if ( lDlg.exec() )
	{
		QStringList lList = lDlg.selectedFiles();
		if ( lList.size() > 0 )
		{
			mLogBuffer->SetFilename( (*lList.begin()).toAscii().data() );
		}
	}

	LoadData();
}


///
/// \brief Enables UI state
///

void EventMonitorDlg::EnableInterface()
{
	bool lEnabled = ( mSaveLogCheckBox->checkState() == Qt::Checked );
	mSaveLogLineEdit->setEnabled( false );
	mSaveLogButton->setEnabled( lEnabled );
}


///
/// \brief Load data from the model to the form
///

void EventMonitorDlg::LoadData()
{
	mGenICamCheckBox->setCheckState( ( mLogBuffer->IsGenICamEnabled() ) ? Qt::Checked : Qt::Unchecked );
	mEventsCheckBox->setCheckState( ( mLogBuffer->IsEventsEnabled() ) ? Qt::Checked : Qt::Unchecked );
	mBufferAllCheckBox->setCheckState( ( mLogBuffer->IsBufferAllEnabled() ) ? Qt::Checked : Qt::Unchecked );
	mBufferErrorCheckBox->setCheckState( ( mLogBuffer->IsBufferErrorEnabled() ) ? Qt::Checked : Qt::Unchecked );
	mSaveLogCheckBox->setCheckState( ( mLogBuffer->IsWriteToFileEnabled() ) ? Qt::Checked : Qt::Unchecked );
	mSaveLogLineEdit->setText( mLogBuffer->GetFilename().GetAscii() );
	
	EnableInterface();
}

