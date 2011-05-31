#ifndef QFSOWAKEUPALARM_H
#define QFSOWAKEUPALARM_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoWakeupAlarm
{
public:
    explicit QFsoWakeupAlarm();
    QString busname;
    int timestamp;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWakeupAlarm & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWakeupAlarm & value);
};

Q_DECLARE_METATYPE(QFsoWakeupAlarm)

typedef QList<QFsoWakeupAlarm> QFsoWakeupAlarmList;
Q_DECLARE_METATYPE(QFsoWakeupAlarmList)

#endif // QFSOWAKEUPALARM_H

