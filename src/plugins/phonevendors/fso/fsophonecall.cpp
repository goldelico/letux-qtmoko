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
#include "fsomodemservice.h"
#include "fsocallprovider.h"
#include "fsoutil.h"

FsoPhoneCall::FsoPhoneCall
        ( FsoModemService *service, const QString& identifier,
          const QString& callType, int id )
    : QPhoneCallImpl( &service->call_provider, identifier, callType )
    , service(service)
    , id(id)
    , watcher(QDBusPendingReply<>(), this)
{
}

FsoPhoneCall::~FsoPhoneCall()
{
}

static QPhoneCall::State fsoStatusToQt(QString fsoStatus, bool hangupLocal, int & id)
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
    {
        id = 0;
        return hangupLocal ? QPhoneCall::HangupLocal : QPhoneCall::HangupRemote;
    }
    
    qWarning() << "fsoStatusToQt: unknown status " << fsoStatus;
    return QPhoneCall::OtherFailure;
}

void FsoPhoneCall::setFsoStatus(QString fsoStatus)
{
    setState(fsoStatusToQt(fsoStatus, false, id));
}

void FsoPhoneCall::dial( const QDialOptions& options )
{
    QString number = options.number();
    setNumber( number );
    
    qLog(Modem) << "FsoPhoneCall::dial(" << number << ")";

    // If the number starts with '*' or '#', then this is a request
    // for a supplementary service, not an actual phone call.
    // So we dial and then immediately hang up, allowing the network
    // to send us the SS/USSD response when it is ready.
    if ( number.startsWith("*") || number.startsWith("#") ) {
        service->suppl_services.sendSupplementaryServiceData(number);
        setState( QPhoneCall::ServiceHangup );
        return;
    }
    
    QDBusPendingReply<int> reply = service->gsmCall.Initiate(number, "voice");
    watchCall(reply, &watcher, this, SLOT(initiateFinished(QDBusPendingCallWatcher*)));
    setState(QPhoneCall::Dialing);
}

void FsoPhoneCall::initiateFinished(QDBusPendingCallWatcher * watcher)
{
    QDBusPendingReply<int> reply = *watcher;
    if(checkReply(reply, "Initiate"))
    {
        id = reply.value();
    }   
}

void FsoPhoneCall::hangup( QPhoneCall::Scope scope)
{
    qLog(Modem) << "FsoPhoneCall::hangup()";

    if(scope == QPhoneCall::CallOnly)
    {
        QDBusPendingReply<> reply = service->gsmCall.Release(id);
        checkReply(reply, "Release");
    }
    else
    {
        // TODO: not sure if ReleaseAll() is ok
        QDBusPendingReply<> reply = service->gsmCall.ReleaseAll();
        checkReply(reply, "ReleaseAll");
    }
    id = -1;
    setState(QPhoneCall::HangupLocal);
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
