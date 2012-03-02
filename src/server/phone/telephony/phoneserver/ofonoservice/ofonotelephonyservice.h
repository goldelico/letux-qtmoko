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

#ifndef OFONOMODEMSERVICE_H
#define OFONOMODEMSERVICE_H

#include <qmodemservice.h>
#include <qmodemcall.h>
#include <qmodemcallprovider.h>
#include <qmodemsimtoolkit.h>
#include <qbandselection.h>
#include <qofonogsmnetwork.h>
#include <qofonogsmcall.h>
#include <qofonogsmdevice.h>
#include <qofonogsmsms.h>
#include <qofonopimmessages.h>
#include <qofonopimmessagequery.h>
#include <qofonopimcontact.h>
#include <qofonopimcontacts.h>
#include <qofonopimcontactquery.h>
#include <qofonogsmsim.h>
#include <qofonousage.h>

#include "ofonophonecall.h"
#include "ofonocallprovider.h"
#include "ofononetworkregistration.h"
#include "ofonosupplementaryservices.h"
#include "ofonoservicechecker.h"
#include "ofonorffunctionality.h"
#include "ofonosmssender.h"
#include "ofonosmsreader.h"
#include "ofonophonebook.h"
#include "ofonosiminfo.h"

class OFonoCallProvider;

class OFonoTelephonyService : public QTelephonyService
{
    Q_OBJECT
public:
    OFonoTelephonyService(const QString & service, QObject * parent = 0);
    ~OFonoTelephonyService();

    QOFonoGSMDevice gsmDev;
    QOFonoGSMNetwork gsmNet;
    QOFonoGSMCall gsmCall;
    QOFonoGSMSMS gsmSms;
    QOFonoGSMSIM gsmSim;
    QOFonoPIMMessages pimMsg;
    QOFonoPIMContacts pimContacts;
    QOFonoUsage ofonoUsage;

    OFonoServiceChecker service_checker;
    OFonoRfFunctionality rf_functionality;
    OFonoNetworkRegistration network_registration;
    OFonoSupplementaryServices suppl_services;
    OFonoCallProvider call_provider;
    OFonoSMSSender sms_sender;
    OFonoSMSReader sms_reader;
    OFonoPhoneBook phone_book;
    OFonoSimInfo sim_info;

    bool deviceStatusInitialized;

    void initialize();

private slots:
    void getDeviceStatusFinished(QOFonoDBusPendingCall &);
    void setResourcePolicyFinished(QOFonoDBusPendingCall &);
    void deviceStatusChange(const QString & status);
    void networkStatusChange(const QVariantMap &);
    void callStatusChange(int id, const QString & status,
                          const QVariantMap & properties);
    void incomingUssd(const QString & mode, const QString & message);
    void incomingTextMessage(const QString & number, const QString & timestamp,
                             const QString & contents);
    void incomingMessageReport(int reference, const QString & status,
                               const QString & sender_number,
                               const QString & contents);
};

#endif
