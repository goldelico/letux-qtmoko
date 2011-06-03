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

#include "fsocallprovider.h"

FsoCallProvider::FsoCallProvider( const QString & service, QObject * parent )
    : QPhoneCallProvider( service, parent )
    , gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
{
    setCallTypes(QStringList() << "Voice");
}

FsoCallProvider::~FsoCallProvider()
{
}

QPhoneCallImpl *FsoCallProvider::create
        ( const QString& identifier, const QString& callType )
{
	qDebug() << "FsoCallProvider::create";
    return new FsoPhoneCall( this, identifier, callType, -1 );
}

void FsoCallProvider::dial(FsoPhoneCall *call, const QDialOptions& options)
{
    QDBusPendingReply<int> reply = gsmCall.Initiate(options.number(), "voice");
    call->setState(QPhoneCall::Dialing);
    if(checkReply(reply, "Initiate"))
    {
        call->id = reply.value();
    }   
}

void FsoCallProvider::hangup(FsoPhoneCall *call, QPhoneCall::Scope scope)
{
    if(scope == QPhoneCall::CallOnly)
    {
        QDBusPendingReply<> reply = gsmCall.Release(call->id);
        if(checkReply(reply, "Release"))
        {
            call->id = -1;
        }
    }
    else
    {
        // TODO: not sure if this is ok
        QDBusPendingReply<> reply = gsmCall.ReleaseAll();
        if(checkReply(reply, "ReleaseAll"))
        {
            call->id = -1;
        }
    }
    call->setState(QPhoneCall::HangupLocal);
}