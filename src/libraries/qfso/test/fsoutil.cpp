#include "fsoutil.h"

FsoUtil::FsoUtil(QObject *parent) : QObject(parent)
        , pendingCall(QDBusPendingCall::fromError(QDBusError()))
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

void FsoUtil::watchCall(QDBusPendingCall & call,
                        const QObject * receiver,
                        const char * finishedMethod)
{
    if(!pendingNotified)
    {
        QDBusPendingReply<> pendingReply = pendingCall;
        pendingReply.waitForFinished();
        emit finished(pendingReply);
    }

    if(receiver != pendingReceiver)
    {
        disconnect();
        QObject::connect(this, SIGNAL(finished(QDBusPendingCall &)),
                     receiver, finishedMethod);

        pendingReceiver = receiver;
    }

    pendingNotified = false;
    pendingCall = call;
    checkInterval = 10;

    QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
}

void watchCall(QDBusPendingCall & call,
               const QObject * receiver,
               const char * finishedMethod)
{
    FsoUtil::instance.watchCall(call, receiver, finishedMethod);
}
