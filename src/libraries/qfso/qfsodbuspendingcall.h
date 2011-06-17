#ifndef QFSODBUSPENDINGCALL_H
#define QFSODBUSPENDINGCALL_H

#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoDBusPendingCall : public QDBusPendingCall
{
public:
    QFsoDBusPendingCall(const QDBusPendingCall &other);

    QString debug;      // debug info - method call as string
};

#endif // QFSODBUSPENDINGCALL_H
