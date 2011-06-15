#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>

template <class T, class T2, class T3>
        int checkReply(QDBusPendingReply<T, T2, T3> & reply,
                       const QString & fn,
                       bool waitForFinished = true,
                       int ok = 1,
                       int err = 0,
                       int unfinished = 0)
{
    if(waitForFinished)
    {
        reply.waitForFinished();
    }
    if(reply.isError())
    {
        QString errorStr = reply.error().message();
        qWarning() << "Error in " << fn << errorStr;
        return err;
    }
    if(reply.isFinished() && reply.isValid())
    {
        qDebug() << "    dbus call " + fn + "() ok";
        return ok;
    }
    return unfinished;
}

// This class is used to implement watchCall mechanism which allows us to
// call callback method when dbus call is finished. It's similar to
// QDBusPendingCallWatcher, which for some reason sometimes does not work.
class FsoUtil : public QObject {
    Q_OBJECT
public:
    FsoUtil(QObject *parent = 0);
    ~FsoUtil();

public:
    QDBusPendingReply<> pendingReply;           // reply for current call, we can have just one pending call at a time
    bool pendingNotified;                       // true if we already signalled finish
    const QObject * pendingReceiver;            // object to receive finish signal
    int checkInterval;                          // we start checking with small intervals and then make them longer

    static FsoUtil instance;

    template <class T, class T2, class T3>
            void watchCall(QDBusPendingReply<T,T2,T3> & reply,
                           const QObject * receiver,
                           const char * finishedMethod);

Q_SIGNALS:
    void finished(QDBusPendingReply<> & reply);

private slots:
    void pendingCheck();
};

template <class T, class T2, class T3>
        void FsoUtil::watchCall(QDBusPendingReply<T,T2,T3> & reply,
                                const QObject * receiver,
                                const char * finishedMethod)
{
    if(!pendingNotified)
    {
        pendingReply.waitForFinished();
        emit finished(pendingReply);
    }

    if(receiver != pendingReceiver)
    {
        disconnect();
        QObject::connect(this, SIGNAL(finished(QDBusPendingReply<> &)),
                     receiver, finishedMethod);

        pendingReceiver = receiver;
    }

    pendingNotified = false;
    pendingReply = reply;
    checkInterval = 10;

    QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
}

template <class T, class T2, class T3>
        void watchCall(QDBusPendingReply<T,T2,T3> & reply,
                       const QObject * receiver,
                       const char * finishedMethod)
{
    FsoUtil::instance.watchCall(reply, receiver, finishedMethod);
}

#endif // FSOUTIL_H
