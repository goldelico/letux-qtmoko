#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include "qfsodbuspendingcall.h"
#include "qfsodbuspendingreply.h"

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
    QFsoDBusPendingCall pendingCall;            // current call, we can have just one pending call at a time
    bool pendingNotified;                       // true if we already signalled finish
    const QObject * pendingReceiver;            // object to receive finish signal
    int checkInterval;                          // we start checking with small intervals and then make them longer

    static FsoUtil instance;

    void watchCall(QFsoDBusPendingCall & call,
                   const QObject * receiver,
                   const char * finishedMethod);

Q_SIGNALS:
    void finished(QFsoDBusPendingCall & call);

private slots:
    void pendingCheck();
};

void watchCall(QFsoDBusPendingCall & call,
               const QObject * receiver,
               const char * finishedMethod);

#endif // FSOUTIL_H
