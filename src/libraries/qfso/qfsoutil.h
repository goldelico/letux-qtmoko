#ifndef QFSOUTIL_H
#define QFSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include "qfsodbuspendingcall.h"
#include "qfsodbuspendingreply.h"

#if defined(QFSO_LIBRARY)
#define QFSO_EXPORT Q_DECL_EXPORT
#else
#define QFSO_EXPORT Q_DECL_IMPORT
#endif

// Check reply from fso dbus call.
// Waits for reply to finish if waitForFinished is true.
// Returns value specified in ok if success.
// Returns value specified in err if error.
template <class T, class T2, class T3>
        int QFSO_EXPORT checkReply(QFsoDBusPendingReply<T, T2, T3> & reply,
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
        qWarning() << "Error in " << reply.methodCall << errorStr;
        return err;
    }
    if(reply.isFinished() && reply.isValid())
    {
        qDebug() << "    dbus call " + reply.methodCall + " ok";
        return ok;
    }
    return unfinished;
}

// This class is used to implement watchCall mechanism which allows us to
// call callback method when dbus call is finished. It's similar to
// QDBusPendingCallWatcher, which for some reason sometimes does not work
// on some methods.
//
// This implementation now allows only once pending call at a time. Next
// call will use waitForFinished() to wait until previous call ends.
class QFSO_EXPORT QFsoUtil : public QObject {
    Q_OBJECT
public:
    QFsoUtil(QObject *parent = 0);
    ~QFsoUtil();

public:
    QFsoDBusPendingCall pendingCall;            // current call, we can have just one pending call at a time
    bool pendingNotified;                       // true if we already signalled finish
    const QObject * pendingReceiver;            // object to receive finish signal
    int checkInterval;                          // we start checking with small intervals and then make them longer

    static QFsoUtil instance;

    void watchCall(QFsoDBusPendingCall & call,
                   const QObject * receiver,
                   const char * finishedMethod);

    static void waitForFinished();              // wait for current pending call to be finished

Q_SIGNALS:
    void finished(QFsoDBusPendingCall & call);

private slots:
    void pendingCheck();
};

void QFSO_EXPORT watchFsoCall(QFsoDBusPendingCall & call,
                              const QObject * receiver,
                              const char * finishedMethod);

#endif // QFSOUTIL_H
