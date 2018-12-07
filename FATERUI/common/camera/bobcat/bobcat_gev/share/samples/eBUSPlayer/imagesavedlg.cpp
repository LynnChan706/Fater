// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "imagesavedlg.h"
#include "filteringdlg.h"

#include <assert.h>
#include <QtGui/QFileDialog>
#include <QtCore/QDir>

#ifdef WIN32
#include <shlobj.h>
#endif

#ifndef WIN32
#include <sys/time.h>
#endif


///
/// \brief Constructor
///

ImageSaveDlg::ImageSaveDlg( ImageSaving *aImageSaving, QWidget* aParent )
    : QDialog( aParent )
    , mImageSaving( aImageSaving )
{
    CreateLayout();
}


///
/// \brief Destructor
///

ImageSaveDlg::~ImageSaveDlg()
{
}


///
/// \brief Modal display entry point
///

int ImageSaveDlg::exec()
{
    ToDialog();

	return QDialog::exec();
}


///
/// \brief Creates the form's layout
///

void ImageSaveDlg::CreateLayout()
{
    mSaveEnabledCheck = new QCheckBox("Enable Image Saving");
    QObject::connect( mSaveEnabledCheck, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

	QGroupBox *lLocationBox = CreateLocationBox();
	QGroupBox *lFormatBox = CreateFormatBox();
	QGroupBox *lThrottlingBox = CreateThrottlingBox();
	QHBoxLayout *lButtons = new QHBoxLayout;
    mOKButton = new QPushButton( tr( "OK" ) );
    mCancelButton = new QPushButton( tr( "Cancel" ) );
    lButtons->addStretch();
    lButtons->addWidget( mOKButton );
    lButtons->addWidget( mCancelButton );

    QObject::connect( mOKButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    QObject::connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QVBoxLayout *lLayout = new QVBoxLayout;
    lLayout->addWidget(mSaveEnabledCheck);
    lLayout->addWidget(lLocationBox);
    lLayout->addWidget(lFormatBox);
    lLayout->addWidget(lThrottlingBox);
    lLayout->addStretch( 100 );
    lLayout->addLayout(lButtons);

    setLayout( lLayout );
    setFixedSize( 520, 400 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Image Saving Options" ) );
}


///
/// \brief Creates the format group box
///

QGroupBox *ImageSaveDlg::CreateFormatBox()
{
    QGroupBox *lFormatBox = new QGroupBox( tr( "Format" ) );
    mFormatCombo = new QComboBox();
    mFormatCombo->addItem( "Device independent bitmap (BMP)" );
    mFormatCombo->addItem( "Tagged Image File Format (TIFF)" );
    mFormatCombo->addItem( "Raw binary data" );
    if ( mImageSaving->IsMp4Supported() )
    {
        mFormatCombo->addItem( "H.264 video in MP4 container" );
    }
	QHBoxLayout *lLayout = new QHBoxLayout;
	lLayout->addWidget(mFormatCombo);
	lFormatBox->setLayout(lLayout);
	return lFormatBox;
}


///
/// \brief Creates the location group box
///

QGroupBox *ImageSaveDlg::CreateLocationBox()
{
	QGroupBox *lLocationBox = new QGroupBox( tr( "Location to save images" ) );

	mSavePathEdit = new QLineEdit();

	mLocationButton = new QPushButton( "..." );
	mLocationButton->setMaximumWidth( 40 );

	QHBoxLayout *lLayout = new QHBoxLayout;
	lLayout->addWidget(mSavePathEdit);
	lLayout->addWidget(mLocationButton);

	QObject::connect( mLocationButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedLocationButton() ) );

	lLocationBox->setLayout( lLayout );
	return lLocationBox;
}


///
/// \brief Creates the throttling group box
///

QGroupBox *ImageSaveDlg::CreateThrottlingBox()
{
	QGroupBox *lThrottlingBox = new QGroupBox( tr( "Throttling options" ) );

	mOneOutOfRadio = new QRadioButton("Save one image out of every");
	mOneOutOfEdit = new QLineEdit();
	mOneOutOfEdit->setMinimumWidth( 75 );
	mOneOutOfEdit->setMaximumWidth( 75 );
	mCapturedImagesLabel = new QLabel("captured images.");
	QHBoxLayout *lOneOutOfLayout = new QHBoxLayout;
	lOneOutOfLayout->addWidget(mOneOutOfRadio);
	lOneOutOfLayout->addWidget(mOneOutOfEdit);
	lOneOutOfLayout->addWidget(mCapturedImagesLabel);
	lOneOutOfLayout->addStretch( 100 );
	QObject::connect( mOneOutOfRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

	mMaxRateRadio = new QRadioButton("Save a maximum of one image out of every");
	mMaxRateEdit = new QLineEdit();
	mMaxRateEdit->setMinimumWidth( 75 );
	mMaxRateEdit->setMaximumWidth( 75 );
	mMsLabel = new QLabel("ms.");
	QHBoxLayout *lMaxRateLayout = new QHBoxLayout;
	lMaxRateLayout->addWidget(mMaxRateRadio);
	lMaxRateLayout->addWidget(mMaxRateEdit);
	lMaxRateLayout->addWidget(mMsLabel);
	lMaxRateLayout->addStretch( 100 );
	QObject::connect( mMaxRateRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

	mAverageThroughputRadio = new QRadioButton("Throttle throughput to HDD. Maintain");
	mAverageThroughputEdit = new QLineEdit();
	mAverageThroughputEdit->setMinimumWidth( 75 );
	mAverageThroughputEdit->setMaximumWidth( 75 );
	mAverageLabel = new QLabel("Mbits/s average.");
	QHBoxLayout *lAverageThroughputLayout = new QHBoxLayout;
	lAverageThroughputLayout->addWidget(mAverageThroughputRadio);
	lAverageThroughputLayout->addWidget(mAverageThroughputEdit);
	lAverageThroughputLayout->addWidget(mAverageLabel);
	lAverageThroughputLayout->addStretch( 100 );
	QObject::connect( mAverageThroughputRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

	mNoThrottleRadio = new QRadioButton("No throttling (Limited by CPU, HDD, etc. Images may be dropped).");
	QObject::connect( mNoThrottleRadio, SIGNAL( clicked() ), this, SLOT( OnRadioClicked() ) );

	QVBoxLayout *lLayout = new QVBoxLayout;
	lLayout->addLayout(lOneOutOfLayout);
	lLayout->addLayout(lMaxRateLayout);
	lLayout->addLayout(lAverageThroughputLayout);
	lLayout->addWidget(mNoThrottleRadio);

	lThrottlingBox->setLayout( lLayout );
	return lThrottlingBox;
}


///
/// \brief Accept QDialog override
///

void ImageSaveDlg::accept()
{
    if (FromDialog())
    {
        QDialog::accept();
    }
}


///
/// \brief Reject QDialog override
///

void ImageSaveDlg::reject()
{
    QDialog::reject();
}


///
/// \brief Location button clicked handler
///

void ImageSaveDlg::OnBnClickedLocationButton()
{
	 QString dir = QFileDialog::getExistingDirectory( this, "Save images to folder",
         mSavePathEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

	 mSavePathEdit->setText(dir);
}


///
/// \brief Radio button clicked handler
///

void ImageSaveDlg::OnRadioClicked()
{
   EnableInterface();
}


///
/// \brief Enabled the UI
///

void ImageSaveDlg::EnableInterface()
{
	bool lEnabled = mSaveEnabledCheck->isChecked();
	mOneOutOfEdit->setEnabled( lEnabled && mOneOutOfRadio->isChecked() );
	mMaxRateEdit->setEnabled( lEnabled && mMaxRateRadio->isChecked() );
	mAverageThroughputEdit->setEnabled( lEnabled && mAverageThroughputRadio->isChecked());
	mSavePathEdit->setEnabled( lEnabled );
	mLocationButton->setEnabled( lEnabled );
	mOneOutOfRadio->setEnabled( lEnabled );
	mMaxRateRadio->setEnabled( lEnabled );
	mAverageThroughputRadio->setEnabled( lEnabled );
	mNoThrottleRadio->setEnabled( lEnabled );
	mCapturedImagesLabel->setEnabled( lEnabled );
	mMsLabel->setEnabled( lEnabled );
	mAverageLabel->setEnabled( lEnabled );
	mFormatCombo->setEnabled( lEnabled );
}


///
/// \brief Loads data on the dialog
///

void ImageSaveDlg::ToDialog()
{
    switch ( mImageSaving->GetFormat() )
    {
    default:
        assert( 0 );
        break;

    case ImageSaving::FormatBmp:
        mFormatCombo->setCurrentIndex( 0 );
        break;

    case ImageSaving::FormatTiff:
        mFormatCombo->setCurrentIndex( 1 );
        break;

    case ImageSaving::FormatRaw:
        mFormatCombo->setCurrentIndex( 2 );
        break;

    case ImageSaving::FormatMp4:
        mFormatCombo->setCurrentIndex( 3 );
        break;

    }

    QVariant lOneOutOf( mImageSaving->GetOneOutOf() );
    mOneOutOfEdit->setText( lOneOutOf.toString() );

    QVariant lMaxRate( mImageSaving->GetMaxRate() );
    mMaxRateEdit->setText( lMaxRate.toString() );

    QVariant lAverageThroughput( mImageSaving->GetAverageThroughput() );
    mAverageThroughputEdit->setText( lAverageThroughput.toString() );

    switch ( mImageSaving->GetThrottling() )
    {
    default:
        assert( 0 );
        break;

    case ImageSaving::ThrottleOneOutOf:
        mOneOutOfRadio->setChecked(true);
        break;
    case ImageSaving::ThrottleMaxRate:
        mMaxRateRadio->setChecked(true);
        break;
    case ImageSaving::ThrottleAverageThroughput:
        mAverageThroughputRadio->setChecked(true);
        break;
    case ImageSaving::ThrottleNone:
        mNoThrottleRadio->setChecked(true);
        break;
    }

    mSavePathEdit->setText( mImageSaving->GetPath().GetAscii() );
    mSaveEnabledCheck->setChecked( mImageSaving->GetEnabled() );

    EnableInterface();
}


///
/// \brief Saves data from the dialog
///

bool ImageSaveDlg::FromDialog()
{
    bool lSuccess = true;

    if ( mOneOutOfRadio->isChecked() )
    {
        mImageSaving->SetThrottling( ImageSaving::ThrottleOneOutOf );
    }
    else if ( mMaxRateRadio->isChecked() )
    {
        mImageSaving->SetThrottling( ImageSaving::ThrottleMaxRate );
    }
    else if ( mAverageThroughputRadio->isChecked() )
    {
        mImageSaving->SetThrottling( ImageSaving::ThrottleAverageThroughput );
    }
    else if ( mNoThrottleRadio->isChecked() )
    {
        mImageSaving->SetThrottling( ImageSaving::ThrottleNone );
    }
    else
    {
        assert(false);
    }

    mImageSaving->SetOneOutOf( mOneOutOfEdit->text().toInt() );
    mImageSaving->SetMaxRate( mMaxRateEdit->text().toInt() );
    mImageSaving->SetAverageThroughput( mAverageThroughputEdit->text().toInt() );

    mImageSaving->SetPath( mSavePathEdit->text().toAscii().data() );
    mImageSaving->SetEnabled( mSaveEnabledCheck->isChecked() );

    switch ( mFormatCombo->currentIndex() )
    {
    default:
        assert( 0 );
        break;

    case 0:
        mImageSaving->SetFormat( ImageSaving::FormatBmp );
        break;

    case 1:
        mImageSaving->SetFormat( ImageSaving::FormatTiff );
        break;

    case 2:
        mImageSaving->SetFormat( ImageSaving::FormatRaw );
        break;

    case 3:
        mImageSaving->SetFormat( ImageSaving::FormatMp4 );
        break;
    }

    return lSuccess;
}

