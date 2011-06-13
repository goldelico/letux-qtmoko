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

#ifndef FSOSUPPLEMENTARYSERVICES_H
#define FSOSUPPLEMENTARYSERVICES_H

#include <QTimer>
#include <QtDBus>
#include <qsupplementaryservices.h>

class FsoTelephonyService;

class FsoSupplementaryServices : public QSupplementaryServices
{
    Q_OBJECT
public:
    FsoSupplementaryServices( FsoTelephonyService * service );
    ~FsoSupplementaryServices();

    FsoTelephonyService * service;
    QDBusPendingCallWatcher watcher;
    
    void onIncomingUssd(const QString &mode, const QString &message);

public slots:
    void cancelUnstructuredSession();
    void sendUnstructuredData( const QString& data );
    void sendSupplementaryServiceData( const QString& data );

private slots:
    void sendUssdRequestFinished(QDBusPendingCallWatcher * watcher);
};

#endif

