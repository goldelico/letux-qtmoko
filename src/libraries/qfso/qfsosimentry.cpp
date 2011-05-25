#include "qfsosimentry.h"

QFsoSIMEntry::QFsoSIMEntry()
{
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMEntry & value)
{
    argument.beginStructure();
    argument << value.index;
    argument << value.name;
    argument << value.number;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMEntry & value)
{
    argument.beginStructure();
    argument >> value.index;
    argument >> value.name;
    argument >> value.number;
    argument.endStructure();
    return argument;
}

