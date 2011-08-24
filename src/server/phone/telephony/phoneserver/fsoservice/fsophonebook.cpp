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

#include "fsophonebook.h"
#include "fsotelephonyservice.h"

FsoPhoneBook::FsoPhoneBook(FsoTelephonyService * service)
:  QPhoneBook(service->service(), service, QCommInterface::Server)
    , service(service)
    , contactQuery("", "", QDBusConnection::systemBus(), this)
{
}

FsoPhoneBook::~FsoPhoneBook()
{
}

void FsoPhoneBook::getEntries(const QString & store)
{
    qDebug() << "FsoPhoneBook::getEntries store=" << store;

    // We support only "SM" store
    if (store != "SM") {
        QList < QPhoneBookEntry > list;
        emit entries(store, list);
        return;
    }
    // Destroy old query
    if (!contactQuery.service().isEmpty()) {
        QFsoDBusPendingReply <> reply2 = contactQuery.Dispose();
        checkResult(reply2);
    }
    // Query for all contacts
    QFsoDBusPendingReply < QString > reply =
        service->pimContacts.Query(QVariantMap());
    if (!checkResult(reply)) {
        return;
    }
    QString path = reply.value();
    new(&contactQuery) QFsoPIMContactQuery("org.freesmartphone.opimd", path,
                                           QDBusConnection::systemBus(), this);

    QFsoDBusPendingCall call = contactQuery.GetMultipleResults(-1);
    watchFsoCall(call, this,
                 SLOT(getMultipleResultsFinished(QFsoDBusPendingCall &)));
}

void FsoPhoneBook::getMultipleResultsFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply < QFsoVariantMapList > reply = call;
    qDebug() << "1";
    if (!checkResult(reply)) {
        return;
    }
    QFsoVariantMapList contacts = reply.value();

    qDebug() << "contacts.count()=" << contacts.count();

    for (int i = 0; i < contacts.count(); i++) {
        QVariantMap contact = contacts.at(i);
        for (int j = 0; j < contact.count(); j++) {
            QString key = contact.keys().at(j);
            qDebug() << "contact j=" << j << ", " << key << "=" <<
                contact.value(key).toString();
        }
    }

    QList < QPhoneBookEntry > list;
    emit entries("SM", list);
}

void FsoPhoneBook::add(const QPhoneBookEntry & entry, const QString & store,
                       bool flush)
{
    qDebug() << "FsoPhoneBook::add entry.text()=" << entry.text() << "store=" <<
        store << "flush=" << flush;
    if (store != "SM") {
        if (flush)
            getEntries(store);
        return;
    }
}

void FsoPhoneBook::remove(uint index, const QString & store, bool flush)
{
    qDebug() << "FsoPhoneBook::remove index=" << index << "store=" << store <<
        "flush=" << flush;
    if (store != "SM") {
        if (flush)
            getEntries(store);
        return;
    }
}

void FsoPhoneBook::update(const QPhoneBookEntry & entry, const QString & store,
                          bool flush)
{
    qDebug() << "FsoPhoneBook::update entry.text()=" << entry.text() << "store="
        << store << "flush=" << flush;
    if (store != "SM") {
        if (flush)
            getEntries(store);
        return;
    }
}

void FsoPhoneBook::flush(const QString & store)
{
    getEntries(store);
}

void FsoPhoneBook::setPassword(const QString &, const QString &)
{
    // Nothing to do here.
}

void FsoPhoneBook::clearPassword(const QString &)
{
    // Nothing to do here.
}

void FsoPhoneBook::requestLimits(const QString & store)
{
    qDebug() << "FsoPhoneBook::requestLimits store=" << store;
    QPhoneBookLimits l;
    l.setNumberLength(20);
    l.setTextLength(18);
    l.setFirstIndex(1);
    l.setLastIndex(150);
    emit limits(store, l);
}

void FsoPhoneBook::requestFixedDialingState()
{
    qDebug() << "FsoPhoneBook::requestFixedDialingState";
    emit fixedDialingState(fixedDialingEnabled);
}

void FsoPhoneBook::setFixedDialingState(bool enabled, const QString &)
{
    qDebug() << "FsoPhoneBook::setFixedDialingState enabled=" << enabled;
    fixedDialingEnabled = enabled;
    emit setFixedDialingStateResult(QTelephony::OK);
}
