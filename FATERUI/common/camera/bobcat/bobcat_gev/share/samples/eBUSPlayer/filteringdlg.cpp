// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "ebusplayer.h"
#include "filteringdlg.h"
#include "utilities.h" 
#include "histogramwidget.h"

#include <math.h>
#include <assert.h>


#define GAIN_TICK_COUNT ( 1000 )
#define GAIN_MAX ( 16 )

#define GRID_MIN_ROW_HEIGHT ( 24 )
#define SPIN_MIN_WIDTH ( 80 )
#define SPIN_MIN_HEIGHT ( 14 )


const double cNormalizer = log( static_cast<double>( GAIN_TICK_COUNT ) );


///
/// \brief Constructor
///

FilteringDlg::FilteringDlg( ImageFiltering *aImageFiltering, DisplayThread *aDisplayThread, QWidget* aParent )
    : QDialog( aParent )
    , mImageFiltering( aImageFiltering )
    , mFilter( &aImageFiltering->GetRGBFilter() )
    , mDisplayThread( aDisplayThread )
    , mSkipSyncSpinsCount( 0 )
    , mSkipSyncSlidersCount( 0 )
{
	CreateLayout();
	InitControls();
}


///
/// \brief Destructor
///

FilteringDlg::~FilteringDlg()
{
}


///
/// \brief Creates the UI layout
///

void FilteringDlg::CreateLayout()
{
	QVBoxLayout *lLayout = new QVBoxLayout;
	lLayout->addWidget( CreateRGBFiltering() );
	lLayout->addWidget( CreateBayer() );
	lLayout->addWidget( CreateHistogram() );

    setLayout( lLayout );
    setFixedSize( 620, 500 );

    Qt::WindowFlags lFlags = windowFlags();
    lFlags |= Qt::CustomizeWindowHint;
    lFlags &= ~Qt::WindowContextHelpButtonHint;
    lFlags &= ~Qt::WindowSystemMenuHint;
    
    lFlags |= Qt::WindowStaysOnTopHint;
    setWindowFlags( lFlags );

    setWindowTitle( tr( "Image Filtering" ) );
}


///
/// \brief Creates the RGB filtering group box
///

