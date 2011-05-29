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
#include "fsomodemservice.h"
#include "fsoservicechecker.h"
#include "fsophonerffunctionality.h"
#include "fsonetworkregistration.h"

FsoModemService::FsoModemService
        ( const QString& service, QSerialIODeviceMultiplexer *mux,
          QObject *parent )
    : QModemService( service, mux, parent )
{
}

FsoModemService::~FsoModemService()
{
}

void FsoModemService::initialize()
{
    // Create our Fso-specific overrides for the service interfaces
 
     // If the modem does not exist, then tell clients via QServiceChecker.
    if ( !supports<QServiceChecker>() )
        addInterface( new FsoServiceChecker( service(), this ) );
 
    if ( !supports<QPhoneRfFunctionality>() )
        addInterface( new FsoRfFunctionality( service(), this ) );
 
    if ( !supports<QNetworkRegistration>() )
        addInterface( new FsoNetworkRegistration( service(), this ) );

/*    if ( !supports<QSimInfo>() )
        addInterface( new FsoSimInfo( this ) );

    if ( !supports<QSimToolkit>() )
        addInterface( new FsoSimToolkit( this ) );

    if ( !supports<QPhoneBook>() )
        addInterface( new FsoPhoneBook( this ) );

    if ( !supports<QTelephonyConfiguration>() )
        addInterface( new FsoConfiguration( this ) );

    if ( !callProvider() )
        setCallProvider( new FsoCallProvider( this ) );

    // Call QModemService to create other interfaces that we didn't override.
    QModemService::initialize();*/
}
