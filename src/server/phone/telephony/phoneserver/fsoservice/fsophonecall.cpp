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
#include "fsotelephonyservice.h"
#include "fsocallprovider.h"

FsoPhoneCall::FsoPhoneCall
        ( FsoTelephonyService *service, const QString& identifier,
          const QString& callType, int id )
    : QPhoneCallImpl( &service->call_provider, identifier, callType )
    , service(service)
    , id(id)
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
    
    qDebug() << "FsoPhoneCall::dial(" << number << ")";

    // If the number starts with '*' or '#', then this is a request
    // for a supplementary service, not an actual phone call.
    // So we dial and then immediately hang up, allowing the network
    // to send us the SS/USSD response when it is ready.
    if ( number.startsWith("*") || number.startsWith("#") ) {
        service->suppl_services.sendSupplementaryServiceData(number);
        setState( QPhoneCall::ServiceHangup );
        return;
    }
    
    QFsoDBusPendingCall call = service->gsmCall.Initiate(number, "voice");
    watchCall(call, this, SLOT(initiateFinished(QFsoDBusPendingCall &)));
    setState(QPhoneCall::Dialing);
}

void FsoPhoneCall::initiateFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply<int> reply = call;
    if(checkReply(reply, "Initiate"))
    {
        id = reply.value();
    }   
}

void FsoPhoneCall::hangup( QPhoneCall::Scope scope)
{
    qDebug() << "FsoPhoneCall::hangup()";

    if(scope == QPhoneCall::CallOnly)
    {
        QFsoDBusPendingReply<> reply = service->gsmCall.Release(id);
        checkReply(reply);
    }
    else
    {
        // TODO: not sure if ReleaseAll() is ok
        QFsoDBusPendingReply<> reply = service->gsmCall.ReleaseAll();
        checkReply(reply);
    }
    id = -1;
    setState(QPhoneCall::HangupLocal);
}

void FsoPhoneCall::accept()
{
    qDebug() << "FsoPhoneCall::accept()";
    QFsoDBusPendingReply<> reply = service->gsmCall.Activate(id);
    checkReply(reply);
}

void FsoPhoneCall::hold()
{
    qDebug() << "FsoPhoneCall::hold()";
    QFsoDBusPendingReply<> reply = service->gsmCall.HoldActive();
    checkReply(reply);
}

void FsoPhoneCall::activate( QPhoneCall::Scope )
{
    qDebug() << "FsoPhoneCall::activate()";
    QFsoDBusPendingReply<> reply = service->gsmCall.Activate(id);
    checkReply(reply);
}

void FsoPhoneCall::tone( const QString& tones )
{
    qDebug() << "FsoPhoneCall::tone(" << tones << ")";
}

void FsoPhoneCall::transfer( const QString& number )
{
    qDebug() << "FsoPhoneCall::transfer(" << number << ")";
}
