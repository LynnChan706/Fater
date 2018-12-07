// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "filetransferdlg.h"
#include "pvmessagebox.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFileDialog>
#include <QtCore/QThread>

#include <assert.h>
#include <algorithm>

#include <PvGenFile.h>


class TransferThread : public QThread
{
public:

	///
	/// \brief Constructor
	///

	TransferThread( PvGenParameterArray *aParameters )
		: mParameters( aParameters )
		, mMode( PvGenOpenModeUndefined )
	{

	}

	///
	/// \brief Destructor
	///

	virtual ~TransferThread()
	{
		if ( mFile.IsOpened() )
		{
			mFile.Close();
		}
	}

	///
	/// \brief Opens the device file. Call before starting the thread.
	///

	PvResult Open( PvGenOpenMode aMode, const QString &aDeviceFile, const QString &aHostFile )
	{
		mMode = aMode;
		mDeviceFile = aDeviceFile;
		mHostFile = aHostFile;

	    return mFile.Open( mParameters, mDeviceFile.toAscii().data(), mMode );
	}

	void GetCompleted( int64_t &aCompleted, int64_t &aTotal ) { mFile.GetProgress( aCompleted, aTotal ); }
	PvResult &GetResult() { return mResult; }

protected:

    virtual void run()
    {
        if ( mMode == PvGenOpenModeWrite )
        {
            // Write the file to the device
            mResult = mFile.WriteFrom( mHostFile.toAscii().data() );
        }
        else
        {
            // Read the file from the device
            mResult = mFile.ReadTo( mHostFile.toAscii().data() );
        }
    }

private:

    PvGenParameterArray *mParameters;
    PvGenFile mFile;

    PvGenOpenMode mMode;
    QString mDeviceFile;
    QString mHostFile;

    PvResult mResult;

};


///
/// \brief Constructor
///

FileTransferDlg::FileTransferDlg( PvDevice *aDevice, QWidget* aParent )
    : QDialog( aParent )
    , mDevice( aDevice )
	, mParameters( aDevice->GetParameters() )
	, mThread( NULL )
{
	CreateLayout();
    mDevice->RegisterEventSink( this );
}


///
/// \brief Destructor
///

FileTransferDlg::~FileTransferDlg()
{
    if ( mDevice != NULL )
    {
        mDevice->UnregisterEventSink( this );
    }
}


///
/// \brief Creates the UI layout
///

