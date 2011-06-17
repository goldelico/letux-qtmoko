#ifndef QFSODBUSPENDINGCALL_H
#define QFSODBUSPENDINGCALL_H

#include <QDBusPendingCall>

class QFsoDBusPendingCall : public QDBusPendingCall
{
public:
    QFsoDBusPendingCall(const QDBusPendingCall &other);

    QString debug;      // debug info - method call as string
};

#endif // QFSODBUSPENDINGCALL_H
