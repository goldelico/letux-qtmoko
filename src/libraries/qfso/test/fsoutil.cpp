#include "fsoutil.h"

FsoUtil::FsoUtil(QObject *parent) : QObject(parent)
        , pendingCall(QDBusPendingReply<>())
        , pendingNotified(true)
        , pendingReceiver(NULL)
        , checkInterval(10)
{
}

FsoUtil::~FsoUtil()
{
}

FsoUtil FsoUtil::instance;

void FsoUtil::pendingCheck()
{
    QDBusPendingReply<> reply = pendingCall;
    if(!reply.isFinished())
    {
        checkInterval += 10;
        QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
    }
    else
    {
        pendingNotified = true;
        emit finished(pendingCall);
    }
}

void FsoUtil::watchCall(QFsoDBusPendingCall & call,
                        const QObject * receiver,
                        const char * finishedMethod)
{
    if(!pendingNotified)
    {
        QFsoDBusPendingReply<> pendingReply = pendingCall;
        qDebug() << "watchCall: waiting for finish of " << pendingCall.methodCall;
        pendingReply.waitForFinished();
        qDebug() << "watchCall: finished " << pendingCall.methodCall;
        emit finished(pendingCall);
    }

    if(receiver != pendingReceiver)
    {
        disconnect();
        QObject::connect(this, SIGNAL(finished(QFsoDBusPendingCall &)),
                         receiver, finishedMethod);

        pendingReceiver = receiver;
    }

    pendingNotified = false;
    pendingCall = call;
    checkInterval = 10;

    QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
}

void watchCall(QFsoDBusPendingCall & call,
               const QObject * receiver,
               const char * finishedMethod)
{
    FsoUtil::instance.watchCall(call, receiver, finishedMethod);
}
