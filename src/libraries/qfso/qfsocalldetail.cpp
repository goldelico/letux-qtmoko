#include "qfsocalldetail.h"

QFsoCallDetail::QFsoCallDetail()
{
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoCallDetail & value)
{
    argument.beginStructure();
    argument << value.id;
    argument << value.status;
    argument << value.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoCallDetail & value)
{
    argument.beginStructure();
    argument >> value.id;
    argument >> value.status;
    argument >> value.properties;
    argument.endStructure();
    return argument;
}

