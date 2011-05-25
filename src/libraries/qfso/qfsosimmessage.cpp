#include "qfsosimmessage.h"

QFsoSIMMessage::QFsoSIMMessage()
{
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMMessage & value)
{
    argument.beginStructure();
    argument << value.index;
    argument << value.status;
    argument << value.number;
    argument << value.timestamp;
    argument << value.contents;
    argument << value.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMMessage & value)
{
    argument.beginStructure();
    argument >> value.index;
    argument >> value.status;
    argument >> value.number;
    argument >> value.timestamp;
    argument >> value.contents;
    argument >> value.properties;
    argument.endStructure();
    return argument;
}