void FileTransferDlg::CreateLayout()
{
	mFileGroupBox = new QGroupBox( "Device File" );
	mFileComboBox = new QComboBox;
    QObject::connect( mFileComboBox, SIGNAL( activated( int ) ), this, SLOT( OnFileComboSelChanged( int ) ) );
	QVBoxLayout *lFileLayout = new QVBoxLayout;
	lFileLayout->addWidget( mFileComboBox );
	mFileGroupBox->setLayout( lFileLayout );

	mTransferGroupBox = new QGroupBox( "Transfer" );
	mDownloadButton = new QPushButton( "Download" );
    QObject::connect( mDownloadButton, SIGNAL( clicked() ), this, SLOT( OnDownloadClicked() ) );
	mUploadButton = new QPushButton( "Upload" );
    QObject::connect( mUploadButton, SIGNAL( clicked() ), this, SLOT( OnUploadClicked() ) );
	QHBoxLayout *lButtonsLayout = new QHBoxLayout;
	lButtonsLayout->addWidget(mDownloadButton);
	lButtonsLayout->addWidget(mUploadButton);
	mProgressBar = new QProgressBar;
	mProgressLabel = new QLabel;
	QVBoxLayout *lTransferLayout = new QVBoxLayout;
	lTransferLayout->addLayout( lButtonsLayout );
	lTransferLayout->addWidget( mProgressBar );
	lTransferLayout->addWidget( mProgressLabel );
	mTransferGroupBox->setLayout( lTransferLayout );

    mCancelButton = new QPushButton( "Cancel" );
    QObject::connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( OnCancelClicked() ) );

	QVBoxLayout *lLayout = new QVBoxLayout;
	lLayout->addWidget( mFileGroupBox );
	lLayout->addWidget( mTransferGroupBox );
    lLayout->addStretch( 100 );
	lLayout->addWidget( mCancelButton );

    setLayout( lLayout );
    setFixedSize( 520, 250 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    setWindowFlags( lFlags );

    setWindowTitle( "File Transfer" );

    mTimer = new QTimer( this );
    mTimer->setInterval( 100 );
    connect( mTimer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
}


///
/// \brief Enables UI state
///

void FileTransferDlg::EnableInterface()
{
    bool lTransfering = ( mThread != NULL );
    bool lFileEnabled = ( !lTransfering );

    mFileGroupBox->setEnabled( lFileEnabled );
    mFileComboBox->setEnabled( lFileEnabled );

    bool lUploadEnabled = false;
    bool lDownloadEnabled = false;

    if ( !lTransfering )
    {
        if ( mFileComboBox->currentIndex() >= 0 )
        {
            QString lFile = mFileComboBox->currentText();

            lUploadEnabled = PvGenFile::IsWritable( mParameters, lFile.toAscii().data() );
            lDownloadEnabled = PvGenFile::IsReadable( mParameters, lFile.toAscii().data() );
        }
    }

    mTransferGroupBox->setEnabled( lTransfering || lUploadEnabled || lDownloadEnabled );
    mDownloadButton->setEnabled( lDownloadEnabled );
    mUploadButton->setEnabled( lUploadEnabled );

    mProgressBar->setEnabled( lTransfering );
    mProgressLabel->setEnabled( lTransfering );
    mCancelButton->setEnabled( !lTransfering );
}


///
/// \brief Modal dialog execution entry point
///

int FileTransferDlg::exec()
{
    LoadFileCombo();
    EnableInterface();

    // We always bring the progress back on a 0-100 scale
    mProgressBar->setRange( 0, 100 );
    mProgressBar->setValue( 0 );

    return QDialog::exec();
}


///
/// \brief Dialog OK handler
///

void FileTransferDlg::accept()
{
	if ( mThread == NULL )
	{
		QDialog::accept();
	}
}


///
/// \brief Dialog Cancel handler
///

void FileTransferDlg::reject()
{
	if ( mThread == NULL )
	{
		QDialog::reject();
	}
}


///
/// \brief Loads all files to the combo box
///

void FileTransferDlg::LoadFileCombo()
{
    PvStringList lFiles;
    PvGenFile::GetFiles( mParameters, lFiles );

    mFileComboBox->clear();
    PvString *lString = lFiles.GetFirst();
    while ( lString != NULL )
    {
        mFileComboBox->addItem( lString->GetAscii() );
        lString = lFiles.GetNext();
    }
}


///
/// \brief File combo selection changed handler
///

void FileTransferDlg::OnFileComboSelChanged( int aIndex )
{
	EnableInterface();
}


///
/// \brief Download button clicked handler
///

void FileTransferDlg::OnDownloadClicked()
{
    if ( mThread != NULL )
    {
        return;
    }

    // Get device file
    QString lDeviceFile = mFileComboBox->currentText();

    // Get host file to download to
    QString lHostFile = QFileDialog::getSaveFileName(this, "Save File", lDeviceFile, "All files (*.*)");

    // Create thread object that will perform the file transfer
    mThread = new TransferThread( mParameters );
    PvResult lResult = mThread->Open( PvGenOpenModeRead, lDeviceFile, lHostFile );
    if ( !lResult.IsOK() )
    {
        delete mThread;
        mThread = NULL;

        PvMessageBox( this, lResult );
        return;
    }

    mThread->start();
    mTimer->start();

    EnableInterface();
}


///
/// \brief Upload button clicked handler
///

void FileTransferDlg::OnUploadClicked()
{
    if ( mThread != NULL )
    {
        return;
    }

    // Get device file
    QString lDeviceFile = mFileComboBox->currentText();

    // Get host file we upload to the device
    QString lHostFile = QFileDialog::getOpenFileName( this, "Load File", NULL, "All files (*.*)" );

    // Create thread object that will perform the file transfer
    mThread = new TransferThread( mParameters );
    PvResult lResult = mThread->Open( PvGenOpenModeWrite, lDeviceFile, lHostFile );
    if ( !lResult.IsOK() )
    {
        delete mThread;
        mThread = NULL;

        PvMessageBox( this, lResult );
        return;
    }

    mThread->start();
    mTimer->start();

    EnableInterface();
}


///
/// \brief Cancel button clicked handler
///

void FileTransferDlg::OnCancelClicked()
{
	reject();
}


///
/// \brief Time tick event: update the transfer progress
///

void FileTransferDlg::OnTimer()
{
    int64_t lCompleted = 0, lTotal = 0;
    mThread->GetCompleted( lCompleted, lTotal );

    double lDCompleted = static_cast<double>( lCompleted) / static_cast<double>( lTotal ) * 100.0f;
    short lProgress = qMin( static_cast<short>( 100 ), static_cast<short>( lDCompleted + 0.5 ) );

    mProgressBar->setValue( static_cast<int>( lProgress ) );

    QString lStr;
    lStr.sprintf( "%lld/%lld bytes transferred...", (long long)lCompleted, (long long)lTotal );
    mProgressLabel->setText( lStr );

    // If the thread is done, complete
    if ( mThread->isFinished() )
    {
    	mTimer->stop();

    	PvResult lResult = mThread->GetResult();
		if ( !lResult.IsOK() )
		{
			PvMessageBox( this, lResult );
		}

        mProgressBar->setValue( 100 );
        QMessageBox::information( this, "eBUS Player", "Transfer successfully completed.", QMessageBox::Ok );

    	delete mThread;
    	mThread = NULL;

        accept();
    }
}


///
/// \brief Callback that the device has been disconnected
///

void FileTransferDlg::OnLinkDisconnected( PvDevice *aDevice )
{
    // Only explicitly close the dialog here if the transfer has not yet started
    if ( mThread == NULL )
    {
        mDevice = NULL;
        reject();
    }
}


