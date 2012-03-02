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

OFonoNetworkRegistration::OFonoNetworkRegistration(OFonoTelephonyService * service)
:  QNetworkRegistrationServer(service->service(), service)
    , service(service)
{
}

OFonoNetworkRegistration::~OFonoNetworkRegistration()
{
}

static QTelephony::RegistrationState ofonoRegStateToQt(QString state)
{
    if (state == "home")        // registered to home network
        return QTelephony::RegistrationHome;
    if (state == "unregistered")    // not registered, not trying
        return QTelephony::RegistrationNone;
    if (state == "busy")        // not registered, but currently trying
        return QTelephony::RegistrationSearching;
    if (state == "denied")      // no permitted network available
        return QTelephony::RegistrationDenied;
    if (state == "unknown")     // no idea
        return QTelephony::RegistrationUnknown;
    if (state == "roaming")     // registered to foreign network
        return QTelephony::RegistrationRoaming;

    qWarning() << "unknown OFONO registration state" << state;
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

void OFonoNetworkRegistration::deviceStatus(QString status)
{
    if (status == "alive-sim-ready") {
        QOFonoDBusPendingCall call = service->gsmNet.Register();
        watchOFonoCall(call, this, SLOT(registerFinished(QOFonoDBusPendingCall &)));
    } else if (status == "alive-registered") {
        QOFonoDBusPendingCall call = service->gsmNet.GetStatus();
        watchOFonoCall(call, this,
                     SLOT(getStatusFinished(QOFonoDBusPendingCall &)));
    }
}

void OFonoNetworkRegistration::registerFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply <> reply = call;
    checkReply(reply);
}

void OFonoNetworkRegistration::networkStatusChange(const QVariantMap & map)
{
    QString registration = map.value("registration").toString();
    QString mode = map.value("mode").toString();
    QString provider = map.value("provider").toString();
    QString display = map.value("display").toString();
    QString act = map.value("act").toString();

    qDebug() << "OFonoNetworkRegistration::networkStatusChange registration=" <<
        registration << ", mode=" << mode << ", provider=" << provider;

    updateInitialized(true);
    updateRegistrationState(ofonoRegStateToQt(registration));
    updateCurrentOperator(ofonoOpModeToQt(mode), provider, display, act);
}

void OFonoNetworkRegistration::getStatusFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply < QVariantMap > reply = call;
    if (checkReply(reply) && !initialized()) {
        networkStatusChange(reply.value());
    }
}

void OFonoNetworkRegistration::setCurrentOperator
    (QTelephony::OperatorMode, const QString & id, const QString &)
{
    QOFonoDBusPendingReply <> reply = service->gsmNet.RegisterWithProvider(id);
    emit setCurrentOperatorResult(qTelResult(reply));
}

void OFonoNetworkRegistration::requestAvailableOperators()
{
    QOFonoDBusPendingReply < QOFonoNetworkProviderList > reply =
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
    emit availableOperators(opers);
}
