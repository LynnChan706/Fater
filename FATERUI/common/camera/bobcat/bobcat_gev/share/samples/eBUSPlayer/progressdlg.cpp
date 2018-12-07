// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "progressdlg.h"
#include "ebusplayer.h"

#include <QtGui/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QtGui/QBoxLayout>

#include <PvDevice.h>
#include <PvInterface.h>

#include <assert.h>


class TaskThread : public QThread
{
public:

    TaskThread( Task *aTask )
        : mTask( aTask )
    {
    }

    void Sleep( unsigned long aTimeout )
    {
        msleep( aTimeout );
    }

protected:

    virtual void run()
    {
        mTask->Execute();
    }

private:

    Task *mTask;
};


///
/// \brief Constructor
///

ProgressDlg::ProgressDlg( QWidget* aParent )
    : QDialog( aParent )
    , mThread( NULL )
{
    CreateLayout();

    mTimer = new QTimer( this );
    mTimer->setInterval( 100 );
    connect( mTimer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
    mTimer->start();
}


///
/// \brief Destructor
///

ProgressDlg::~ProgressDlg()
{
    if ( mTimer != NULL )
    {
        delete mTimer;
        mTimer = NULL;
    }

    if ( mThread != NULL )
    {
        delete mThread;
        mThread = NULL;
    }
}


///
/// \brief Creates the layout of the dialog
///

void ProgressDlg::CreateLayout()
{
    mStatusLabel = new QLabel( "" );
    mStatusLabel->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    mStatusLabel->setWordWrap( true );

    QVBoxLayout *lLayout = new QVBoxLayout;
    QBoxLayout *lBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    lBoxLayout->addSpacing(30);
    lBoxLayout->addWidget( mStatusLabel);
    lLayout->addLayout(lBoxLayout);

    setLayout( lLayout );
    setFixedSize( 400, 120 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    lFlags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags( lFlags );

    mWheel = new QPixmap( ":eBUSPlayer/res/wheel.bmp" );
    mWheel->setMask( mWheel->createMaskFromColor( QColor( 0xFF, 0xFF, 0xFF ) ) );

    setWindowTitle( "Connection Progress" );
}


///
/// \brief Paint event. Draws the spinning wheel.
///

void ProgressDlg::paintEvent( QPaintEvent * event )
{
	QPainter lPainter( this );

	QRectF target( 12, 15, 16, 16 );
	QRectF source( mWheelIndex * 16, 0, 16, 16 );
	lPainter.drawPixmap( target, *mWheel, source );

	QWidget::paintEvent( event );
}


///
/// \brief Timer event handler. Update status and close the dialog is the task is completed.
///

void ProgressDlg::OnTimer()
{
    // Advance to next image in sprite
    ( ++mWheelIndex ) %= 8;

    // Tell Qt to repaint
    update();

    // Update dialog
    Update();

	// If the device thread is done, complete and start stream thread
	if ( mThread->isFinished() )
	{
		accept();
	}
}


///
/// \brief Start thread, call base class
///

int ProgressDlg::exec()
{
    this->setWindowTitle( mTitle );

    assert( mThread == NULL );
    mThread = new TaskThread( mTask );
    mThread->start();

    return QDialog::exec();
}


///
/// \brief Update the UI with the current status
///

void ProgressDlg::Update()
{
    QString lOldStr;
    lOldStr = mStatusLabel->text();

    /////////////////////////////////////////////////////////////////
    mMutex.lock();

    if ( lOldStr != mStatus )
    {
        mStatusLabel->setText( mStatus );
    }

    mMutex.unlock();
    /////////////////////////////////////////////////////////////////
}


///
/// \brief Runs a task
///

int ProgressDlg::RunTask( Task *aTask )
{
    mTask = aTask;
    mTask->SetProgress( this );

    int lReturn = exec();

    mTask->SetProgress( NULL );
    mTask = NULL;

    return static_cast<int>( lReturn );
}


///
/// \brief Sets the progress status. Can be called from UI or task thread.
///

void ProgressDlg::SetStatus( const std::string &aStatus )
{
    /////////////////////////////////////////////////////////////////
    mMutex.lock();

    PvString lString( aStatus.c_str() );
    mStatus = lString.GetAscii();

    mMutex.unlock();
    /////////////////////////////////////////////////////////////////

    // Make sure every step is visible, if only for a little while
    mThread->Sleep( 50 );
}


///
/// \brief Shows a warning (status change + delay)
///

void ProgressDlg::ShowWarning( const std::string &aStatus )
{
    SetStatus( aStatus );
    mThread->Sleep( 3000 );
}



