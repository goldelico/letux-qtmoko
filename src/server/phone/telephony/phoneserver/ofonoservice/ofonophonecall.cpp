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

#include "ofonophonecall.h"
#include "ofonotelephonyservice.h"
#include "ofonocallprovider.h"

OFonoPhoneCall::OFonoPhoneCall
        ( OFonoTelephonyService *service, const QString& identifier,
          const QString& callType, int id )
    : QPhoneCallImpl( &service->call_provider, identifier, callType )
    , service(service)
    , id(id)
{
}

OFonoPhoneCall::~OFonoPhoneCall()
{
}

static QPhoneCall::State ofonoStatusToQt(QString ofonoStatus, bool hangupLocal, int & id)
{
    if(ofonoStatus == "INCOMING")     // The call is incoming (but not yet accepted)
        return QPhoneCall::Incoming;
    if(ofonoStatus == "OUTGOING")     // The call is outgoing (but not yet established)
        return QPhoneCall::Dialing;
    if(ofonoStatus == "ACTIVE")       // The call is the active call (you can talk),
        return QPhoneCall::Connected;
    if(ofonoStatus == "HELD")         // The call is being held
        return QPhoneCall::Hold;
    if(ofonoStatus == "RELEASE")      // The call has been released
    {
        id = 0;
        return hangupLocal ? QPhoneCall::HangupLocal : QPhoneCall::HangupRemote;
    }
    
    qWarning() << "ofonoStatusToQt: unknown status " << ofonoStatus;
    return QPhoneCall::OtherFailure;
}

void OFonoPhoneCall::setOFonoStatus(QString ofonoStatus)
{
    setState(ofonoStatusToQt(ofonoStatus, false, id));
}

void OFonoPhoneCall::dial( const QDialOptions& options )
{
    QString number = options.number();
    setNumber( number );
    
    qDebug() << "OFonoPhoneCall::dial(" << number << ")";

    // If the number starts with '*' or '#', then this is a request
    // for a supplementary service, not an actual phone call.
    // So we dial and then immediately hang up, allowing the network
    // to send us the SS/USSD response when it is ready.
    if ( number.startsWith("*") || number.startsWith("#") ) {
        service->suppl_services.sendSupplementaryServiceData(number);
        setState( QPhoneCall::ServiceHangup );
        return;
    }
    
    QOFonoDBusPendingCall call = service->gsmCall.Initiate(number, "voice");
    watchOFonoCall(call, this, SLOT(initiateFinished(QOFonoDBusPendingCall &)));
    setState(QPhoneCall::Dialing);
}

void OFonoPhoneCall::initiateFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply<int> reply = call;
    if(checkReply(reply, "Initiate"))
    {
        id = reply.value();
    }   
}

void OFonoPhoneCall::hangup( QPhoneCall::Scope scope)
{
    qDebug() << "OFonoPhoneCall::hangup()";

    if(scope == QPhoneCall::CallOnly)
    {
        QOFonoDBusPendingReply<> reply = service->gsmCall.Release(id);
        checkReply(reply);
    }
    else
    {
        // TODO: not sure if ReleaseAll() is ok
        QOFonoDBusPendingReply<> reply = service->gsmCall.ReleaseAll();
        checkReply(reply);
    }
    id = -1;
    setState(QPhoneCall::HangupLocal);
}

void OFonoPhoneCall::accept()
{
    qDebug() << "OFonoPhoneCall::accept()";
    QOFonoDBusPendingReply<> reply = service->gsmCall.Activate(id);
    checkReply(reply);
}

void OFonoPhoneCall::hold()
{
    qDebug() << "OFonoPhoneCall::hold()";
    QOFonoDBusPendingReply<> reply = service->gsmCall.HoldActive();
    checkReply(reply);
}

void OFonoPhoneCall::activate( QPhoneCall::Scope )
{
    qDebug() << "OFonoPhoneCall::activate()";
    QOFonoDBusPendingReply<> reply = service->gsmCall.Activate(id);
    checkReply(reply);
}

void OFonoPhoneCall::tone( const QString& tones )
{
    qDebug() << "OFonoPhoneCall::tone(" << tones << ")";
}

void OFonoPhoneCall::transfer( const QString& number )
{
    qDebug() << "OFonoPhoneCall::transfer(" << number << ")";
}
