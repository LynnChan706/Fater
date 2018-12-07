// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>

class PvBufferConverterRGBFilter;
class ImageFiltering;
class DisplayThread;
class HistogramWidget;


class FilteringDlg : public QDialog
{
    Q_OBJECT

public:

    FilteringDlg( ImageFiltering *aImageFiltering, DisplayThread *aDisplayThread, QWidget* aParent );
	virtual ~FilteringDlg();

    void ToDialog();

protected:

	void CreateLayout();
    QGroupBox *CreateRGBFiltering();
    QGroupBox *CreateBayer();
    QGroupBox *CreateHistogram();
    
	void InitControls();

protected slots:

    void OnBnClickedEnabledCheck();
    void OnBnClickedWbbutton();
    void OnBnClickedResetbutton();

    void OnBayerComboChanged( int aIndex );

    void OnRGSpinChanged( double aValue );
    void OnRGSliderChanged( int aValue );
    void OnGGSpinChanged( double aValue );
    void OnGGSliderChanged( int aValue );
    void OnBGSpinChanged( double aValue );
    void OnBGSliderChanged( int aValue );

    void OnROSpinChanged( int aValue );
    void OnROSliderChanged( int aValue );
    void OnGOSpinChanged( int aValue );
    void OnGOSliderChanged( int aValue );
    void OnBOSpinChanged( int aValue );
    void OnBOSliderChanged( int aValue );

    void OnBnClickedRangeFilterCheck();
    void OnBnClickedAutoRange();
    void OnDarkSliderChanged( int aValue );
    void OnLightSliderChanged( int aValue );

private:

    void EnableInterface();
    void LoadHistogram();

    double FromGainSlider( double aValue );
    int ToGainSlider( QSlider *aSlider, double &aValue );

	QCheckBox *mRGBFilterCheckBox;
    QDoubleSpinBox *mRGSpin;
    QDoubleSpinBox *mGGSpin;
    QDoubleSpinBox *mBGSpin;
    QSpinBox *mROSpin;
    QSpinBox *mGOSpin;
    QSpinBox *mBOSpin;
    QSlider *mRGSlider;
    QSlider *mGGSlider;
    QSlider *mBGSlider;
    QSlider *mROSlider;
    QSlider *mGOSlider;
    QSlider *mBOSlider;
    QPushButton *mWBButton;
    QPushButton *mResetButton;
    QComboBox *mBayerCombo;
    QLabel *mGainsLabel;
    QLabel *mOffsetsLabel;
    QLabel *mRedLabel;
    QLabel *mGreenLabel;
    QLabel *mBlueLabel;

    HistogramWidget *mHistogram;
    QCheckBox *mRangeFilterCheckBox;
    QSlider *mDarkSlider;
    QSlider *mLightSlider;
    QPushButton *mAutoRangeButton;

    int mSkipSyncSpinsCount;
    int mSkipSyncSlidersCount;

    ImageFiltering *mImageFiltering;
    PvBufferConverterRGBFilter *mFilter;
    DisplayThread *mDisplayThread;
};

