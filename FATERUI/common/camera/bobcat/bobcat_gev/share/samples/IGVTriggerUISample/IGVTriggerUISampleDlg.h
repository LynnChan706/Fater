// *****************************************************************************
//
//     Copyright (c) 2013, Imperx Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __IGVTRIGGERUISAMPLEDLG_H__
#define __IGVTRIGGERUISAMPLEDLG_H__

#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtCore/QMutex>
#include <QtCore/QEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <PvDeviceInfoGEV.h>
#include <PvGenBrowserWnd.h>
#include <PvDisplayWnd.h>

#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvPipeline.h>

#define WM_UPDATEACQUISITIONMODE ( QEvent::User + 0x4441 )

class DisplayThread;

// forward declare (for now)
class IGVTriggerUISampleDlg;
class PvGenStateStack;


class IGVTriggerUISampleDlg : public QMainWindow,
    protected PvDeviceEventSink, protected PvGenEventSink,
	protected PvPipelineEventSink
{
    Q_OBJECT

public:

    IGVTriggerUISampleDlg( QWidget *parent = 0, Qt::WFlags flags = 0 );
    virtual ~IGVTriggerUISampleDlg();
    
    void StartStreaming();
    void StopStreaming();

protected slots:

    void OnConnectButton();
    void OnDisconnectButton();
    void OnStart();
    void OnStop();
    void OnFileExitAction();
    void OnCbnSelchangeMode( int aIndex );
    void OnCbnSelchangeTriggerMode(int aIndex);
    void OnCbnSelchangeTriggerSource(int aIndex);
    void OnCbnSelchangeTriggerType(int aIndex);
    void OnCbnSelchangeTriggerActivation(int aIndex);
    void OnCbnSelchangeTriggerDebounce(int aIndex);
    void OnBnClickedTriggerSwCommand();
    void OnShowCommParameters();
    void OnShowDeviceParameters();
    void OnShowStreamParameters();
    void OnRegisterInterface();
    void CreateEnumComboBox(QComboBox* paramComboBox, char* paramName);

protected:

    // UI
    void CreateLayout();
    QGroupBox *CreateConnectGroup();
    QGroupBox *CreateAcquisition();
    QGroupBox *CreateTrigger();
    QGroupBox *CreateParameters();
    QGroupBox *CreateDisplay();
    void CreateMenu();

    bool event( QEvent *aEvent );

    void ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aArray, const QString &aTitle );
    void CloseGenWindow( PvGenBrowserWnd *aWnd );

    void EnableInterface();
    void EnableControls( bool aEnabled );
    void EnableGenBrowsers( bool aEnabled );

    // Core
    void Connect( PvDeviceInfoGEV *aDI);
    void StartAcquisition();
    void StopAcquisition();
	void Disconnect();
    uint32_t GetPayloadSize();

    // PvGenEventSink implementation
    void OnParameterUpdate( PvGenParameter *aParameter );

    // PvDeviceEventSink implementation
    // Events used to bring async code back in main thread context
    void OnUpdateAcquisitionModeEvent();

    QString GetErrorString();

    void SendEventIfPossible( int aMsg );

private:

    // UI
    QPushButton *mConnectButton;
    QPushButton *mDisconnectButton;

    QLineEdit *mIpAddressLine;
    QLineEdit *mMacAddressLine;
    QLineEdit *mManufacturerLine;
    QLineEdit *mModelLine;
    QLineEdit *mNameLine;
    QComboBox *mModeCombo;

    QPushButton *mStartButton;
    QPushButton *mStopButton;
    QPushButton *mCommunicationButton;
    QPushButton *mDeviceButton;
    QPushButton *mStreamButton;
    PvDisplayWnd *mDisplay;

    QComboBox *mTriggerMode;
    QComboBox *mTriggerSource;
    QComboBox *mTriggerType;
    QComboBox *mTriggerActivation;
    QComboBox *mTriggerDebounce;
    QPushButton *mFlashSWTrigger;

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
	PvGenBrowserWnd *mStreamWnd;

};


#endif // __IGVTRIGGERUISAMPLEDLG_H__

