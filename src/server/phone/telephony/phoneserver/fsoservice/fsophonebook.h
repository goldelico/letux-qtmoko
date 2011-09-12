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

#ifndef FSOPHONEBOOK_H
#define FSOPHONEBOOK_H

#include <QtDBus>
#include <qphonebook.h>
#include "fsoutil.h"
#include <qfsopimcontact.h>
#include <qfsopimcontacts.h>
#include <qfsopimcontactquery.h>

class FsoTelephonyService;

class FsoPhoneBook : public QPhoneBook
{
    Q_OBJECT
public:
    FsoPhoneBook(FsoTelephonyService * service);
    ~FsoPhoneBook();

    FsoTelephonyService *service;
    bool simReady;
    bool wantEntries;           // getEntries was called while sim was not ready yet
    bool wantLimits;              // request limits was called while sim was not ready yet
    int freeIndex;              // free index for new contact
    int numUsed;                // number of used

    void deviceStatus(QString status);  // called by fso telephony service to report device status

public slots:
    void getEntries(const QString & store);
    void add(const QPhoneBookEntry & entry, const QString & store, bool flush);
    void remove(uint index, const QString & store, bool flush);
    void update(const QPhoneBookEntry & entry, const QString & store,
                bool flush);
    void flush(const QString & store);
    void setPassword(const QString & store, const QString & passwd);
    void clearPassword(const QString & store);
    void requestLimits(const QString & store);
    void requestFixedDialingState();
    void setFixedDialingState(bool enabled, const QString & pin2);

private:
     bool fixedDialingEnabled;

private slots:
    void retrievePhonebookFinished(QFsoDBusPendingCall &);
};

#endif
