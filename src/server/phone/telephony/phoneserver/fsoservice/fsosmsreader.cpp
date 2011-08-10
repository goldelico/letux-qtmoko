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
    , msgQuery("", "", QDBusConnection::systemBus(), this)
    , resultIndex(-1)
    , resultCount(0)
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
    
    qDebug() << "msgQuery.service()=" << msgQuery.service();
    
    // Construct query for the first time
    if(msgQuery.service().length() == 0)
    {
        QFsoDBusPendingReply<QString> reply = service->pimMsg.Query(QVariantMap());
        if(!checkResult(reply))
        {
            return;
        }
        QString path = reply.value();
        new (&msgQuery) QFsoPIMMessageQuery("org.freesmartphone.opimd", path, QDBusConnection::systemBus(), this);
    }
    
    QFsoDBusPendingCall call = msgQuery.GetResultCount();
    watchFsoCall(call, this, SLOT(getResultCountFinished(QFsoDBusPendingCall &)));
}

void FsoSMSReader::getResultCountFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply<int> reply = call;
    if(!checkResult(reply))
    {
        return;
    }
    resultCount = reply.value();
    emit messageCount(resultCount);
}

static QString fillMsg(QVariantMap & map, int index, QSMSMessage & m)
{
    QString dump;
    for(int i = 0; i < map.count(); i++)
    {
        QString key = map.keys().at(i);
        dump += " " + key + "=" + map.value(key).toString();
    }
    qDebug() << "fillMsg index=" << index << ", map=" << dump;
    
    QString direction = map.value("Direction").toString();
    QString timestamp = map.value("Timestamp").toString();
    uint secs = map.value("Timestamp").toUInt();
    QString text = map.value("Content").toString();
    QByteArray textBytes = text.toLatin1();
    quint16 crc = qChecksum(textBytes.constData(), textBytes.length());
    
    m.setText(text);
    m.setSender(map.value("Peer").toString());
    m.setTimestamp(QDateTime::fromTime_t(secs));
    
    return timestamp + ":" + QString::number(crc);
}

void FsoSMSReader::firstMessage()
{
    qDebug() << "FsoSMSReader::firstMessage()";
    QFsoDBusPendingReply<> reply = msgQuery.Rewind();
    if(!checkResult(reply))
    {
        return;
    }
    resultIndex = 0;
    nextMessage();
}

void FsoSMSReader::getResultFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply<QVariantMap> reply = call;
    if(!checkResult(reply))
    {
        return;
    }
    QSMSMessage msg;
    QVariantMap map = reply.value();
    QString id = fillMsg(map, ++resultIndex, msg);
    emit fetched(id, msg);
}

void FsoSMSReader::nextMessage()
{
    qDebug() << "FsoSMSReader::nextMessage()";
    if(resultIndex < resultCount)
    {   
        QFsoDBusPendingCall call = msgQuery.GetResult();
        watchFsoCall(call, this, SLOT(getResultFinished(QFsoDBusPendingCall &)));
    }
    else
    {
        QSMSMessage dummy;
        emit fetched("", dummy);
    }
}

void FsoSMSReader::deleteMessage( const QString& id )
{
    qDebug() << "FsoSMSReader::deleteMessage() id=" << id;
}

void FsoSMSReader::setUnreadCount( int value )
{
    qDebug() << "FsoSMSReader::setUnreadCount() value=" << value;
}