QGroupBox *FilteringDlg::CreateRGBFiltering()
{
	QGroupBox *lBox = new QGroupBox( "RGB Filtering" );

	QFont lBoldFont;
	lBoldFont.setBold( true );

	QGridLayout *lRGBLayout = new QGridLayout ;

	// First grid row
	int lRow = 0; int lCol = 0;
	QHBoxLayout *lEnabledLayout = new QHBoxLayout;
	mRGBFilterCheckBox = new QCheckBox( "Enabled" );
	QObject::connect( mRGBFilterCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedEnabledCheck() ) );
	mGainsLabel = new QLabel( "Gains" );
	mGainsLabel->setFont(lBoldFont);
	mOffsetsLabel = new QLabel( "Offsets" );
	mOffsetsLabel->setFont(lBoldFont);
	lRGBLayout->addWidget( mRGBFilterCheckBox, lRow, lCol++ );
	lCol++; // Skip slider
	lRGBLayout->addWidget( mGainsLabel, lRow, lCol++ );
	lCol++; // Skip slider
	lRGBLayout->addWidget( mOffsetsLabel, lRow, lCol++ );

	// RED grid row
	lRow++; lCol = 0;
	lRGBLayout->setRowMinimumHeight( lRow, GRID_MIN_ROW_HEIGHT );
	mRedLabel = new QLabel( "Red" );
	mRedLabel->setFont( lBoldFont );
	mRGSlider = new QSlider( Qt::Horizontal );
	mRGSpin = new QDoubleSpinBox;
	mRGSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mRGSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	mROSlider = new QSlider( Qt::Horizontal );
	mROSpin = new QSpinBox;
	mROSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mROSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	lRGBLayout->addWidget( mRedLabel, lRow, lCol++ );
	lRGBLayout->addWidget( mRGSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mRGSpin, lRow, lCol++ );
	lRGBLayout->addWidget( mROSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mROSpin, lRow, lCol++ );
	connect( mRGSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnRGSliderChanged( int ) ) );
	connect( mRGSpin, SIGNAL( valueChanged( double ) ), this, SLOT( OnRGSpinChanged( double ) ) );
	connect( mROSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnROSliderChanged( int ) ) );
	connect( mROSpin, SIGNAL( valueChanged( int ) ), this, SLOT( OnROSpinChanged( int ) ) );

	// GREEN grid row
	lRow++; lCol = 0;
	lRGBLayout->setRowMinimumHeight( lRow, GRID_MIN_ROW_HEIGHT );
	mGreenLabel = new QLabel( "Green" );
	mGreenLabel->setFont( lBoldFont );
	mGGSlider = new QSlider( Qt::Horizontal );
	mGGSpin = new QDoubleSpinBox;
	mGGSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mGGSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	mGOSlider = new QSlider( Qt::Horizontal );
	mGOSpin = new QSpinBox;
	mGOSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mGOSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	lRGBLayout->addWidget( mGreenLabel, lRow, lCol++ );
	lRGBLayout->addWidget( mGGSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mGGSpin, lRow, lCol++ );
	lRGBLayout->addWidget( mGOSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mGOSpin, lRow, lCol++ );
	connect( mGGSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnGGSliderChanged( int ) ) );
	connect( mGGSpin, SIGNAL( valueChanged( double ) ), this, SLOT( OnGGSpinChanged( double ) ) );
	connect( mGOSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnGOSliderChanged( int ) ) );
	connect( mGOSpin, SIGNAL( valueChanged( int ) ), this, SLOT( OnGOSpinChanged( int ) ) );

	// BLUE grid row
	lRow++; lCol = 0;
	lRGBLayout->setRowMinimumHeight( lRow, GRID_MIN_ROW_HEIGHT );
	mBlueLabel = new QLabel( "Blue" );
	mBlueLabel->setFont( lBoldFont );
	mBGSlider = new QSlider( Qt::Horizontal );
	mBGSpin = new QDoubleSpinBox;
	mBGSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mBGSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	mBOSlider = new QSlider( Qt::Horizontal );
	mBOSpin = new QSpinBox;
	mBOSpin->setMinimumHeight( SPIN_MIN_HEIGHT );
	mBOSpin->setMinimumWidth( SPIN_MIN_WIDTH );
	lRGBLayout->addWidget( mBlueLabel, lRow, lCol++ );
	lRGBLayout->addWidget( mBGSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mBGSpin, lRow, lCol++ );
	lRGBLayout->addWidget( mBOSlider, lRow, lCol++ );
	lRGBLayout->addWidget( mBOSpin, lRow, lCol++ );
	connect( mBGSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnBGSliderChanged( int ) ) );
	connect( mBGSpin, SIGNAL( valueChanged( double ) ), this, SLOT( OnBGSpinChanged( double ) ) );
	connect( mBOSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnBOSliderChanged( int ) ) );
	connect( mBOSpin, SIGNAL( valueChanged( int ) ), this, SLOT( OnBOSpinChanged( int ) ) );

	QHBoxLayout *lButtonLayout = new QHBoxLayout;
	mWBButton = new QPushButton( tr( "White Balance" ) );
	mResetButton = new QPushButton( tr( "Reset" ) );
	QObject::connect( mWBButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedWbbutton() ) );
	QObject::connect( mResetButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedResetbutton() ) );
	lButtonLayout->addWidget( mWBButton );
	lButtonLayout->addWidget( mResetButton );

	QVBoxLayout *lRGBFilteringLayout = new QVBoxLayout;
	lRGBFilteringLayout->addLayout( lRGBLayout );
	lRGBFilteringLayout->addLayout( lButtonLayout );

	lBox->setLayout( lRGBFilteringLayout );

    return lBox;
}


///
/// \brief Creates the RGB filtering group box
///

QGroupBox *FilteringDlg::CreateBayer()
{
	QGroupBox *lBox = new QGroupBox( "Bayer Interpolation" );

	QVBoxLayout *lBayerInterpolationLayout = new QVBoxLayout;
	mBayerCombo = new QComboBox();
	mBayerCombo->addItem( "Simple (2x2)" );

    QObject::connect( mBayerCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnBayerComboChanged( int ) ) );
	mBayerCombo->addItem( "Interpolation (3x3)" );

	lBayerInterpolationLayout->addWidget( mBayerCombo );
	lBox->setLayout( lBayerInterpolationLayout );

    return lBox;
}


///
/// \brief Creates the RGB filtering group box
///

