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

#include <fsosmssender.h>
#include "fsotelephonyservice.h"
#include "fsoutil.h"

FsoSmsCallWacher::FsoSmsCallWacher(const QDBusPendingCall & call, const QString & smsId, QObject * parent)
    : QDBusPendingCallWatcher(call, parent)
    , smsId(smsId)
{
    
}

FsoSmsCallWacher::~FsoSmsCallWacher()
{
}

FsoSMSSender::FsoSMSSender( FsoTelephonyService *service )
    : QSMSSender( service->service(), service, QCommInterface::Server )
    , service(service)
{
}

FsoSMSSender::~FsoSMSSender()
{
}

void FsoSMSSender::send( const QString& id, const QSMSMessage& msg )
{
    qDebug() << "FsoSMSSender::send" << "id=" << id << "recipient=" << msg.recipient() << "text=" << msg.text();
    
    QDBusPendingReply<int, QString> reply = service->gsmSms.SendTextMessage(
        msg.recipient(), msg.text(), msg.statusReportRequested());
       
    FsoSmsCallWacher *watcher = new FsoSmsCallWacher(reply, id, this);

    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(sendTextMessageFinished(QDBusPendingCallWatcher*)));
}

void FsoSMSSender::sendTextMessageFinished(QDBusPendingCallWatcher * watcher)
{
    qDebug() << "sendTextMessageFinished";
    FsoSmsCallWacher *fsoWatcher = static_cast<FsoSmsCallWacher*>(watcher);
    QDBusPendingReply<int, QString> reply = *watcher;
    if(checkReply(reply, "SendTextMessage"))
    {
        emit finished(fsoWatcher->smsId, QTelephony::OK);
    }
    else
    {
        emit finished(fsoWatcher->smsId, QTelephony::Error);
    }
}