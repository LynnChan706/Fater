// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <map>

#include <QtGui/QDialog>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include <LogBuffer.h>


class EventMonitorDlg : public QDialog
{
    Q_OBJECT

public:

	EventMonitorDlg( QWidget *aParent, LogBuffer *aLogBuffer );
	virtual ~EventMonitorDlg();

	void LoadData();

protected slots:

	void OnBnClickedSaveLogCheckBox();
	void OnBnClickedSaveLogButton();
	void OnBnClickedClearButton();
	void OnBnClickedGenICamCheckBox();
	void OnBnClickedEventsCheckBox();
	void OnBnClickedBufferAllCheckBox();
	void OnBnClickedBufferErrorCheckBox();
	void OnTimer();

protected:

	void CreateLayout();
	void EnableInterface();

private:

	QCheckBox* mSaveLogCheckBox;
	QLineEdit* mSaveLogLineEdit;
	QPushButton* mSaveLogButton;
	QTextEdit* mLogEdit;
	QPushButton* mClearLogButton;
	QCheckBox* mGenICamCheckBox;
	QCheckBox* mEventsCheckBox;
	QCheckBox* mBufferAllCheckBox;
	QCheckBox* mBufferErrorCheckBox;

	QTimer *mTimer;

	int mLogEditTextLength;
	int mLastUpdateTime;

	bool mNeedInit;

    LogBuffer *mLogBuffer;

};

