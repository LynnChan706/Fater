// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>

#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>

#include <Setup.h>


class Setup;


class SetupDlg : public QDialog
{
    Q_OBJECT

public:

    SetupDlg( Setup *aSetup, QWidget* aParent );
	virtual ~SetupDlg();

    int exec();

protected slots:

    void accept();
    void reject();
    void OnRadioClicked();

protected:

    void CreateLayout();
    QGroupBox *CreateRole();
    QGroupBox *CreateDefaultChannel();
    QGroupBox *CreateDestination();

    void LoadToUI();

    void EnableInterface();

    void IPStrToCtrl( const QString &aIPStr, QLineEdit *aCtrl );

    void SetCurrentRole( Setup::Role aRole );
    void SetCurrentDestination( Setup::Destination aDestination );

    Setup::Role GetCurrentRole();
    Setup::Destination GetCurrentDestination();

    QRadioButton *mCtrlDataRadio;
    QRadioButton *mCtrlRadio;
    QRadioButton *mDataRadio;
    QLabel *mDefaultChannelLabel;
    QLineEdit *mDefaultChannelEdit;
    QRadioButton *mUnicastSpecificRadio;
    QRadioButton *mUnicastAutoRadio;
    QRadioButton *mUnicastOtherRadio;
    QLineEdit *mUnicastSpecificPortEdit;
    QLineEdit *mUnicastIPCtrl;
    QLineEdit *mUnicastPortEdit;
    QRadioButton *mMulticastRadio;
    QLineEdit *mMulticastIPCtrl;
    QLabel *mUnicastSpecificPortLabel;
    QLabel *mUnicastIPLabel;
    QLabel *mUnicastPortLabel;
    QLineEdit *mMulticastPortEdit;
    QLabel *mMulticastIPLabel;
    QLabel *mMulticastPortLabel;
    QPushButton *mOKButton;
    QPushButton *mCancelButton;

private:

    Setup *mSetup;
};

