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

FsoCallProvider::FsoCallProvider( FsoModemService *service )
    : QModemCallProvider( service )
    , gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
{
    this->service = service;
}

FsoCallProvider::~FsoCallProvider()
{
}

QPhoneCallImpl *FsoCallProvider::create
        ( const QString& identifier, const QString& callType )
{
    return new FsoPhoneCall( this, identifier, callType, -1 );
}

QModemCallProvider::AtdBehavior FsoCallProvider::atdBehavior() const
{
    return AtdOkIsConnect;
}

void FsoCallProvider::abortDial
	( uint modemIdentifier, QPhoneCall::Scope scope )
{
}

QString FsoCallProvider::releaseCallCommand( uint modemIdentifier ) const
{
    return "";
}

QString FsoCallProvider::releaseActiveCallsCommand() const
{
    return "";
}

QString FsoCallProvider::releaseHeldCallsCommand() const
{
    return "";
}

QString FsoCallProvider::putOnHoldCommand() const
{
    return "";
}

QString FsoCallProvider::setBusyCommand() const
{
    return "";
}

QString FsoCallProvider::activateCallCommand
	( uint modemIdentifier, bool otherActiveCalls ) const
{
    return "";
}

QString FsoCallProvider::activateHeldCallsCommand() const
{
    return "";
}

QString FsoCallProvider::joinCallsCommand( bool detachSubscriber ) const
{
    return "";
}

QString FsoCallProvider::deflectCallCommand( const QString& number ) const
{
    return "";
}

void FsoCallProvider::resetModem()
{
}

void FsoCallProvider::dial(FsoPhoneCall *call, const QDialOptions& options)
{
    QDBusPendingReply<int> reply = gsmCall.Initiate(options.number(), "voice");
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
}