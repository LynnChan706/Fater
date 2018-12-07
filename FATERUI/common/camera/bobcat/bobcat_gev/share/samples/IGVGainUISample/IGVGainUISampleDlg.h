// *****************************************************************************
//
//     Copyright (c) 2014, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IGVGAINUISAMPLEDLG_H__
#define __IGVGAINUISAMPLEDLG_H__

#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QCheckBox>
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

#pragma once

#define WM_UPDATEACQUISITIONMODE ( QEvent::User + 0x4441 )

#define IDC_ED_EXP_VALUE 1


class DisplayThread;

// forward declare (for now)
class IGVGainUISampleDlg;
class PvGenStateStack;


class IGVGainUISampleDlg : public QMainWindow,
    protected PvDeviceEventSink, protected PvGenEventSink,
	protected PvPipelineEventSink
{
    Q_OBJECT

public:

    IGVGainUISampleDlg( QWidget *parent = 0, Qt::WFlags flags = 0 );
    virtual ~IGVGainUISampleDlg();
    
    void StartStreaming();
    void StopStreaming();

protected slots:

    void OnBnClickedConnectButton();
    void OnBnClickedDisconnectButton();

    void OnBnClickedStart();
    void OnBnClickedStop();
    void OnFileExitAction(); //OnClose
    void OnCbnSelchangeMode( int aIndex );

    void OnBnClickedSetGain();
    void OnBnClickedSetOffset();

    void OnCbnSelchangeCbGainTapN(int aIndex );
    void OnCbnSelchangeCbOffsetTapN(int aIndex);
    void OnCbnSelchangeCbPreampTapN(int aIndex);
    void OnCbnSelchangeCbPreampVal(int aIndex);
    void OnCbnSelchangeCbTapBalance(int aIndex);
    void setEditGainText(const QString &text);
    void setEditOffsetText(const QString &text);
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
    QGroupBox *CreateGain();
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

    QComboBox *m_cbGainSel;
    QComboBox *m_cbOffsetSel;
    QComboBox *m_cbPreAmpSel;
    QComboBox *m_cbPreAmp;
    QComboBox *m_cbTapBalance;
    QLineEdit *m_edGain;
    QLineEdit *m_edOffset;
    QPushButton *m_bt_ed_gain_set;
    QPushButton *m_bt_ed_offset_set;

    QAction *mFileExitAction;


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


#endif // __IGVGAINUISAMPLEDLG_H__


