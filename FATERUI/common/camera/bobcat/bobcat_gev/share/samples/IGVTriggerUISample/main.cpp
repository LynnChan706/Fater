// *****************************************************************************
//
//     Copyright (c) 2013, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#include "IGVTriggerUISampleDlg.h"

#include <QtGui/QApplication>
#include <QtCore/QTime>

#ifdef _LINUX_
#include <signal.h>
#endif // _LINUX_

#ifdef _LINUX_
void SignalHangler( int aSig )
{
	qApp->quit();
}
#endif // _LINUX_

int main(int argc, char *argv[])
{
	// Bug 3013: now forcing locale to English, US for consistency
	QLocale::setDefault( QLocale( QLocale::English, QLocale::UnitedStates ) );
	QApplication a(argc, argv);

    IGVTriggerUISampleDlg lIGVTriggerUISampleDlg;

    a.setWindowIcon( QIcon( ":/IGVTriggerUISample/res/IGVTriggerUISample.bmp") );

    QTime lTimer;
    lTimer.start();

    while ( lTimer.elapsed() < 500 )
    {
    	a.processEvents();
    }

    QCoreApplication::setOrganizationName( "Imperx Inc" );
    QCoreApplication::setOrganizationDomain( "imperx.com" );
    QCoreApplication::setApplicationName( "IGVTriggerUISampleDlg" );
    
    while ( lTimer.elapsed() < 2500 )
    {
    	a.processEvents();
    }

#ifdef _LINUX_
    // When catching the signal, we will simulate the File\Exit menu usage to ensure a proper cleanup
    QObject::connect( &a, SIGNAL( aboutToQuit() ), &lIGVTriggerUISampleDlg, SLOT( OnFileExitAction() ) );
    signal( SIGTERM, SignalHangler );
    signal( SIGINT, SignalHangler );
#endif // _LINUX_

    lIGVTriggerUISampleDlg.show();

    while ( lTimer.elapsed() < 3000 )
    {
    	a.processEvents();
    }

       return a.exec();
}

