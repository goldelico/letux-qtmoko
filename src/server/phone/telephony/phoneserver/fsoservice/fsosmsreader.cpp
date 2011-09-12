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
    , numSlots(-1)
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

    numSlots = service->sim_info.info.value("slots").toInt();
    if (numSlots <= 0) {
        QTimer::singleShot(30000, this, SLOT(test()));  // sim info not available, try again later
        return;
    }
    // Fill messages with values from SIM
    messages.clear();
    index = 0;

    QFsoDBusPendingCall call = service->gsmSim.RetrieveMessage(0);
    watchFsoCall(call, this,
                 SLOT(retrieveMessageFinished(QFsoDBusPendingCall &)));
}

void FsoSMSReader::retrieveMessageFinished(QFsoDBusPendingCall & call)
{
    QFsoDBusPendingReply < QString, QString, QString, QVariantMap > reply =
        call;
    if (checkReply(reply)) {
        QFsoSIMMessage f;
        f.index = index;
        f.status = reply.argumentAt(0).toString();
        f.number = reply.argumentAt(1).toString();
        f.contents = reply.argumentAt(2).toString();
        f.properties = qdbus_cast < QVariantMap > (reply.argumentAt(3));
        f.timestamp = f.properties.value("timestamp").toString();
        messages.append(f);
    }
    index++;
    if (index >= numSlots) {
        qDebug() << "emit messageCount=" << messages.count();
        emit messageCount(messages.count());
        return;
    }
    QFsoDBusPendingCall nextCall = service->gsmSim.RetrieveMessage(index);
    watchFsoCall(nextCall, this,
                 SLOT(retrieveMessageFinished(QFsoDBusPendingCall &)));
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
    for (int i = 0; i < messages.count(); i++) {
        QFsoSIMMessage f = messages.at(i);
        QString msgId = getMsgId(f.contents, f.timestamp);
        if (msgId != id) {
            continue;
        }
        QFsoDBusPendingReply <> delReply =
            service->gsmSim.DeleteMessage(f.index);
        checkReply(delReply);
    }
}

void FsoSMSReader::setUnreadCount(int value)
{
    qDebug() << "FsoSMSReader::setUnreadCount() value=" << value;
}

void FsoSMSReader::incomingTextMessage(const QString & number,
                                       const QString & timestamp,
                                       const QString & contents)
{
    qDebug() << "FsoSMSReader::incomingTextMessage() number=" << number <<
        ", timestamp=" << timestamp << ", contents=" << contents;
    check();
}

void FsoSMSReader::incomingMessage(int index)
{
    qDebug() << "FsoSMSReader::incomingMessage() index=" << index;
    check();
}