QGroupBox *FilteringDlg::CreateHistogram()
{
    // Histogram
    mHistogram = new HistogramWidget( mImageFiltering->GetRangeFilter(), this );
    
    // Range filter check box
    mRangeFilterCheckBox = new QCheckBox( tr( "Enabled" ) );
	QObject::connect( mRangeFilterCheckBox, SIGNAL( clicked() ), this, SLOT( OnBnClickedRangeFilterCheck() ) );

    // Auto range button
    mAutoRangeButton = new QPushButton( tr( "Auto Configure" ) );
	QObject::connect( mAutoRangeButton, SIGNAL( clicked() ), this, SLOT( OnBnClickedAutoRange() ) );

    QVBoxLayout *lV1 = new QVBoxLayout;
    lV1->addWidget( mRangeFilterCheckBox );
    lV1->addWidget( mAutoRangeButton );
    lV1->addStretch();
    
    // Dark slider
    QLabel *lDarkLabel = new QLabel( tr( "Dark" ) );
    mDarkSlider = new QSlider( Qt::Horizontal );
    mDarkSlider->setRange( 0, 254 );
	connect( mDarkSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnDarkSliderChanged( int ) ) );

    // Light slider
    QLabel *lLightLabel = new QLabel( tr( "Light" ) );
    mLightSlider = new QSlider( Qt::Horizontal );
    mLightSlider->setRange( 1, 255 );
	connect( mLightSlider, SIGNAL( valueChanged( int ) ), this, SLOT( OnLightSliderChanged( int ) ) );

    QHBoxLayout *lHDark = new QHBoxLayout;
    lHDark->addWidget( lDarkLabel );
    lHDark->addWidget( mDarkSlider );
    
    QHBoxLayout *lHLight = new QHBoxLayout;
    lHLight->addWidget( lLightLabel );
    lHLight->addWidget( mLightSlider );

    QVBoxLayout *lVSliders = new QVBoxLayout;
    lVSliders->addLayout( lHDark );
    lVSliders->addLayout( lHLight );
    lVSliders->addStretch();
    
    QHBoxLayout *lHLayout = new QHBoxLayout;
    lHLayout->addLayout( lV1 );
    lHLayout->addLayout( lVSliders );

    QVBoxLayout *lVLayout = new QVBoxLayout;
    lVLayout->addWidget( mHistogram );
    lVLayout->addLayout( lHLayout );
    
    // Group box
	QGroupBox *lBox = new QGroupBox( "Histogram" );
    lBox->setLayout( lVLayout );

    return lBox; 
}


///
/// \brief Bayer combo selection changed event
///

void FilteringDlg::OnBayerComboChanged( int aIndex )
{
    // Bayer filtering
    int lSelected = mBayerCombo->currentIndex();
    switch ( lSelected )
    {
        case 0:
            mImageFiltering->SetBayerFilter( PvBayerFilterSimple );
            break;

        case 1:
            mImageFiltering->SetBayerFilter( PvBayerFilter3X3 );
            break;

        default:
            assert( 0 );
            break;
    }
}


///
/// \brief
///

