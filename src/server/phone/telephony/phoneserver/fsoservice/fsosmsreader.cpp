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

#include "fsosmsreader.h"
#include "fsotelephonyservice.h"

FsoSMSReader::FsoSMSReader( FsoTelephonyService *service )
    : QSMSReader( service->service(), service, QCommInterface::Server )
    , service(service)
    , smsId()
    , messages()
    , pimMessages()
    , msgQuery("", "", QDBusConnection::systemBus(), this)
    , messageIndex(-1)
{
    QTimer::singleShot(40000, this, SLOT(test()));
}

FsoSMSReader::~FsoSMSReader()
{
}

void FsoSMSReader::test()
{
    check();
    //qDebug() << "FsoSMSReader::test()";
    //setReady(true);
    //emit messageCount(10);
    //setUnreadCount(10);
}

void FsoSMSReader::deviceStatus(QString status)
{
    bool oldReady = ready();
    bool newReady = status == "alive-registered";
    if(oldReady == newReady)
    {
        return;
    }
    setReady(newReady);
    if(newReady)
    {
        check();
    }
}

void FsoSMSReader::check()
{
    qDebug() << "FsoSMSReader::check()";
    
    /* RetrieveTextMessages kills ogsmd for me
    QFsoDBusPendingCall call = service->gsmSms.RetrieveTextMessages();
    watchFsoCall(call, this, SLOT(retrieveTextMessagesFinished(QFsoDBusPendingCall &))); */
    
    QFsoDBusPendingReply<QString> reply = service->pimMsg.Query(QVariantMap());
    if(!checkResult(reply))
    {
        return;
    }
    QString path = reply.value();
    new (&msgQuery) QFsoPIMMessageQuery("org.freesmartphone.opimd", path, QDBusConnection::systemBus(), this);
    
    QFsoDBusPendingCall call = msgQuery.GetMultipleResults(-1);
    watchFsoCall(call, this, SLOT(getMultipleResultsFinished(QFsoDBusPendingCall &)));
}

void FsoSMSReader::getMultipleResultsFinished(QFsoDBusPendingCall & call)
{
    qDebug() << "getMultipleResultsFinished";
    QFsoDBusPendingReply<QFsoVariantMapList> reply = call;
    if(!checkResult(reply))
    {
        return;
    }
    pimMessages = reply.value();
    qDebug() << "pimMessages.count()=" << pimMessages.count();
    emit messageCount(pimMessages.count());
}

void FsoSMSReader::retrieveTextMessagesFinished(QFsoDBusPendingCall & call)
{
    qDebug() << "retrieveTextMessagesFinished";
    QFsoDBusPendingReply<QFsoSIMMessageList> reply = call;
    if(checkResult(reply))
    {
        messages = reply.value();
        emit messageCount(messages.count());
    }
}

static QString fillMsg(QFsoVariantMapList & pimMessages, int index, QSMSMessage & m)
{
    if(index >= pimMessages.count())
    {
        return "";
    }
    QVariantMap map = pimMessages.at(index);
    QString direction = map.value("Direction").toString();
    QString timestamp = map.value("Timestamp").toString();
    uint secs = map.value("Timestamp").toUInt();
    m.setText(map.value("Content").toString());    
    m.setSender(map.value("Peer").toString());
    m.setTimestamp(QDateTime::fromTime_t(secs));
    return timestamp;
}

void FsoSMSReader::firstMessage()
{
    qDebug() << "FsoSMSReader::firstMessage()";
    messageIndex = 0;
    QSMSMessage msg;
    QString id = fillMsg(pimMessages, 0, msg);
    emit fetched(id, msg);
}

void FsoSMSReader::nextMessage()
{
    qDebug() << "FsoSMSReader::nextMessage()";
    QSMSMessage msg;
    QString id = fillMsg(pimMessages, ++messageIndex, msg);
    emit fetched(id, msg);
}

void FsoSMSReader::deleteMessage( const QString& id )
{
    qDebug() << "FsoSMSReader::deleteMessage() id=" << id;
}

void FsoSMSReader::setUnreadCount( int value )
{
    qDebug() << "FsoSMSReader::setUnreadCount() value=" << value;
}