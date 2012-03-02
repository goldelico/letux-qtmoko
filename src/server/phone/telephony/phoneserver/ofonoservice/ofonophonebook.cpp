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

#include "ofonophonebook.h"
#include "ofonotelephonyservice.h"

OFonoPhoneBook::OFonoPhoneBook(OFonoTelephonyService * service)
:  QPhoneBook(service->service(), service, QCommInterface::Server)
    , service(service)
    , simReady(false)
    , wantEntries(false)
    , wantLimits(false)
    , freeIndex(-1)
    , numUsed(0)
{
}

OFonoPhoneBook::~OFonoPhoneBook()
{
}

void OFonoPhoneBook::deviceStatus(QString status)
{
    bool oldReady = simReady;
    simReady = (status == "alive-registered");

    qDebug() << "OFonoPhoneBook::deviceStatus status=" << status << ", oldReady="
        << oldReady << ", simReady=" << simReady;

    if (simReady && oldReady != simReady) {
        if (wantEntries) {
            wantEntries = false;
            getEntries("SM");
        }
        if (wantLimits) {
            wantLimits = false;
            requestLimits("SM");
        }
    }
}

void OFonoPhoneBook::getEntries(const QString & store)
{
    qDebug() << "OFonoPhoneBook::getEntries store=" << store;

    // We support only "SM" store
    if (store != "SM") {
        QList < QPhoneBookEntry > list;
        emit entries(store, list);
        return;
    }

    if (!simReady) {
        wantEntries = true;
        return;
    }

    QOFonoDBusPendingCall call =
        service->gsmSim.RetrievePhonebook("contacts", 0, 65535);
    watchOFonoCall(call, this,
                 SLOT(retrievePhonebookFinished(QOFonoDBusPendingCall &)));
}

void OFonoPhoneBook::retrievePhonebookFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply < QOFonoSIMEntryList > reply = call;

    if (!checkReply(reply)) {
        return;
    }

    QOFonoSIMEntryList pb = reply.value();
    QList < QPhoneBookEntry > list;

    qDebug() << "pb.count()=" << pb.count();
    freeIndex = -1;
    numUsed = pb.count();

    for (int i = 0; i < pb.count(); i++) {
        QOFonoSIMEntry entry = pb.at(i);

        qDebug() << "entry i=" << i << ", index=" << entry.index << ", name=" <<
            entry.name << ", number=" << entry.number;

        QPhoneBookEntry item;
        item.setIndex(entry.index);
        item.setNumber(entry.number);
        item.setText(entry.name);
        list.append(item);

        if (freeIndex < 0 || entry.index == freeIndex) {
            freeIndex = entry.index + 1;
        }
    }
    emit entries("SM", list);
}

void OFonoPhoneBook::add(const QPhoneBookEntry & entry, const QString & store,
                       bool flush)
{
    qDebug() << "OFonoPhoneBook::add entry.text()=" << entry.text() << "store=" <<
        store << "flush=" << flush;

    QOFonoDBusPendingReply <> reply =
        service->gsmSim.StoreEntry("contacts", freeIndex, entry.text(),
                                   entry.number());

    checkReply(reply);
    getEntries(store);
}

void OFonoPhoneBook::remove(uint index, const QString & store, bool flush)
{
    qDebug() << "OFonoPhoneBook::remove index=" << index << "store=" << store <<
        "flush=" << flush;

    QOFonoDBusPendingReply <> reply =
        service->gsmSim.DeleteEntry("contacts", index);

    checkReply(reply);
    getEntries(store);
}

void OFonoPhoneBook::update(const QPhoneBookEntry & entry, const QString & store,
                          bool flush)
{
    qDebug() << "OFonoPhoneBook::update entry.text()=" << entry.text() << "store="
        << store << "flush=" << flush;

    QOFonoDBusPendingReply <> reply =
        service->gsmSim.StoreEntry("contacts", entry.index(), entry.text(),
                                   entry.number());
    checkReply(reply);
    getEntries(store);
}

void OFonoPhoneBook::flush(const QString & store)
{
    getEntries(store);
}

void OFonoPhoneBook::setPassword(const QString &, const QString &)
{
    // Nothing to do here.
}

void OFonoPhoneBook::clearPassword(const QString &)
{
    // Nothing to do here.
}

void OFonoPhoneBook::requestLimits(const QString & store)
{
    qDebug() << "OFonoPhoneBook::requestLimits store=" << store;

    if (!simReady) {
        wantLimits = true;
        return;
    }

    QOFonoDBusPendingReply < int, int, int >reply =
        service->gsmSim.GetPhonebookInfo("contacts");

    if (!checkReply(reply)) {
        return;
    }
    QPhoneBookLimits l;
    l.setUsed(numUsed);
    l.setFirstIndex(1);
    l.setLastIndex(reply.argumentAt(0).toInt());
    l.setNumberLength(reply.argumentAt(1).toInt());
    l.setTextLength(reply.argumentAt(2).toInt());
    emit limits(store, l);
}

void OFonoPhoneBook::requestFixedDialingState()
{
    qDebug() << "OFonoPhoneBook::requestFixedDialingState";
    emit fixedDialingState(fixedDialingEnabled);
}

void OFonoPhoneBook::setFixedDialingState(bool enabled, const QString &)
{
    qDebug() << "OFonoPhoneBook::setFixedDialingState enabled=" << enabled;
    fixedDialingEnabled = enabled;
    emit setFixedDialingStateResult(QTelephony::OK);
}
