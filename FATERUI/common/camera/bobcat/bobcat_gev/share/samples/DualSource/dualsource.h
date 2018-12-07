// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __DUALSOURCE_H__
#define __DUALSOURCE_H__

#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtCore/QMutex>
#include <QtCore/QEvent>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <PvDeviceInfoGEV.h>
#include <PvDeviceGEV.h>


class Source;
class SourceWidget;
class PvGenBrowserWnd;



class DualSource : public QMainWindow
{
    Q_OBJECT

public:

    DualSource( QWidget *parent = 0, Qt::WFlags flags = 0 );
    virtual ~DualSource();
    
protected slots:

	void OnFileMenuAboutToShow();
    void OnFileExitAction();

	void OnConnectionMenuAboutToShow();
    void OnConnectionConnectAction();
    void OnConnectionDisconnectAction();

    void OnParametersMenuAboutToShow();
    void OnParametersCommunicationAction();
    void OnParametersDeviceAction();
    void OnParametersStream1Action();
    void OnParametersStream2Action();

protected:

    void EnableInterface();

    void ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aArray, const QString &aTitle );
    void CloseGenWindow( PvGenBrowserWnd *aWnd );

    void Connect( const PvDeviceInfoGEV *aDeviceInfo );
    void Disconnect();

    // UI
    void CreateLayout();
    void CreateMenu();

private:

    QMenu *mFileMenu;
    QAction *mFileExitAction;

    QMenu *mConnectionMenu;
    QAction *mConnectionConnectAction;
    QAction *mConnectionDisconnectAction;

    QMenu *mParametersMenu;
    QAction *mParametersCommunicationAction;
    QAction *mParametersDeviceAction;
    QAction *mParametersStream1Action;
    QAction *mParametersStream2Action;

    SourceWidget *mSource1Widget;
    SourceWidget *mSource2Widget;

    Source *mSource1;
    Source *mSource2;

    PvDeviceGEV mDevice;

	PvGenBrowserWnd *mCommunicationBrowser;
    PvGenBrowserWnd *mDeviceBrowser;
    PvGenBrowserWnd *Stream1Browser;
    PvGenBrowserWnd *Stream2Browser;
};


#endif // __DUALSOURCE_H__


