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

#include "fsorffunctionality.h"
#include "fsotelephonyservice.h"
#include "fsoutil.h"

FsoRfFunctionality::FsoRfFunctionality(FsoTelephonyService * service)
:QPhoneRfFunctionality(service->service(), service, QCommInterface::Server)
    , service(service)
    , modemAlive(false)
    , reqLevel((QPhoneRfFunctionality::Level)(-1))
{
}

FsoRfFunctionality::~FsoRfFunctionality()
{
}

QPhoneRfFunctionality::Level fsoLevelToQt(QString level)
{
    if (level == "full") {
        return QPhoneRfFunctionality::Full;
    }
    if (level == "airplane") {
        return QPhoneRfFunctionality::DisableTransmitAndReceive;
    }
    if (level == "minimal") {
        return QPhoneRfFunctionality::Minimum;
    }
    qWarning() << "fsoLevelToQt: unknown level " << level;
    return QPhoneRfFunctionality::Full;
}

QString qtLevelToFso(QPhoneRfFunctionality::Level level)
{
    switch (level) {
    case QPhoneRfFunctionality::Full:
        return "full";
    case QPhoneRfFunctionality::Minimum:
        return "minimal";
    default:
        return "airplane";
    }
}

void FsoRfFunctionality::deviceStatus(QString status)
{
    bool oldAlive = modemAlive;
    modemAlive = status.startsWith("alive-");

    qDebug() << "FsoRfFunctionality::deviceStatus status=" << status <<
        ", oldAlive=" << oldAlive << ", modemAlive=" << modemAlive;

    if (oldAlive != modemAlive) {
        // Set level which was requested while modem was not alive
        if (modemAlive && reqLevel >= 0) {
            setLevel(reqLevel);
            reqLevel = (QPhoneRfFunctionality::Level)(-1);
        }
        forceLevelRequest();
    }
}

void FsoRfFunctionality::forceLevelRequest()
{
    qDebug() << "forceLevelRequest";

    if (!modemAlive) {
        return;
    }

    QFsoDBusPendingReply < QString, bool, QString > reply =
        service->gsmDev.GetFunctionality();
    if (!checkReply(reply, true)) {
        return;
    }
    QString level = reply.argumentAt(0).toString();
    setValue("level", qVariantFromValue(fsoLevelToQt(level)));
    emit levelChanged();
}

void FsoRfFunctionality::setLevel(QPhoneRfFunctionality::Level level)
{
    qWarning() << "setLevel level=" << level;

    if (!modemAlive) {
        reqLevel = level;       // just remember the request
        return;
    }
    // Retrieve autoregister and pin values
    QFsoDBusPendingReply < QString, bool, QString > reply =
        service->gsmDev.GetFunctionality();
    QTelephony::Result res = qTelResult(reply);
    if (res != QTelephony::OK) {
        emit setLevelResult(res);
        return;
    }
    bool autoregister = reply.argumentAt(1).toBool();
    QString pin = reply.argumentAt(2).toString();
    QString fsoLevel = qtLevelToFso(level);

    // Set actual value
    QFsoDBusPendingReply <> reply2 =
        service->gsmDev.SetFunctionality(fsoLevel, autoregister, pin);
    emit setLevelResult(qTelResult(reply2));
}
