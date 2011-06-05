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

#ifndef FSOCALLPROVIDER_H
#define FSOCALLPROVIDER_H

#include <QTimer>
#include <QDebug>
#include <qtopialog.h>
#include <qphonecallprovider.h>

class FsoModemService;

class FsoCallProvider : public QPhoneCallProvider
{
    Q_OBJECT
public:
    FsoCallProvider( FsoModemService * service );
    ~FsoCallProvider();

    FsoModemService *service;

protected:
    QPhoneCallImpl *create
        ( const QString& identifier, const QString& callType );
        
};

#endif

