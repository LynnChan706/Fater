// *****************************************************************************
//
//     Copyright (c) 2014, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#include "IGVTrueSenseUISampleDlg.h"

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

    IGVTrueSenseUISampleDlg lIGVTrueSenseUISampleDlg;

    a.setWindowIcon( QIcon( ":/IGVTrueSenseUISample/res/IGVTrueSenseUISample.bmp") );

    QTime lTimer;
    lTimer.start();

    while ( lTimer.elapsed() < 500 )
    {
    	a.processEvents();
    }

    QCoreApplication::setOrganizationName( "Imperx Inc" );
    QCoreApplication::setOrganizationDomain( "imperx.com" );
    QCoreApplication::setApplicationName( "IGVTrueSenseUISampleDlg" );
    
    while ( lTimer.elapsed() < 2500 )
    {
    	a.processEvents();
    }

#ifdef _LINUX_
    // When catching the signal, we will simulate the File\Exit menu usage to ensure a proper cleanup
    QObject::connect( &a, SIGNAL( aboutToQuit() ), &lIGVTrueSenseUISampleDlg, SLOT( OnFileExitAction() ) );
    signal( SIGTERM, SignalHangler );
    signal( SIGINT, SignalHangler );
#endif // _LINUX_

    lIGVTrueSenseUISampleDlg.show();

    while ( lTimer.elapsed() < 3000 )
    {
    	a.processEvents();
    }

       return a.exec();
}

