// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "dualsource.h"

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
	// Forcing locale to English, US for consistency
	QLocale::setDefault( QLocale( QLocale::English, QLocale::UnitedStates ) );

	QApplication a(argc, argv);

    QCoreApplication::setOrganizationName( "Pleora Technologies Inc" );
    QCoreApplication::setOrganizationDomain( "pleora.com" );
    QCoreApplication::setApplicationName( "DualSource" );
    
    DualSource w;

#ifdef _LINUX_
    // When catching the signal, we will simulate the File\Exit menu usage to ensure a proper cleanup
    QObject::connect( &a, SIGNAL( aboutToQuit() ), &w, SLOT( OnFileExitAction() ) );
    signal( SIGTERM, SignalHangler );
    signal( SIGINT, SignalHangler );
#endif // _LINUX_

    w.show();

    return a.exec();
}



