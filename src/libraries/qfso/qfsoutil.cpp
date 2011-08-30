#include "qfsoutil.h"

QFsoUtil::QFsoUtil(QObject *parent) : QObject(parent)
        , pendingCall(QDBusPendingReply<>())
        , pendingNotified(true)
        , pendingReceiver(NULL)
        , pendingFinished(NULL)
        , checkInterval(10)
{
}

QFsoUtil::~QFsoUtil()
{
}

QFsoUtil QFsoUtil::instance;

void QFsoUtil::emitFinished(QObject *receiver, const char *finishedMethod, QFsoDBusPendingCall &call)
{
    disconnect();
    QObject::connect(this, SIGNAL(finished(QFsoDBusPendingCall &)),
                     receiver, finishedMethod);
    emit finished(call);

   //QMetaObject::invokeMethod(receiver, "getStatusFinished", Qt::DirectConnection,
   //                          Q_ARG(QFsoDBusPendingCall &, call));
}

void QFsoUtil::pendingCheck()
{
    if(pendingNotified)
    {
        return;
    }

    QDBusPendingReply<> reply = pendingCall;
    if(!reply.isFinished())
    {
        checkInterval += 10;
        QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
    }
    else
    {
        pendingNotified = true;
        emitFinished(pendingReceiver, pendingFinished, pendingCall);
    }
}

void QFsoUtil::watchCall(QFsoDBusPendingCall & call,
                         QObject * receiver,
                         const char * finishedMethod)
{
    bool oldNotified = pendingNotified;
    QFsoDBusPendingCall oldCall = pendingCall;
    QObject *oldReceiver = pendingReceiver;
    const char *oldFinished = pendingFinished;

    pendingNotified = false;
    pendingCall = call;
    pendingReceiver = receiver;
    pendingFinished = finishedMethod;

    checkInterval = 10;
    QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));

    if(!oldNotified)
    {
        qDebug() << "watchCall: waiting for finish of " << oldCall.methodCall;
        oldCall.waitForFinished();
        qDebug() << "watchCall: " << oldCall.methodCall << " finished";
        emitFinished(oldReceiver, oldFinished, oldCall);
    }
}

void QFsoUtil::waitForFinished()
{
    instance.pendingCall.waitForFinished();
}

void watchFsoCall(QFsoDBusPendingCall & call,
                  QObject * receiver,
                  const char * finishedMethod)
{
    QFsoUtil::instance.watchCall(call, receiver, finishedMethod);
}
