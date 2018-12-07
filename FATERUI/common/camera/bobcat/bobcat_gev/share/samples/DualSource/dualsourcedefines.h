// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __DUALSOURCEDEFINES_H__
#define __DUALSOURCEDEFINES_H__


#include <QtGui/QMessageBox>


#define SAFE_DELETE( a ) \
	if ( ( a ) != NULL ) \
	{ \
        delete ( a ); \
		( a ) = NULL; \
	}


inline
void PvMessageBox( QWidget *aWidget, PvResult &aResult )
{
	QString lError = aResult.GetCodeString().GetAscii();
	QString lDescription = aResult.GetDescription().GetAscii();
	QMessageBox::critical( aWidget, "Error", lError + "\r\n\r\n" + lDescription + "\r\n" );
}


#endif // __DUALSOURCEDEFINES_H__


