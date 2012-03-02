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

#include "ofonosmssender.h"
#include "ofonotelephonyservice.h"

OFonoSMSSender::OFonoSMSSender( OFonoTelephonyService *service )
    : QSMSSender( service->service(), service, QCommInterface::Server )
    , service(service)
    , smsId()
{
}

OFonoSMSSender::~OFonoSMSSender()
{
}

void OFonoSMSSender::send( const QString& id, const QSMSMessage& msg )
{
    qDebug() << "OFonoSMSSender::send" << "id=" << id << "recipient=" << msg.recipient() << "text=" << msg.text();
    
    QOFonoDBusPendingCall call = service->gsmSms.SendTextMessage(
        msg.recipient(), msg.text(), msg.statusReportRequested());

    watchOFonoCall(call, this, SLOT(sendTextMessageFinished(QOFonoDBusPendingCall &)));
    smsId = id;
}

void OFonoSMSSender::sendTextMessageFinished(QOFonoDBusPendingCall & call)
{
    qDebug() << "sendTextMessageFinished";
    QOFonoDBusPendingReply<int, QString> reply = call;
    emit finished(smsId, qTelResult(reply));
}