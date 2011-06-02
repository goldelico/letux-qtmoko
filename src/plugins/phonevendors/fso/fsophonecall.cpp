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

FsoPhoneCall::FsoPhoneCall
        ( QPhoneCallProvider *provider, const QString& identifier,
          const QString& callType, int callNo )
    : QPhoneCallImpl( provider, identifier, callType )
{
    this->provider = provider;
    this->callNo = callNo;
}

FsoPhoneCall::~FsoPhoneCall()
{
}

void FsoPhoneCall::dial( const QDialOptions& options )
{
    qLog(Modem) << "FsoPhoneCall::dial(" << options.number() << ")";
}

void FsoPhoneCall::hangup( QPhoneCall::Scope )
{
    qLog(Modem) << "FsoPhoneCall::hangup()";
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
