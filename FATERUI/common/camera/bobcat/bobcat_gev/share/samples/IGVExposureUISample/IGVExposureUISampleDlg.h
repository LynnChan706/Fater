// *****************************************************************************
//
//     Copyright (c) 2013, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IGVEXPOSUREUISAMPLEDLG_H__
#define __IGVEXPOSUREUISAMPLEDLG_H__

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
#include <PvDeviceInfoGEV.h>
#include <PvGenBrowserWnd.h>
#include <PvDisplayWnd.h>
#include <PvStreamGEV.h>
#include <PvDeviceGEV.h>
#include <PvDevice.h>
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
class IGVExposureUISampleDlg;
class PvGenStateStack;


class IGVExposureUISampleDlg : public QMainWindow,
    protected PvDeviceEventSink, protected PvGenEventSink,
	protected PvPipelineEventSink
{
    Q_OBJECT

public:

    IGVExposureUISampleDlg( QWidget *parent = 0, Qt::WFlags flags = 0 );
    virtual ~IGVExposureUISampleDlg();
    
    void StartStreaming();
    void StopStreaming();

protected slots:

    void OnBnClickedConnectButton();
    void OnBnClickedDisconnectButton();
    void OnBnClickedBtExpSet();
    void OnBnClickedBtFtmSet();
    void OnBnClickedBtFpsSet();
    void OnBnClickedChFrameTimeEnable();
    void OnBnClickedStart();
    void OnBnClickedStop();
    void OnFileExitAction(); //OnClose
    void OnCbnSelchangeMode( int aIndex );
    void OnCbnSelchangeCbExpMode(int aIndex );
    void setExpText(const QString &text);
    void setFtmText(const QString &text);
    void setFpsText(const QString &text);
    void OnBnClickedLinkButton(); //ShowCommParameters();   //OnBnClickedLinkButton()
    void OnBnClickedDeviceButton();//ShowDeviceParameters(); //OnBnClickedDeviceButton()
    void OnBnClickedStreamparamsButton();//ShowStreamParameters(); //OnBnClickedStreamparamsButton()
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

    QLineEdit *mIPEdit; //pAddressLine;
    QLineEdit *mMACEdit; //MacAddressLine;
    QLineEdit *mManufacturerEdit;//Line;
    QLineEdit *mModelEdit;
    QLineEdit *mNameEdit;
    
    QComboBox *mModeCombo;
    QPushButton *mPlayButton;
    QPushButton *mStopButton;

    QPushButton *mCommunicationButton;
    QPushButton *mDeviceButton;
    QPushButton *mStreamButton;
    PvDisplayWnd *mDisplay;

    QComboBox *m_cbExpMode;
    QCheckBox *m_chFtmCtrl;
    QLineEdit *m_edExp;
    QLineEdit *m_edFtm;
    QLineEdit *m_edFps;
    QPushButton *m_bt_exp_set;
    QPushButton *m_bt_ftm_set;
    QPushButton *m_bt_fps_set;
	
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


#endif // __IGVEXPOSUREUISAMPLEDLG_H__



#if 0

class IGVExposureUISampleDlg : public CDialog, PvGenEventSink
{
public:

    bool IsAcquiringImages() const { return mAcquiringImages; }

protected:

	HICON m_hIcon;

	afx_msg void OnBnClickedDeviceEvents();
	afx_msg void OnMove(int x, int y);


	CRect mCrt;
	BOOL mNeedInit;

    bool mAcquiringImages;

	Thread *mThreadDisplay;

	
public:

	void CreateEnumComboBox(CComboBox&, char*);
	void OnCbnSelchangeEnum(CComboBox&, char*);
	void OnBnClickedSetInteger(int ctlID, char* param);

	
	afx_msg void OnCbnSelchangeCbExpMode();
	afx_msg void OnBnClickedBtExpSet();
	afx_msg void OnBnClickedBtFtmSet();
	afx_msg void OnBnClickedBtFpsSet();
	afx_msg void OnBnClickedChFrameTimeEnable();

	
};

#endif
