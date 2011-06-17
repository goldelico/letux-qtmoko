#include "qfsodbuspendingcall.h"

QFsoDBusPendingCall::QFsoDBusPendingCall(const QDBusPendingCall &other)
    : QDBusPendingCall(other)
    , debug()
{
}
