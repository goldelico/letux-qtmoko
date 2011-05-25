#include "qfsowakeupalarm.h"

QFsoWakeupAlarm::QFsoWakeupAlarm()
{
}

void QFsoWakeupAlarm::registerMetaType()
{
    qRegisterMetaType<QFsoWakeupAlarm>("QFsoWakeupAlarm");
    qDBusRegisterMetaType<QFsoWakeupAlarm>();
    qRegisterMetaType<QFsoWakeupAlarmList>("QFsoWakeupAlarmList");
    qDBusRegisterMetaType<QFsoWakeupAlarmList>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWakeupAlarm & value)
{
    argument.beginStructure();
    argument << value.busname;
    argument << value.timestamp;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWakeupAlarm & value)
{
    argument.beginStructure();
    argument >> value.busname;
    argument >> value.timestamp;
    argument.endStructure();
    return argument;
}

