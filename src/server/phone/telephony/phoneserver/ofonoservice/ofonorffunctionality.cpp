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

#include "ofonorffunctionality.h"
#include "ofonotelephonyservice.h"
#include "ofonoutil.h"

OFonoRfFunctionality::OFonoRfFunctionality(OFonoTelephonyService * service)
:QPhoneRfFunctionality(service->service(), service, QCommInterface::Server)
    , service(service)
    , modemAlive(false)
    , reqLevel((QPhoneRfFunctionality::Level)(-1))
{
}

OFonoRfFunctionality::~OFonoRfFunctionality()
{
}

QPhoneRfFunctionality::Level ofonoLevelToQt(QString level)
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
    qWarning() << "ofonoLevelToQt: unknown level " << level;
    return QPhoneRfFunctionality::Full;
}

QString qtLevelToOFono(QPhoneRfFunctionality::Level level)
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

void OFonoRfFunctionality::deviceStatus(QString status)
{
    bool oldAlive = modemAlive;
    modemAlive = status.startsWith("alive-");

    qDebug() << "OFonoRfFunctionality::deviceStatus status=" << status <<
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

void OFonoRfFunctionality::forceLevelRequest()
{
    qDebug() << "forceLevelRequest";

    if (!modemAlive) {
        return;
    }

/*    QOFonoDBusPendingReply < QString, bool, QString > reply =
        service->gsmDev.GetFunctionality();
    if (!checkReply(reply, true)) {
        return;
    }
    QString level = reply.argumentAt(0).toString();
    setValue("level", qVariantFromValue(ofonoLevelToQt(level)));
    emit levelChanged();
    */
}

void OFonoRfFunctionality::setLevel(QPhoneRfFunctionality::Level level)
{
    qWarning() << "setLevel level=" << level;

/*    
    if (!modemAlive) {
        reqLevel = level;       // just remember the request
        return;
    }
    // Retrieve autoregister and pin values
    QOFonoDBusPendingReply < QString, bool, QString > reply =
        service->gsmDev.GetFunctionality();
    QTelephony::Result res = qTelResult(reply);
    if (res != QTelephony::OK) {
        emit setLevelResult(res);
        return;
    }
    bool autoregister = reply.argumentAt(1).toBool();
    QString pin = reply.argumentAt(2).toString();
    QString ofonoLevel = qtLevelToOFono(level);

    // Set actual value
    QOFonoDBusPendingReply <> reply2 =
        service->gsmDev.SetFunctionality(ofonoLevel, autoregister, pin);
    emit setLevelResult(qTelResult(reply2)); */
}
