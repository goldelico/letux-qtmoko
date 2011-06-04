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

#include "fsophonecall.h"
#include "fsocallprovider.h"

FsoPhoneCall::FsoPhoneCall
        ( FsoCallProvider *provider, const QString& identifier,
          const QString& callType, int id )
    : QPhoneCallImpl( provider, identifier, callType )
    , provider(provider)
    , id(id)
    , hangupLocal(false)
{
}

FsoPhoneCall::~FsoPhoneCall()
{
}

static QPhoneCall::State fsoStatusToQt(QString fsoStatus, bool hangupLocal)
{
    if(fsoStatus == "INCOMING")     // The call is incoming (but not yet accepted)
        return QPhoneCall::Incoming;
    if(fsoStatus == "OUTGOING")     // The call is outgoing (but not yet established)
        return QPhoneCall::Dialing;
    if(fsoStatus == "ACTIVE")       // The call is the active call (you can talk),
        return QPhoneCall::Connected;
    if(fsoStatus == "HELD")         // The call is being held
        return QPhoneCall::Hold;
    if(fsoStatus == "RELEASE")      // The call has been released
        return hangupLocal ? QPhoneCall::HangupLocal : QPhoneCall::HangupRemote;
    
    qWarning() << "fsoStatusToQt: unknown status " << fsoStatus;
    return QPhoneCall::OtherFailure;
}

void FsoPhoneCall::setFsoStatus(QString fsoStatus)
{
    setState(fsoStatusToQt(fsoStatus, hangupLocal));
}

void FsoPhoneCall::dial( const QDialOptions& options )
{
    qLog(Modem) << "FsoPhoneCall::dial(" << options.number() << ")";
    provider->dial(this, options);
}

void FsoPhoneCall::hangup( QPhoneCall::Scope scope)
{
    qLog(Modem) << "FsoPhoneCall::hangup()";
    hangupLocal = true;
    provider->hangup(this, scope);
}

void FsoPhoneCall::accept()
{
    qLog(Modem) << "FsoPhoneCall::accept()";
}

void FsoPhoneCall::hold()
{
    qLog(Modem) << "FsoPhoneCall::hold()";
}

void FsoPhoneCall::activate( QPhoneCall::Scope )
{
    qLog(Modem) << "FsoPhoneCall::activate()";
}

void FsoPhoneCall::tone( const QString& tones )
{
    qLog(Modem) << "FsoPhoneCall::tone(" << tones << ")";
}

void FsoPhoneCall::transfer( const QString& number )
{
    qLog(Modem) << "FsoPhoneCall::transfer(" << number << ")";
}
