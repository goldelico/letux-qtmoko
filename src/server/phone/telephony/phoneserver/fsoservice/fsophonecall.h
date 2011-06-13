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

#ifndef FSOPHONECALL_H
#define FSOPHONECALL_H

#include <QtDBus>
#include <qphonecallprovider.h>
#include <qtopialog.h>

class FsoTelephonyService;

class FsoPhoneCall : public QPhoneCallImpl
{
    Q_OBJECT
public:
    FsoPhoneCall( FsoTelephonyService *service, const QString& identifier,
                  const QString& callType, int id );
    virtual ~FsoPhoneCall();

    FsoTelephonyService *service;
    int id;
    QDBusPendingCallWatcher watcher;

    void dial( const QDialOptions& options );
    void hangup( QPhoneCall::Scope scope );
    void accept();
    void hold();
    void activate( QPhoneCall::Scope scope );
    void tone( const QString& tones );
    void transfer( const QString& number );
    
    void setFsoStatus(QString fsoStatus);

public slots:
    void initiateFinished(QDBusPendingCallWatcher * watcher);
};

#endif
