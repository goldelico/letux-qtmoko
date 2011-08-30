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

#include "fsosiminfo.h"
#include "fsotelephonyservice.h"

FsoSimInfo::FsoSimInfo(FsoTelephonyService * service)
:  QSimInfo(service->service(), service, QCommInterface::Server)
    , service(service)
{
}

FsoSimInfo::~FsoSimInfo()
{
}

void FsoSimInfo::deviceStatus(QString status)
{
    qDebug() << "FsoSimInfo::deviceStatus status=" << status;

    if (status == "alive-registered") {
        QFsoDBusPendingCall call = service->gsmSim.GetSimInfo();
        watchFsoCall(call, this,
                     SLOT(getSimInfoFinished(QFsoDBusPendingCall &)));
    } else {
        setIdentity(QString());
    }
}

void FsoSimInfo::getSimInfoFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply < QVariantMap > reply = call;
    if (!checkResult(reply)) {
        return;
    }
    QVariantMap info = reply.value();
    setIdentity(info.value("imsi").toString());
}
