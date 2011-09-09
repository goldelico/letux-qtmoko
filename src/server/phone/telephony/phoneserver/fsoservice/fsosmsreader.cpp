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

FsoSMSReader::FsoSMSReader(FsoTelephonyService * service)
:  QSMSReader(service->service(), service, QCommInterface::Server)
    , service(service)
    , messages()
    , index(-1)
{
    //QTimer::singleShot(40000, this, SLOT(test()));
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

    qDebug() << "FsoSMSReader::deviceStatus oldReady=" << oldReady <<
        ", newReady=" << newReady;

    if (oldReady == newReady) {
        return;
    }
    setReady(newReady);
    if (newReady) {
        check();
    }
}

void FsoSMSReader::check()
{
    qDebug() << "FsoSMSReader::check()";

    // Query for all messages
    QFsoDBusPendingCall call = service->gsmSms.RetrieveTextMessages();
    watchFsoCall(call, this,
                 SLOT(retrieveTextMessagesFinished(QFsoDBusPendingCall &)));
}

void FsoSMSReader::retrieveTextMessagesFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply < QFsoSIMMessageList > reply = call;
    if (!checkResult(reply)) {
        return;
    }
    messages = reply.value();
    qDebug() << "emit messageCount=" << messages.count();
    emit messageCount(messages.count());
}

static QString getMsgId(const QString & contents, const QString & timestamp)
{
    QByteArray contentBytes = contents.toLatin1();
    quint16 crc = qChecksum(contentBytes.constData(), contentBytes.length());
    return QString("%1:%2").arg(timestamp).arg(crc);
}

static QString fillMsg(const QFsoSIMMessage & f, QSMSMessage & m, int index)
{
    qDebug() << "fillMsg index=" << index << "f.number=" << f.number +
        ", f.index=" << f.index << ", f.contents=" << f.contents;

    m.setText(f.contents);
    m.setSender(f.number);
    m.setTimestamp(QDateTime::fromTime_t(f.timestamp.toInt()));

    return getMsgId(f.contents, f.timestamp);
}

void FsoSMSReader::firstMessage()
{
    qDebug() << "FsoSMSReader::firstMessage()";
    index = 0;
    nextMessage();
}

void FsoSMSReader::nextMessage()
{
    qDebug() << "FsoSMSReader::nextMessage()";
    QSMSMessage msg;
    QString id;
    if (index < messages.count()) {
        id = fillMsg(messages.at(index), msg, index);
        index++;
    }
    emit fetched(id, msg);
}

void FsoSMSReader::deleteMessage(const QString & id)
{
    qDebug() << "FsoSMSReader::deleteMessage() id=" << id;

    // Find message by id
    bool found = false;
    QFsoSIMMessage msg;
    for (int i = 0; i < messages.count(); i++) {
        msg = messages.at(i);
        QString msgId = getMsgId(msg.contents, msg.timestamp);
        if (msgId == id) {
            found = true;
            break;
        }
    }
    if (!found) {
        qWarning() << "FsoSMSReader::deleteMessage() - message not found id=" <<
            id;
    }
    // Delete messages with same timestamp
    int numSlots = service->sim_info.info.value("slots").toInt();
    qDebug() << "numSlots=" << numSlots;
    for (int i = 0; i < numSlots; i++) {
        QFsoDBusPendingReply < QString, QString, QString, QVariantMap > reply =
            service->gsmSim.RetrieveMessage(i);
        if (!checkResult(reply)) {
            continue;
        }
        QVariantMap map = qdbus_cast < QVariantMap > (reply.argumentAt(3));
        QString timestamp = map.value("timestamp").toString();
        if (timestamp != msg.timestamp) {
            continue;
        }
        QFsoDBusPendingReply <> delReply = service->gsmSim.DeleteMessage(i);
        checkResult(delReply);
    }
}

void FsoSMSReader::setUnreadCount(int value)
{
    qDebug() << "FsoSMSReader::setUnreadCount() value=" << value;
}

void FsoSMSReader::incomingTextMessage(const QString &, const QString &,
                                       const QString &)
{
    check();
}
