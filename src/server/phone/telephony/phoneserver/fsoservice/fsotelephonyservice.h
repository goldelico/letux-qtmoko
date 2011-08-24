/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#ifndef FSOMODEMSERVICE_H
#define FSOMODEMSERVICE_H

#include <qmodemservice.h>
#include <qmodemcall.h>
#include <qmodemcallprovider.h>
#include <qmodemsimtoolkit.h>
#include <qbandselection.h>
#include <qfsogsmnetwork.h>
#include <qfsogsmcall.h>
#include <qfsogsmdevice.h>
#include <qfsogsmsms.h>
#include <qfsopimmessages.h>
#include <qfsopimmessagequery.h>

#include "fsophonecall.h"
#include "fsocallprovider.h"
#include "fsonetworkregistration.h"
#include "fsosupplementaryservices.h"
#include "fsoservicechecker.h"
#include "fsorffunctionality.h"
#include "fsosmssender.h"
#include "fsosmsreader.h"
#include "fsophonebook.h"

class FsoCallProvider;

class FsoTelephonyService : public QTelephonyService
{
    Q_OBJECT
public:
    FsoTelephonyService
        ( const QString& service, QObject *parent = 0 );
    ~FsoTelephonyService();

    QFsoGSMDevice gsmDev;
    QFsoGSMNetwork gsmNet;
    QFsoGSMCall gsmCall;
    QFsoGSMSMS gsmSms;
    QFsoPIMMessages pimMsg;

    FsoServiceChecker service_checker;
    FsoRfFunctionality rf_functionality;
    FsoNetworkRegistration network_registration;
    FsoSupplementaryServices suppl_services;
    FsoCallProvider call_provider;
    FsoSMSSender sms_sender;
    FsoSMSReader sms_reader;

    void initialize();
    
private slots:
    void getDeviceStatusFinished(QFsoDBusPendingCall &);
    void callStatusChange(int id, const QString &status, const QVariantMap &properties);
    void incomingUssd(const QString &mode, const QString &message);
    void incomingTextMessage(const QString &number, const QString &timestamp, const QString &contents);
    void incomingMessageReport(int reference, const QString &status, const QString &sender_number, const QString &contents);
    void incomingMessage(const QString &path);
};

#endif

