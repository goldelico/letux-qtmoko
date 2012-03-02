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
#include "ofonotelephonyservice.h"

OFonoTelephonyService::OFonoTelephonyService(const QString & service,
                                         QObject * parent)
:QTelephonyService(service, parent)
, gsmDev("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device",
         QDBusConnection::systemBus(), this)
, gsmNet("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device",
         QDBusConnection::systemBus(), this)
, gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device",
          QDBusConnection::systemBus(), this)
, gsmSms("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device",
         QDBusConnection::systemBus(), this)
, gsmSim("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device",
         QDBusConnection::systemBus(), this)
, pimMsg("org.freesmartphone.opimd", "/org/freesmartphone/PIM/Messages",
         QDBusConnection::systemBus(), this)
, pimContacts("org.freesmartphone.opimd", "/org/freesmartphone/PIM/Contacts",
              QDBusConnection::systemBus(), this)
, ofonoUsage("org.freesmartphone.ousaged", "/org/freesmartphone/Usage",
           QDBusConnection::systemBus(), this)
, service_checker(this)
, rf_functionality(this)
, network_registration(this)
, suppl_services(this)
, call_provider(this)
, sms_sender(this)
, sms_reader(this)
, phone_book(this)
, sim_info(this)
, deviceStatusInitialized(false)
{
    connect(&gsmDev,
            SIGNAL(DeviceStatus(const QString &)),
            this, SLOT(deviceStatusChange(const QString &)));

    connect(&gsmNet,
            SIGNAL(Status(const QVariantMap &)),
            this, SLOT(networkStatusChange(const QVariantMap &)));

    connect(&gsmCall,
            SIGNAL(CallStatus(int, const QString &, const QVariantMap &)),
            this,
            SLOT(callStatusChange(int, const QString &, const QVariantMap &)));

    connect(&gsmNet,
            SIGNAL(IncomingUssd(const QString &, const QString &)),
            this, SLOT(incomingUssd(const QString &, const QString &)));

    connect(&gsmSms,
            SIGNAL(IncomingTextMessage
                   (const QString &, const QString &, const QString &)), this,
            SLOT(incomingTextMessage
                 (const QString &, const QString &, const QString &)));

    connect(&gsmSms,
            SIGNAL(IncomingMessageReport
                   (int, const QString &, const QString &, const QString &)),
            this,
            SLOT(incomingMessageReport
                 (int, const QString &, const QString &, const QString &)));

    QOFonoDBusPendingCall call = gsmDev.GetDeviceStatus();
    watchOFonoCall(call, this,
                 SLOT(getDeviceStatusFinished(QOFonoDBusPendingCall &)));
}

OFonoTelephonyService::~OFonoTelephonyService()
{
}

void OFonoTelephonyService::initialize()
{
    // Create our OFono-specific overrides for the service interfaces

    // If the modem does not exist, then tell clients via QServiceChecker.
    if (!supports < QServiceChecker > ())
        addInterface(&service_checker);

    if (!supports < QPhoneRfFunctionality > ())
        addInterface(&rf_functionality);

    if (!supports < QNetworkRegistration > ())
        addInterface(&network_registration);

    if (!supports < QSupplementaryServices > ())
        addInterface(&suppl_services);

    if (!supports < QSMSSender > ())
        addInterface(&sms_sender);

    if (!supports < QSMSReader > ())
        addInterface(&sms_reader);

    if (!supports < QSimInfo > ())
        addInterface(&sim_info);

    /*
       if ( !supports<QSimToolkit>() )
       addInterface( new OFonoSimToolkit( this ) );

       if ( !supports<QTelephonyConfiguration>() )
       addInterface( new OFonoConfiguration( this ) );
     */

    if (!supports < QPhoneBook > ())
        addInterface(&phone_book);

    if (!callProvider())
        setCallProvider(&call_provider);

    // Call QModemService to create other interfaces that we didn't override.
    //QModemService::initialize();
}

void OFonoTelephonyService::deviceStatusChange(const QString & status)
{
    deviceStatusInitialized = true;
    rf_functionality.deviceStatus(status);
    network_registration.deviceStatus(status);
    sim_info.deviceStatus(status);
    sms_reader.deviceStatus(status);
    phone_book.deviceStatus(status);
}

void OFonoTelephonyService::getDeviceStatusFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply < QString > reply = call;
    if (!checkReply(reply)) {
        return;
    }
    QString status = reply.value();
    if (!deviceStatusInitialized) {
        deviceStatusChange(reply.value());
    }
    if (status == "closed") {
        QOFonoDBusPendingCall call = ofonoUsage.SetResourcePolicy("GSM", "enabled");
        watchOFonoCall(call, this,
                     SLOT(setResourcePolicyFinished(QOFonoDBusPendingCall &)));
    }
}

void OFonoTelephonyService::setResourcePolicyFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply <> reply = call;
    checkReply(reply);
}

void OFonoTelephonyService::networkStatusChange(const QVariantMap & status)
{
    network_registration.networkStatusChange(status);
}

void OFonoTelephonyService::callStatusChange(int id, const QString & status,
                                           const QVariantMap & properties)
{
    QString str = QString("id=%1, status=%2").arg(id).arg(status);
    for (int i = 0; i < properties.count(); i++) {
        QString key = properties.keys().at(i);
        str += ", " + key + "=" + properties.value(key).toString();
    }
    qDebug() << "CallStatusChange" << str;

    if (status == "INCOMING") {
        // Incoming calls have to be created here
        OFonoPhoneCall *call = new OFonoPhoneCall(this, NULL, "Voice", id);
        call->setNumber(properties.value("peer").toString());
        call->setOFonoStatus(status);
        return;
    }
    // Check if it is an existing call
    QList < QPhoneCallImpl * >list = call_provider.calls();
    for (int i = 0; i < list.count(); i++) {
        OFonoPhoneCall *call = static_cast < OFonoPhoneCall * >(list.at(i));
        qDebug() << "call identifier=" << call->identifier() << ", call id=" <<
            call->id;
        if (call->id == id) {
            call->setOFonoStatus(status);
            return;
        }
    }

    if (status == "RELEASE") {
        return;                 // Released call can have id -1, it was not found in the loop but that's ok
    }

    qWarning() << "callStatusChange: unhandled status for new call" << status;
}

void OFonoTelephonyService::incomingUssd(const QString & mode,
                                       const QString & message)
{
    qDebug() << "Incomming ussd", "mode=" + mode + ", message=" + message;
    suppl_services.onIncomingUssd(mode, message);
}

void OFonoTelephonyService::incomingTextMessage(const QString & number,
                                              const QString & timestamp,
                                              const QString & contents)
{
    qDebug() << "incomingTextMessage number=" << number
        << " timestamp=" << timestamp << " contents=" << contents;
    sms_reader.incomingTextMessage(number, timestamp, contents);
}

void OFonoTelephonyService::incomingMessageReport(int reference,
                                                const QString & status,
                                                const QString & sender_number,
                                                const QString & contents)
{
    qDebug() << "incomingMessageReport reference=" << reference
        << " status=" << status << " sender_number=" << sender_number
        << " contents=" << contents;
}
