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

#include "fsosupplementaryservices.h"
#include "fsomodemservice.h"
#include "fsoutil.h"

FsoSupplementaryServices::FsoSupplementaryServices(FsoModemService *service)
    : QSupplementaryServices( service->service(), service, QCommInterface::Server )
    , service(service)
    , watcher(QDBusPendingReply<>(), this)
{
}

FsoSupplementaryServices::~FsoSupplementaryServices()
{
}

void FsoSupplementaryServices::cancelUnstructuredSession()
{
    qDebug() << "FsoSupplementaryServices::cancelUnstructuredSession";
}

void FsoSupplementaryServices::sendUnstructuredData( const QString& data )
{
    qDebug() << "FsoSupplementaryServices::sendUnstructuredData data=" << data;
}

void FsoSupplementaryServices::sendSupplementaryServiceData
        ( const QString& data )
{
    qDebug() << "FsoSupplementaryServices::sendSupplementaryServiceData data=" << data;
    QDBusPendingReply<> reply = service->gsmNet.SendUssdRequest(data);
    watchCall(reply, &watcher, this, SLOT(sendUssdRequestFinished(QDBusPendingCallWatcher*)));
}

void FsoSupplementaryServices::sendUssdRequestFinished(QDBusPendingCallWatcher * watcher)
{
    QDBusPendingReply<> reply = *watcher;
    bool ok = checkReply(reply, "SendUssdRequest");
    emit supplementaryServiceResult(ok ? QTelephony::OK : QTelephony::Error);
}

static QSupplementaryServices::UnstructuredAction fsoModeToQtAction(const QString &mode)
{
    if(mode == "completed")     // if the last user-initiated request has been successfully completed
        return QSupplementaryServices::NoFurtherActionRequired;
    if(mode == "useraction")    // if this is a network-initiated request and further user action is necessary
        return QSupplementaryServices::FurtherActionRequired;
    if(mode == "terminated")    // if the network terminated the request
        return QSupplementaryServices::TerminatedByNetwork;
    if(mode == "localclient")   // if another local client on the network has responded
        return QSupplementaryServices::OtherLocalClientResponded;
    if(mode == "unsupported")   // if the last user-inititated request is unsupported
        return QSupplementaryServices::OperationNotSupported;
    if(mode == "timeout")       // if the network has not answered in time
        return QSupplementaryServices::NetworkTimeout;
    
    qWarning() << "fsoModeToQtAction: unknown mode " << mode;
    return QSupplementaryServices::NoFurtherActionRequired;
}

void FsoSupplementaryServices::onIncomingUssd(const QString &mode, const QString &message)
{
    emit unstructuredNotification(fsoModeToQtAction(mode), message);
}