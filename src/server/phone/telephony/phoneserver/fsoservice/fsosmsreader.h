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
#include <qfsogsmsms.h>
#include <qfsogsmsim.h>

class FsoTelephonyService;

class FsoSMSReader : public QSMSReader
{
    Q_OBJECT
public:
    FsoSMSReader(FsoTelephonyService * service);
    ~FsoSMSReader();

    FsoTelephonyService *service;
    QFsoSIMMessageList messages;        // messages read from SIM
    QFsoSIMMessageList incoming;        // messages that came from incomingTextMessage event
    int index;
    int numSlots;

    void deviceStatus(QString status);  // called by fso telephony service to report device status
    void incomingTextMessage(const QString & number, const QString & timestamp,
                             const QString & contents);

public slots:
    void test();
    void check();
    void firstMessage();
    void nextMessage();
    void deleteMessage(const QString & id);
    void setUnreadCount(int value);

private slots:
    void retrieveMessageFinished(QFsoDBusPendingCall &);
};

#endif
