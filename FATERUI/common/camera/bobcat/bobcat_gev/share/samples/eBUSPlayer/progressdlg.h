// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <PvDevice.h>

#include <IProgress.h>
#include <Task.h>


class TaskThread;


class ProgressDlg : public QDialog, public IProgress
{
    Q_OBJECT

public:

    ProgressDlg( QWidget* aParent );

    virtual ~ProgressDlg();

    int exec();
	virtual void paintEvent(QPaintEvent * event);

	// IProgress interface
    int RunTask( Task *aTask );
    void SetStatus( const std::string &aStatus );
    void ShowWarning( const std::string &aStatus );
    void SetTitle( const PvString &aTitle ) { mTitle = aTitle.GetAscii(); }

protected slots:

	void OnTimer();

protected:

    void CreateLayout();
    void Update();

	QLabel *mStatusLabel;
    QPixmap *mWheel;
	QTimer *mTimer;
    unsigned long mWheelIndex;

private:

    TaskThread *mThread;
    Task *mTask;

	QString mStatus;
    QString mTitle;
    QMutex mMutex;
};

