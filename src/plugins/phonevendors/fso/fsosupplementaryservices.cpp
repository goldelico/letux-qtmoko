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

#include "fsosupplementaryservices.h"
#include "fsomodemservice.h"
#include "fsoutil.h"

FsoSupplementaryServices::FsoSupplementaryServices(FsoModemService *service)
    : QSupplementaryServices( service->service(), service, QCommInterface::Server )
    , service(service)
{
}

FsoSupplementaryServices::~FsoSupplementaryServices()
{
}

void FsoSupplementaryServices::cancelUnstructuredSession()
{
    qDebug() << "FsoSupplementaryServices::cancelUnstructuredSession";
}

void FsoSupplementaryServices::sendUnstructuredData( const QString& data )
{
    qDebug() << "FsoSupplementaryServices::sendUnstructuredData data=" << data;
}

void FsoSupplementaryServices::sendSupplementaryServiceData
        ( const QString& data )
{
    qDebug() << "FsoSupplementaryServices::sendSupplementaryServiceData data=" << data;
}