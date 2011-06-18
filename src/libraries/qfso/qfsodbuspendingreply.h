#ifndef QFSODBUSPENDINGREPLY_H
#define QFSODBUSPENDINGREPLY_H

#include <QtDBus>
#include "qfsodbuspendingcall.h"

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

template<typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void,
         typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void>
class QFSO_EXPORT QFsoDBusPendingReply : public QFsoDBusPendingCall
{
public:
    QDBusPendingReply<T1,T2,T3,T4,T5,T6,T7,T8> reply;

    inline QFsoDBusPendingReply()
        : QFsoDBusPendingCall(QDBusPendingReply<>())
    {
    }

    inline QFsoDBusPendingReply(const QFsoDBusPendingCall &call)
        : QFsoDBusPendingCall(call)
    {
        reply = call;
        this->method = call.method;
        this->methodCall = call.methodCall;
    }

    inline T1 value()
    {
        return reply.value();
    }

    inline QVariant argumentAt(int index) const
    {
        return reply.argumentAt(index);
    }
};


#endif // QFSODBUSPENDINGREPLY_H
