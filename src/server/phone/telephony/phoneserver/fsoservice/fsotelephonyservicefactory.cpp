/****************************************************************************
 * *
 ** This file is part of the Qt Extended Opensource Package.
 **
 ** Copyright (C) 2011 Radek Polak
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

#include "fsotelephonyservicefactory.h"
#include <qvaluespace.h>
#include <QSignalSourceProvider>
#include <qtimer.h>
#include "phoneserver.h"
#include "fsotelephonyservice.h"

FsoTelephonyServiceFactory::FsoTelephonyServiceFactory(QObject *parent)
{
    Q_UNUSED(parent);
}

QTelephonyService* FsoTelephonyServiceFactory::service()
{
    return new FsoTelephonyService("modem", NULL);
}

QByteArray FsoTelephonyServiceFactory::serviceName() const 
{
    //synchronize with phoneserver.cpp 
    return QByteArray("FsoModemService");    
}

QTOPIA_TASK(FsoTelephonyServiceFactory, FsoTelephonyServiceFactory);
QTOPIA_TASK_PROVIDES(FsoTelephonyServiceFactory, TelephonyServiceFactory);
