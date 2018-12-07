// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "aboutbox.h"
#include <PvVersion.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPainter>


AboutBox::AboutBox( QWidget* aParent )
    : QDialog( aParent )
{
	CreateLayout();
}

AboutBox::~AboutBox()
{


}

void AboutBox::CreateLayout()
{
	setWindowTitle( "About eBUS Player..." );

	QFont lBoldFont;
	lBoldFont.setBold( true );

	mOKButton = new QPushButton("&OK");
    QObject::connect( mOKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    mBitmapLabel = new QLabel;
    mBackground = new QPixmap( ":eBUSPlayer/res/eBUSPlayer_about.bmp" );
    mBitmapLabel->setPixmap( *mBackground );

    mEBUSPlayerLabel = new QLabel("eBUS Player");
	mEBUSPlayerLabel->setFont(lBoldFont);

	QString lPureGEVString;
	lPureGEVString.sprintf( "eBUS SDK version %d.%d.%d (build %d)", VERSION_MAJOR, VERSION_MINOR, VERSION_SUB, VERSION_BUILD );

	mPureGEVLabel = new QLabel( lPureGEVString );
	mCopyrightLabel = new QLabel( VERSION_COPYRIGHT );
	mPleoraLabel = new QLabel( VERSION_COMPANY_NAME );

	QVBoxLayout *lLayout = new QVBoxLayout;
	lLayout->addWidget( mBitmapLabel );
	lLayout->addStretch( 25 );
	lLayout->addWidget( mEBUSPlayerLabel );
	lLayout->addWidget( mPureGEVLabel );
	lLayout->addWidget( mCopyrightLabel );
	lLayout->addWidget( mPleoraLabel );
	lLayout->addStretch( 50 );
    lLayout->addWidget( mOKButton );

    setLayout( lLayout );

    setFixedSize( 400, 380 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    // lFlags |= Qt::FramelessWindowHint;
    // lFlags &= ~Qt::WindowTitleHint;
    setWindowFlags( lFlags );
}


