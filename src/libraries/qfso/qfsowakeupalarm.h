#ifndef QFSOWAKEUPALARM_H
#define QFSOWAKEUPALARM_H
#include <QtDBus>

class QFsoWakeupAlarm
{
public:
    explicit QFsoWakeupAlarm();
    QString busname;
    int timestamp;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWakeupAlarm & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWakeupAlarm & value);
};

Q_DECLARE_METATYPE(QFsoWakeupAlarm)

typedef QList<QFsoWakeupAlarm> QFsoWakeupAlarmList;
Q_DECLARE_METATYPE(QFsoWakeupAlarmList)

#endif // QFSOWAKEUPALARM_H