void FilteringDlg::OnROSliderChanged( int aValue )
{
    mFilter->SetOffsetR( aValue );
    if ( mROSpin->value() != aValue )
    {
        mROSpin->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnGOSliderChanged( int aValue )
{
    mFilter->SetOffsetG( aValue );
    if ( mGOSpin->value() != aValue )
    {
        mGOSpin->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnBOSliderChanged( int aValue )
{
    mFilter->SetOffsetB( aValue );
    if ( mBOSpin->value() != aValue )
    {
        mBOSpin->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnRGSliderChanged( int aValue )
{
    double lValue = FromGainSlider( aValue );
    mFilter->SetGainR( lValue );
    if ( mRGSpin->value() != lValue )
    {
        mRGSpin->setValue( lValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnGGSliderChanged( int aValue )
{
    double lValue = FromGainSlider( aValue );
    mFilter->SetGainG( lValue );
    if ( mGGSpin->value() != lValue )
    {
        mGGSpin->setValue( lValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnBGSliderChanged( int aValue )
{
    double lValue = FromGainSlider( aValue );
    mFilter->SetGainB( lValue );
    if ( mBGSpin->value() != lValue )
    {
        mBGSpin->setValue( lValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnROSpinChanged( int aValue )
{
    mFilter->SetOffsetR( aValue );
    if ( mROSlider->value() != aValue )
    {
        mROSlider->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnGOSpinChanged( int aValue )
{
    mFilter->SetOffsetG( aValue );
    if ( mGOSlider->value() != aValue )
    {
        mGOSlider->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnBOSpinChanged( int aValue )
{
    mFilter->SetOffsetB( aValue );
    if ( mBOSlider->value() != aValue )
    {
        mBOSlider->setValue( aValue );
    }
}


///
/// \brief
///

void FilteringDlg::OnRGSpinChanged( double aValue )
{
    int lPos = ToGainSlider( mRGSlider, aValue );
    mFilter->SetGainR( aValue );
    if ( mRGSlider->value() != lPos )
    {
        mRGSlider->setValue( lPos );
    }
}


///
/// \brief
///

void FilteringDlg::OnGGSpinChanged( double aValue )
{
    int lPos = ToGainSlider( mGGSlider, aValue );
    mFilter->SetGainG( aValue );
    if ( mGGSlider->value() != lPos )
    {
        mGGSlider->setValue( lPos );
    }
}


///
/// \brief
///

void FilteringDlg::OnBGSpinChanged( double aValue )
{
    int lPos = ToGainSlider( mBGSlider, aValue );
    mFilter->SetGainB( aValue );
    if ( mBGSlider->value() != lPos )
    {
        mBGSlider->setValue( lPos );
    }
}


///
/// \brief Initializes all controls
///

void FilteringDlg::InitControls()
{
    mRGSpin->setRange( 0.0, 16.0 );
    mRGSpin->setSingleStep( 0.1 );
    mGGSpin->setRange( 0.0, 16.0 );
    mGGSpin->setSingleStep( 0.1 );
    mBGSpin->setRange( 0.0, 16.0 );
    mBGSpin->setSingleStep( 0.1 );

    mRGSlider->setRange( 0, GAIN_TICK_COUNT );
    mGGSlider->setRange( 0, GAIN_TICK_COUNT );
    mBGSlider->setRange( 0, GAIN_TICK_COUNT );

    mROSpin->setRange( -255, 255 );
    mGOSpin->setRange( -255, 255 );
    mBOSpin->setRange( -255, 255 );

    mROSlider->setRange( -255, 255 );
    mGOSlider->setRange( -255, 255 );
    mBOSlider->setRange( -255, 255 );

    ToDialog();

    mSkipSyncSpinsCount = 0;
    mSkipSyncSlidersCount = 0;
}


///
/// \brief Enables UI state
///

void FilteringDlg::EnableInterface()
{
	bool lRGBEnabled = mRGBFilterCheckBox->isChecked();

    mRGSpin->setEnabled( lRGBEnabled );
    mGGSpin->setEnabled( lRGBEnabled );
    mBGSpin->setEnabled( lRGBEnabled );
    mROSpin->setEnabled( lRGBEnabled );
    mGOSpin->setEnabled( lRGBEnabled );
    mBOSpin->setEnabled( lRGBEnabled );

    mRGSlider->setEnabled( lRGBEnabled );
    mGGSlider->setEnabled( lRGBEnabled );
    mBGSlider->setEnabled( lRGBEnabled );
    mROSlider->setEnabled( lRGBEnabled );
    mGOSlider->setEnabled( lRGBEnabled );
    mBOSlider->setEnabled( lRGBEnabled );

    mWBButton->setEnabled( lRGBEnabled );
    mResetButton->setEnabled( lRGBEnabled );

    mOffsetsLabel->setEnabled( lRGBEnabled );
    mGainsLabel->setEnabled( lRGBEnabled );
    mRedLabel->setEnabled( lRGBEnabled );
    mGreenLabel->setEnabled( lRGBEnabled );
    mBlueLabel->setEnabled( lRGBEnabled );
    
    bool lHistogramEnabled = ( mRangeFilterCheckBox->isChecked() );

    mHistogram->setEnabled( lHistogramEnabled );
    mDarkSlider->setEnabled( lHistogramEnabled );
    mLightSlider->setEnabled( lHistogramEnabled );
    mAutoRangeButton->setEnabled( lHistogramEnabled );
}


///
/// \brief Filtering enabled check box clicked handler
///

void FilteringDlg::OnBnClickedEnabledCheck()
{
    mImageFiltering->SetRGBFilterEnabled( mRGBFilterCheckBox->isChecked() );
    EnableInterface();
}


///
/// \brief Filtering enabled check box clicked handler
///

void FilteringDlg::ToDialog()
{
    // Filter RGB enabled?
    mRGBFilterCheckBox->setChecked( mImageFiltering->GetRGBFilterEnabled() );

    // Gains
    mRGSpin->setValue( mFilter->GetGainR() );
    mGGSpin->setValue( mFilter->GetGainG() );
    mBGSpin->setValue( mFilter->GetGainB() );

    // Offsets
    mROSpin->setValue( mFilter->GetOffsetR() );
    mGOSpin->setValue( mFilter->GetOffsetG() );
    mBOSpin->setValue( mFilter->GetOffsetB() );

    // Bayer filtering
    switch ( mImageFiltering->GetBayerFilter() )
    {
        case PvBayerFilterSimple:
            mBayerCombo->setCurrentIndex( 0 );
            break;

        case PvBayerFilter3X3:
            mBayerCombo->setCurrentIndex( 1 );
            break;

        default:
            assert( 0 );
            break;
    }

    // Histogram
    LoadHistogram();

    // Make sure UI is setup properly!
    EnableInterface();
}


///
/// \brief White balance clicked handler
///

void FilteringDlg::OnBnClickedWbbutton()
{
    mImageFiltering->WhiteBalance( mDisplayThread );
    ToDialog();
}


///
/// \brief Filter reset button clicked handler
///

void FilteringDlg::OnBnClickedResetbutton()
{
    mImageFiltering->ResetRGBFilter();
    ToDialog();
}


///
/// \brief Returns gain value from slider position
///

double FilteringDlg::FromGainSlider( double aValue )
{
    double lPos = PTMIN( static_cast<double>( GAIN_TICK_COUNT - 1 ), aValue );
    double lLog = log( GAIN_TICK_COUNT - lPos );
    double lValue = GAIN_MAX - lLog / cNormalizer * GAIN_MAX;

    return lValue;
}


///
/// \brief Returns slider position from gain value
///

int FilteringDlg::ToGainSlider( QSlider *aSlider, double &aValue )
{
    double lV1 = ( static_cast<double>( GAIN_MAX ) - aValue ) / static_cast<double>( GAIN_MAX ) * cNormalizer;
    double lV2 = static_cast<double>( GAIN_TICK_COUNT ) - exp( lV1 );

    int lMin = aSlider->minimum();
    int lMax = aSlider->maximum();

    int lValue = PTMAX( lMin, PTMIN( lMax, static_cast<int>( lV2 + 0.5 ) ) );

    return lValue;
}


///
/// \brief Loads the histogram data to the form 
///

void FilteringDlg::LoadHistogram()
{
    PvRangeFilter *lRangeFilter = mImageFiltering->GetRangeFilter();

    mRangeFilterCheckBox->setChecked( lRangeFilter->IsEnabled() );
    mDarkSlider->setValue( lRangeFilter->GetDark() );
    mLightSlider->setValue( lRangeFilter->GetLight() );
}


///
/// \brief 
///

void FilteringDlg::OnBnClickedRangeFilterCheck()
{
    mImageFiltering->GetRangeFilter()->SetEnabled( mRangeFilterCheckBox->isChecked() );
    EnableInterface();
}


///
/// \brief 
///

void FilteringDlg::OnBnClickedAutoRange()
{
    mImageFiltering->GetRangeFilter()->AutoConfigure();
    LoadHistogram();
}


///
/// \brief 
///

void FilteringDlg::OnDarkSliderChanged( int aValue )
{
    PvRangeFilter *lRangeFilter = mImageFiltering->GetRangeFilter();
    
    int lValue = mDarkSlider->value();
    if ( lValue != lRangeFilter->GetDark() )
    {
        lRangeFilter->SetDark( lValue );
        LoadHistogram();
    }
}


///
/// \brief 
///

void FilteringDlg::OnLightSliderChanged( int aValue )
{
    PvRangeFilter *lRangeFilter = mImageFiltering->GetRangeFilter();
    
    int lValue = mLightSlider->value();
    if ( lValue != lRangeFilter->GetLight() )
    {
        lRangeFilter->SetLight( lValue );
        LoadHistogram();
    }
}

