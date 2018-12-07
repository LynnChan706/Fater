// *****************************************************************************
//
//     Copyright (c) 2014, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IGVTRUESENSEUISAMPLEDLG_H__
#define __IGVTRUESENSEUISAMPLEDLG_H__

#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QRadioButton>
#include <QtCore/QMutex>
#include <QtCore/QEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <PvGenBrowserWnd.h>
#include <PvDisplayWnd.h>
#include <PvDeviceInfoGEV.h>
#include <PvDeviceGEV.h>
#include <PvDevice.h>
#include <PvStreamGEV.h>
#include <PvStream.h>
#include <PvPipeline.h>

#include <PvDeviceFinderWnd.h>
#include <PvGenParameter.h>
#include <PvBuffer.h>
#include <TrueSenseConverter.h>
#pragma once

#define WM_UPDATEACQUISITIONMODE ( QEvent::User + 0x4441 )

#define IDC_ED_EXP_VALUE 1


class DisplayThread;

// forward declare (for now)
class IGVTrueSenseUISampleDlg;
class PvGenStateStack;


class IGVTrueSenseUISampleDlg : public QMainWindow,
    protected PvDeviceEventSink, protected PvGenEventSink,
	protected PvPipelineEventSink
{
    Q_OBJECT

public:

    IGVTrueSenseUISampleDlg( QWidget *parent = 0, Qt::WFlags flags = 0 );
    virtual ~IGVTrueSenseUISampleDlg();
    
    void StartStreaming();
    void StopStreaming();

protected slots:

    void OnBnClickedConnectButton();
    void OnBnClickedDisconnectButton();

    void OnBnClickedStart();
    void OnBnClickedStop();
    void OnFileExitAction();
    void OnCbnSelchangeMode( int aIndex );
    void OnBnClickedTsPattern();

	void OnBnClickedTrueSense();

    void OnCbnSelchangeCbMethod(int aIndex);
    void OnBnClickedLinkButton(); 
    void OnBnClickedDeviceButton();
    void OnBnClickedStreamparamsButton();
    void OnRegisterInterface();
    void CreateEnumComboBox(QComboBox* paramComboBox, char* paramName);

protected:

    // UI
    void CreateLayout();
    QGroupBox *CreateConnectGroup();
    QGroupBox *CreateAcquisition();
    void CreateGain();
    QGroupBox *CreateParameters();
    QGroupBox *CreateDisplay();
    void CreateMenu();

    bool event( QEvent *aEvent );

    void ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aParams, const QString &aTitle );
    void CloseGenWindow( PvGenBrowserWnd *aWnd );

    void OnBnClickedSetInteger(int ctlID, char* param);

    void EnableInterface();
    void EnableControls( bool aEnabled );
    void EnableGenBrowsers( bool aEnabled );

    // Core
    void Connect( PvDeviceInfoGEV *aDI);
    void Disconnect();
    void StartAcquisition();
    void StopAcquisition();
	
    uint32_t GetPayloadSize();

    // PvGenEventSink implementation
    virtual void OnParameterUpdate( PvGenParameter *aParameter );

    // PvDeviceEventSink implementation
    // Events used to bring async code back in main thread context
    void OnUpdateAcquisitionModeEvent();

    QString GetErrorString();

    void SendEventIfPossible( int aMsg );

private:

    bool mNeedInit;
    // UI
    QPushButton *mConnectButton;
    QPushButton *mDisconnectButton;

    QLineEdit *mIPEdit;
    QLineEdit *mMACEdit;
    QLineEdit *mManufacturerEdit;
    QLineEdit *mModelEdit;
    QLineEdit *mNameEdit;
    QComboBox *mModeCombo;
    QPushButton *mPlayButton;
    QPushButton *mStopButton;

    QPushButton *mCommunicationButton;
    QPushButton *mDeviceButton;
    QPushButton *mStreamButton;
    PvDisplayWnd *mDisplay;
	TrueSenseConverter *mTrueSenseConv;
    QComboBox *m_cbMethod;

	QRadioButton *mIDC_TS_PAT_11;
	QRadioButton *mIDC_TS_PAT_12;
	QRadioButton *mIDC_TS_PAT_13;
	QRadioButton *mIDC_TS_PAT_14;
	QRadioButton *mIDC_TS_PAT_21;
	QRadioButton *mIDC_TS_PAT_22;
	QRadioButton *mIDC_TS_PAT_23;
	QRadioButton *mIDC_TS_PAT_24;
	QRadioButton *mIDC_TS_PAT_31;
	QRadioButton *mIDC_TS_PAT_32;
	QRadioButton *mIDC_TS_PAT_33;
	QRadioButton *mIDC_TS_PAT_34;
	QRadioButton *mIDC_TS_PAT_41;  
	QRadioButton *mIDC_TS_PAT_42;
	QRadioButton *mIDC_TS_PAT_43;
	QRadioButton *mIDC_TS_PAT_44;  
	
    QAction *mFileExitAction;

    QGroupBox *m_GainBox;

    // Core
	PvDeviceGEV mDevice;
    PvStreamGEV *mStream;
    PvPipeline *mPipeline;

    DisplayThread *mDisplayThread;

	QMutex mStartAcquisitionMutex;
	QMutex mStartStreamingMutex;

    bool mDeviceConnected;

    bool mAcquiringImages;

	PvGenBrowserWnd *mDeviceWnd;
	PvGenBrowserWnd *mCommunicationWnd;
	PvGenBrowserWnd *mStreamParametersWnd;

};


#endif // __IGVTRUESENSEUISAMPLEDLG_H__


