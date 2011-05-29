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

#include "fsophonerffunctionality.h"

FsoRfFunctionality::FsoRfFunctionality
        ( const QString& service, QObject *parent )
    : QPhoneRfFunctionality( service, parent, QCommInterface::Server )
    , gsmDev("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
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
    
    QDBusPendingReply<QString, bool, QString> reply = gsmDev.GetFunctionality();
    if(!checkReply(reply, "GetFunctionality", true))
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
    QDBusPendingReply<QString, bool, QString> reply = gsmDev.GetFunctionality();
    if(!checkReply(reply, "GetFunctionality", true))
    {
        emit setLevelResult(QTelephony::Error);
        return;
    }
    bool autoregister = reply.argumentAt(1).toBool();
    QString pin = reply.argumentAt(2).toString();
    QString fsoLevel = qtLevelToFso(level);

    // Set actual value
    QDBusPendingReply<> reply2 = gsmDev.SetFunctionality(fsoLevel, autoregister, pin);
    if(!checkReply(reply2, "SetFunctionality", true))
    {
        emit setLevelResult(QTelephony::Error);
        return;
    }
    emit setLevelResult(QTelephony::OK);
}
