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
#include "fsotelephonyservice.h"

FsoTelephonyService::FsoTelephonyService(const QString& service, QObject *parent)
    : QTelephonyService( service, parent )
    , gsmDev("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , gsmNet("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , gsmSms("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , service_checker(this)
    , rf_functionality(this)
    , network_registration(this)
    , suppl_services(this)
    , call_provider(this)
    , sms_sender(this)
{
    connect(&gsmCall,
            SIGNAL(CallStatus(int, const QString &, const QVariantMap &)),
            this,
            SLOT(callStatusChange(int, const QString &, const QVariantMap &)));

    connect(&gsmNet,
            SIGNAL(IncomingUssd(const QString &, const QString &)),
            this,
            SLOT(incomingUssd(const QString &, const QString &)));            
}

FsoTelephonyService::~FsoTelephonyService()
{
}

void FsoTelephonyService::initialize()
{
    // Create our Fso-specific overrides for the service interfaces
 
     // If the modem does not exist, then tell clients via QServiceChecker.
    if ( !supports<QServiceChecker>() )
        addInterface( &service_checker );
 
    if ( !supports<QPhoneRfFunctionality>() )
        addInterface( &rf_functionality );
 
    if ( !supports<QNetworkRegistration>() )
        addInterface( &network_registration );
    
    if ( !supports<QSupplementaryServices>() )
        addInterface( &suppl_services );
    
    if ( !supports<QSMSSender>() )
        addInterface( &sms_sender );

/*    if ( !supports<QSimInfo>() )
        addInterface( new FsoSimInfo( this ) );

    if ( !supports<QSimToolkit>() )
        addInterface( new FsoSimToolkit( this ) );

    if ( !supports<QPhoneBook>() )
        addInterface( new FsoPhoneBook( this ) );

    if ( !supports<QTelephonyConfiguration>() )
        addInterface( new FsoConfiguration( this ) );
	*/

    if ( !callProvider() )
        setCallProvider( &call_provider );

    // Call QModemService to create other interfaces that we didn't override.
    //QModemService::initialize();
}

void FsoTelephonyService::callStatusChange(int id, const QString &status, const QVariantMap &properties)
{
    QString str = QString("id=%1, status=%2").arg(id).arg(status);
    for(int i = 0; i < properties.count(); i++)
    {
        QString key = properties.keys().at(i);
        str += ", " + key + "=" + properties.value(key).toString();
    }
    qDebug() << "CallStatusChange" << str;

    if(status == "INCOMING")
    {
        // Incoming calls have to be created here
        FsoPhoneCall *call = new FsoPhoneCall( this, NULL, "Voice", id );
        call->setNumber(properties.value("peer").toString());
        call->setFsoStatus(status);
        return;
    }
    
    // Check if it is an existing call
    QList<QPhoneCallImpl *> list = call_provider.calls();
    for(int i = 0; i < list.count(); i++)
    {
        FsoPhoneCall *call = static_cast<FsoPhoneCall*>(list.at(i));
        qDebug() << "call identifier=" << call->identifier() << ", call id=" << call->id;
        if(call->id == id)
        {
            call->setFsoStatus(status);
            return;
        }
    }

    if(status == "RELEASE")
    {
        return;     // Released call can have id -1, it was not found in the loop but that's ok
    }

    qWarning() << "callStatusChange: unhandled status for new call" << status;
}

void FsoTelephonyService::incomingUssd(const QString &mode, const QString &message)
{
    qDebug() << "Incomming ussd", "mode=" + mode + ", message=" + message;
    suppl_services.onIncomingUssd(mode, message);
}
