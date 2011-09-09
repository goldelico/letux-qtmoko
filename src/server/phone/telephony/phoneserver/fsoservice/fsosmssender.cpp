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

#include "fsosmssender.h"
#include "fsotelephonyservice.h"

FsoSMSSender::FsoSMSSender( FsoTelephonyService *service )
    : QSMSSender( service->service(), service, QCommInterface::Server )
    , service(service)
    , smsId()
{
}

FsoSMSSender::~FsoSMSSender()
{
}

void FsoSMSSender::send( const QString& id, const QSMSMessage& msg )
{
    qDebug() << "FsoSMSSender::send" << "id=" << id << "recipient=" << msg.recipient() << "text=" << msg.text();
    
    QFsoDBusPendingCall call = service->gsmSms.SendTextMessage(
        msg.recipient(), msg.text(), msg.statusReportRequested());

    watchFsoCall(call, this, SLOT(sendTextMessageFinished(QFsoDBusPendingCall &)));
    smsId = id;
}

void FsoSMSSender::sendTextMessageFinished(QFsoDBusPendingCall & call)
{
    qDebug() << "sendTextMessageFinished";
    QFsoDBusPendingReply<int, QString> reply = call;
    emit finished(smsId, qTelResult(reply));
}