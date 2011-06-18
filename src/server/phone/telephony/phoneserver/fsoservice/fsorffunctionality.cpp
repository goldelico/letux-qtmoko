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

FsoRfFunctionality::FsoRfFunctionality
        ( FsoTelephonyService *service )
    : QPhoneRfFunctionality( service->service(), service, QCommInterface::Server )
    , service(service)
{
}

FsoRfFunctionality::~FsoRfFunctionality()
{
}

QPhoneRfFunctionality::Level fsoLevelToQt(QString level)
{
    if(level == "full")
    {
        return QPhoneRfFunctionality::Full;
    }
    if(level == "airplane")
    {
        return QPhoneRfFunctionality::DisableTransmitAndReceive;
    }
    if(level == "minimal")
    {
        return QPhoneRfFunctionality::Minimum;
    }
    qWarning() << "fsoLevelToQt: unknown level " << level;
    return QPhoneRfFunctionality::Full;
}

QString qtLevelToFso(QPhoneRfFunctionality::Level level)
{
    switch(level)
    {
        case QPhoneRfFunctionality::Full:
            return "full";
        case QPhoneRfFunctionality::Minimum:
            return "minimal";
        default:
            return "airplane";
    }
}

void FsoRfFunctionality::forceLevelRequest()
{
    qWarning() << "forceLevelRequest";
    
    QFsoDBusPendingReply<QString, bool, QString> reply = service->gsmDev.GetFunctionality();
    if(!checkReply(reply, true))
    {
        return;
    }
    QString level = reply.argumentAt(0).toString();
    setValue( "level", qVariantFromValue( fsoLevelToQt(level) ) );
    emit levelChanged();
}

void FsoRfFunctionality::setLevel( QPhoneRfFunctionality::Level level )
{
    qWarning() << "setLevel level=" << level;

    // Retrieve autoregister and pin values
    QTelephony::Result res;
    QFsoDBusPendingReply<QString, bool, QString> reply = service->gsmDev.GetFunctionality();
    if(!checkReply(reply, true, res))
    {
        emit setLevelResult(res);
        return;
    }
    bool autoregister = reply.argumentAt(1).toBool();
    QString pin = reply.argumentAt(2).toString();
    QString fsoLevel = qtLevelToFso(level);

    // Set actual value
    QFsoDBusPendingReply<> reply2 = service->gsmDev.SetFunctionality(fsoLevel, autoregister, pin);
    checkReply(reply2, true, res);
    emit setLevelResult(res);
}
