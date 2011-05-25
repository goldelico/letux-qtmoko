#include "qfsowakeupalarm.h"

QFsoWakeupAlarm::QFsoWakeupAlarm()
{
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

