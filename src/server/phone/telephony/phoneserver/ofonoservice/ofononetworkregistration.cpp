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

#include "ofononetworkregistration.h"
#include "ofonotelephonyservice.h"
#include "ofonoutil.h"

OFonoNetworkRegistration::OFonoNetworkRegistration(OFonoTelephonyService *
                                                   service)
:  QNetworkRegistrationServer(service->service(), service)
    , service(service)
{
}

OFonoNetworkRegistration::~OFonoNetworkRegistration()
{
}

static QTelephony::RegistrationState ofonoStatusToQt(QString status)
{
    if (status == "registered") // Registered to home network
        return QTelephony::RegistrationHome;
    if (status == "unregistered")   // Not registered to any network
        return QTelephony::RegistrationNone;
    if (status == "searching")  // Not registered, but searching
        return QTelephony::RegistrationSearching;
    if (status == "denied")     // Registration has been denied
        return QTelephony::RegistrationDenied;
    if (status == "unknown")    // Status is unknown
        return QTelephony::RegistrationUnknown;
    if (status == "roaming")    // Registered, but roaming
        return QTelephony::RegistrationRoaming;

    qWarning() << "unknown oFono registration status" << status;
    return QTelephony::RegistrationNone;
}

static QTelephony::OperatorMode ofonoOpModeToQt(QString mode)
{
    if (mode == "automatic")    // automatic selection
        return QTelephony::OperatorModeAutomatic;
    if (mode == "manual")       // manual selection
        return QTelephony::OperatorModeManual;
    if (mode == "manual;automatic") // manual first, then automatic,
        return QTelephony::OperatorModeManualAutomatic;
    if (mode == "unregister")   // manual unregister
        return QTelephony::OperatorModeDeregister;

    qWarning() << "unknown OFONO operator mode " << mode;
    return QTelephony::OperatorModeAutomatic;
}

static QTelephony::OperatorAvailability ofonoOpStatusToQt(QString status)
{
    if (status == "available")
        return QTelephony::OperatorAvailable;
    if (status == "current")
        return QTelephony::OperatorCurrent;
    if (status == "forbidden")
        return QTelephony::OperatorForbidden;

    qWarning() << "unknown OFONO operator status " << status;
    return QTelephony::OperatorUnavailable;
}

void OFonoNetworkRegistration::modemPropertyChanged(const QString & name,
                                                    const QDBusVariant & value)
{
    if (name != "Online" || !value.variant().toBool()) {
        return;
    }

    QOFonoDBusPendingReply < QVariantMap > reply =
        service->oNetReg.GetProperties();
    if (!checkReply(reply)) {
        return;
    }
    QVariantMap properties = reply.value();

    // Notfify about initial properties
    QStringList keys = properties.keys();
    for (int i = 0; i < keys.count(); i++) {
        QString name = keys.at(i);
        QVariant value = properties.value(name);
        QDBusVariant dbusValue(value);
        netRegPropertyChanged(name, dbusValue);
    }

    // Register to network
    QOFonoDBusPendingCall call = service->oNetReg.Register();
    watchOFonoCall(call, this, SLOT(registerFinished(QOFonoDBusPendingCall &)));
}

void OFonoNetworkRegistration::registerFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply <> reply = call;
    checkReply(reply);
}

void OFonoNetworkRegistration::netRegPropertyChanged(const QString & name,
                                                     const QDBusVariant & value)
{
    qDebug() << "netRegPropertyChanged " << name << "=" << value.variant();

    QString registration = "home";
    QString mode = "automatic";
    QString provider = "t-mobile";
    QString display = "t-mobile";
    QString act = "act";

    updateInitialized(true);

    if (name == "Status") {
        updateRegistrationState(ofonoStatusToQt(value.variant().toString()));
        return;
    }
    if (name == "Name") {
        updateCurrentOperator(ofonoOpModeToQt(mode), value.variant().toString(),
                              display, act);
        return;
    }
}

void OFonoNetworkRegistration::setCurrentOperator
    (QTelephony::OperatorMode, const QString & id, const QString &)
{
/*    QOFonoDBusPendingReply <> reply = service->gsmNet.RegisterWithProvider(id);
    emit setCurrentOperatorResult(qTelResult(reply));*/
}

void OFonoNetworkRegistration::requestAvailableOperators()
{
    /*QOFonoDBusPendingReply < QOFonoNetworkProviderList > reply =
       service->gsmNet.ListProviders();
       if (!checkReply(reply)) {
       return;
       }
       QOFonoNetworkProviderList list = reply.value();
       QList < QNetworkRegistration::AvailableOperator > opers;
       for (int i = 0; i < list.count(); i++) {
       QOFonoNetworkProvider provider = list.at(i);
       QNetworkRegistration::AvailableOperator oper;

       oper.availability = ofonoOpStatusToQt(provider.status);
       oper.name = provider.longname;
       oper.id = provider.shortname;
       oper.technology = provider.act;
       opers.append(oper);
       }
       emit availableOperators(opers); */
}
