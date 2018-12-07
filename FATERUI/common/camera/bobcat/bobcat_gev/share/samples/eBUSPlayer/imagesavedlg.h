// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtCore/QTime>

#include <ImageSaving.h>


class ImageSaveDlg  : public QDialog
{
    Q_OBJECT

public:

	ImageSaveDlg( ImageSaving *aImageSaving, QWidget* aParent );
	virtual ~ImageSaveDlg();

    void EnableInterface();

    int exec();

protected:

	QGroupBox *CreateFormatBox();
	QGroupBox *CreateLocationBox();
	QGroupBox *CreateThrottlingBox();
	void CreateLayout();

    void ToDialog();
    bool FromDialog();

protected slots:

	void accept();
	void reject();
	void OnRadioClicked();
	void OnBnClickedLocationButton();

private:

    QLineEdit *mOneOutOfEdit;
    QLineEdit *mMaxRateEdit;
    QLineEdit *mAverageThroughputEdit;
    QLineEdit *mSavePathEdit;
    QCheckBox *mSaveEnabledCheck;

    QPushButton *mLocationButton;
    QPushButton *mOKButton;
    QPushButton *mCancelButton;
    QRadioButton *mOneOutOfRadio;
    QRadioButton *mMaxRateRadio;
    QRadioButton *mAverageThroughputRadio;
    QRadioButton *mNoThrottleRadio;
    QLabel *mCapturedImagesLabel;
    QLabel *mMsLabel;
    QLabel *mAverageLabel;
    QComboBox *mFormatCombo;

    ImageSaving *mImageSaving;

};
