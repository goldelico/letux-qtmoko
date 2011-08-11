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

#ifndef FSOSMSREADER_H
#define FSOSMSREADER_H

#include <QtDBus>
#include <qsmsreader.h>
#include "fsoutil.h"
#include <qfsopimmessage.h>
#include <qfsopimmessages.h>
#include <qfsopimmessagequery.h>

class FsoTelephonyService;

class FsoSMSReader : public QSMSReader
{
    Q_OBJECT
public:
    FsoSMSReader( FsoTelephonyService * service );
    ~FsoSMSReader();

    FsoTelephonyService * service;
    QFsoPIMMessageQuery msgQuery;
    int resultIndex;
    int resultCount;

    void deviceStatus(QString status);              // called by fso telephony service to report device status
    void incomingMessage(const QString & path);
    
public slots:
    void test();
    void check();
    void firstMessage();
    void nextMessage();
    void deleteMessage( const QString& id );
    void setUnreadCount( int value );

private slots:
    void getResultCountFinished(QFsoDBusPendingCall &);
    void getResultFinished(QFsoDBusPendingCall &);
};

#endif

