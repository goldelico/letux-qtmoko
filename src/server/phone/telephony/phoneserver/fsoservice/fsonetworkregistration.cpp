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

#include "fsonetworkregistration.h"
#include "fsotelephonyservice.h"
#include "fsoutil.h"

FsoNetworkRegistration::FsoNetworkRegistration(FsoTelephonyService *service)
    : QNetworkRegistrationServer( service->service(), service )
    , service(service)
{
    QFsoDBusPendingCall call = service->gsmNet.GetStatus();
    watchCall(call, this, SLOT(getStatusFinished(QFsoDBusPendingCall &)));
}

FsoNetworkRegistration::~FsoNetworkRegistration()
{
}

static QTelephony::RegistrationState fsoRegStateToQt(QString state)
{
    if(state == "home")             // registered to home network
        return QTelephony::RegistrationHome;
    if(state == "unregistered")     // not registered, not trying
        return QTelephony::RegistrationNone;
    if(state == "busy")             // not registered, but currently trying
        return QTelephony::RegistrationSearching;
    if(state == "denied")           // no permitted network available
        return QTelephony::RegistrationDenied;
    if(state == "unknown")         // no idea
        return QTelephony::RegistrationUnknown;
    if(state == "roaming")         // registered to foreign network
        return QTelephony::RegistrationRoaming;

    qWarning() << "unknown FSO registration state" << state;
    return QTelephony::RegistrationNone;
}

static QTelephony::OperatorMode fsoOpModeToQt(QString mode)
{
    if(mode == "automatic")             // automatic selection
        return QTelephony::OperatorModeAutomatic;
    if(mode == "manual")                // manual selection
        return QTelephony::OperatorModeManual;
    if(mode == "manual;automatic")      // manual first, then automatic,
        return QTelephony::OperatorModeManualAutomatic;
    if(mode == "unregister")            // manual unregister
        return QTelephony::OperatorModeDeregister;

    qWarning() << "unknown FSO operator mode " << mode;
    return QTelephony::OperatorModeAutomatic;
}

static QTelephony::OperatorAvailability fsoOpStatusToQt(QString status)
{
    if(status == "available")
        return QTelephony::OperatorAvailable;
    if(status == "current")
        return QTelephony::OperatorCurrent;
    if(status == "forbidden")
        return QTelephony::OperatorForbidden;

    qWarning() << "unknown FSO operator status " << status;
    return QTelephony::OperatorUnavailable;
}

void FsoNetworkRegistration::getStatusFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply<QVariantMap> reply = call;
    if(!checkReply(reply, "GsmStatus"))
    {
        return;
    }
    QVariantMap map = reply.value();
    QString registration = map.value("registration").toString();
    QString mode = map.value("mode").toString();
    QString provider = map.value("provider").toString();
    QString display = map.value("display").toString();
    QString act = map.value("act").toString();
    
    updateInitialized(true);
    updateRegistrationState(fsoRegStateToQt(registration));
    updateCurrentOperator(fsoOpModeToQt(mode), provider, display, act);
}

void FsoNetworkRegistration::setCurrentOperator
        ( QTelephony::OperatorMode, const QString & id, const QString &)
{
    QFsoDBusPendingReply<> reply = service->gsmNet.RegisterWithProvider(id);
    bool ok = checkReply(reply, "RegisterWithProvider");
    emit setCurrentOperatorResult(ok ? QTelephony::OK : QTelephony::Error);
}

void FsoNetworkRegistration::requestAvailableOperators()
{
    QFsoDBusPendingReply<QFsoNetworkProviderList> reply = service->gsmNet.ListProviders();
    if(!checkReply(reply, "ListProviders"))
    {
        return;
    }
    QFsoNetworkProviderList list = reply.value();
    QList<QNetworkRegistration::AvailableOperator> opers;
    for(int i = 0; i < list.count(); i++)
    {
        QFsoNetworkProvider provider = list.at(i);
        QNetworkRegistration::AvailableOperator oper;
        
        oper.availability = fsoOpStatusToQt(provider.status);
        oper.name = provider.longname;
        oper.id = provider.shortname;
        oper.technology = provider.act;
        opers.append(oper);
    }    
    emit availableOperators(opers);
}