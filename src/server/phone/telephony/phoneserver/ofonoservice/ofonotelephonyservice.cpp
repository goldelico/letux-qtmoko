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
                                             QString modemDbusPath,
                                             QObject * parent)
:QTelephonyService(service, parent), oModem("org.ofono", modemDbusPath,
                                            QDBusConnection::systemBus(), this),
oCellBroadcast("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oConnMan("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oMessageManager("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oNetReg("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oPhoneBook("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oPushNotify("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oRadio("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oSim("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oSuplServices("org.ofono", modemDbusPath, QDBusConnection::systemBus(), this),
oVoiceCallManager("org.ofono", modemDbusPath, QDBusConnection::systemBus(),
                  this)
, service_checker(this)
, rf_functionality(this)
, network_registration(this)
, suppl_services(this)
, call_provider(this)
, sms_sender(this)
, sms_reader(this)
, phone_book(this)
, sim_info(this)
, modemProperties()
, netRegProperties()
{
    connect(&oModem, SIGNAL(PropertyChanged(const QString, const QDBusVariant)),
            this,
            SLOT(modemPropertyChanged(const QString, const QDBusVariant)));

    connect(&oNetReg,
            SIGNAL(PropertyChanged(const QString, const QDBusVariant)), this,
            SLOT(netRegPropertyChanged(const QString, const QDBusVariant)));

    QOFonoDBusPendingReply < QVariantMap > reply = oModem.GetProperties();
    if (!checkReply(reply, true)) {
        return;
    }
    QVariantMap properties = modemProperties = reply.value();

    // Notfify all interface about initial properties
    QStringList keys = properties.keys();
    for (int i = 0; i < keys.count(); i++) {
        QString name = keys.at(i);
        QVariant value = properties.value(name);
        QDBusVariant dbusValue(value);
        modemPropertyChanged(name, dbusValue);
    }

    // Is modem powered?
    bool powered = properties.value("Powered").toBool();
    if (powered) {
        return;
    }
    // Power it on if not
    QOFonoDBusPendingCall call =
        oModem.SetProperty("Powered", QDBusVariant(true));
    watchOFonoCall(call, this, SLOT(poweredFinished(QOFonoDBusPendingCall &)));
}

OFonoTelephonyService::~OFonoTelephonyService()
{
}

void OFonoTelephonyService::initialize()
{
    // Create our oFono-specific overrides for the service interfaces

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

void OFonoTelephonyService::poweredFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply <> reply = call;
    if (!checkReply(reply)) {
        return;
    }
}

// Returns false if interface is not yet available (e.g. netReg interface is
// not available if modem is not powered or online). Returns true if the
// interface is available and ready to use.
bool OFonoTelephonyService::interfaceAvailable(QOFonoDbusAbstractInterface *
                                               interface)
{
    QStringList interfaces = modemProperties["Interfaces"].toStringList();
    return interfaces.contains(interface->interface());
}

void OFonoTelephonyService::modemPropertyChanged(const QString & name,
                                                 const QDBusVariant & value)
{
    qDebug() << "modemPropertyChanged " << name << "=" << value.variant();
    modemProperties[name] = value.variant();

    rf_functionality.modemPropertyChanged(name, value);
    network_registration.modemPropertyChanged(name, value);
}

void OFonoTelephonyService::netRegPropertyChanged(const QString & name,
                                                  const QDBusVariant & value)
{
    qDebug() << "netRegPropertyChanged " << name << "=" << value.variant();
    netRegProperties[name] = value.variant();

    network_registration.netRegPropertyChanged(name, value);
}
