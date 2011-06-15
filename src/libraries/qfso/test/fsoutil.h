#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>

template <class T, class T2, class T3>
        int checkReply2(QDBusPendingReply<T, T2, T3> & reply,
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

class FsoUtil : public QObject {
    Q_OBJECT
public:
    FsoUtil(QObject *parent = 0);
    ~FsoUtil();

public:
    QDBusPendingReply<> pendingReply;
    bool pendingNotified;
    const QObject * pendingReceiver;
    int checkInterval;

    static FsoUtil instance;

    template <class T, class T2, class T3>
            void watchCall(QDBusPendingReply<T,T2,T3> & reply,
                           const QObject * receiver,
                           const char * finishedMethod);

Q_SIGNALS:
    void finished(QDBusPendingReply<> *reply);

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
        emit finished(&pendingReply);
    }
    pendingNotified = false;
    pendingReply = reply;
    checkInterval = 10;

    QObject::connect(this, SIGNAL(finished(QDBusPendingReply<> *)),
                     receiver, finishedMethod);

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
