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

#include "fsonetworkregistration.h"

FsoNetworkRegistration::FsoNetworkRegistration
        ( const QString& service, QObject *parent )
    : QNetworkRegistrationServer( service, parent )
{
}

FsoNetworkRegistration::~FsoNetworkRegistration()
{
}

void FsoNetworkRegistration::setCurrentOperator
        ( QTelephony::OperatorMode, const QString&, const QString&)
{
    emit setCurrentOperatorResult( QTelephony::OK );
}

void FsoNetworkRegistration::requestAvailableOperators()
{
    QList<QNetworkRegistration::AvailableOperator> opers;
    QNetworkRegistration::AvailableOperator oper;
    oper.availability = QTelephony::OperatorAvailable;
    oper.name = "test";       // No tr
    oper.id = "test";        // No tr
    oper.technology = "GSM";       // No tr
    opers.append( oper );
    emit availableOperators( opers );
}