#include "qfsoworldcountry.h"

QFsoWorldCountry::QFsoWorldCountry()
{
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWorldCountry & value)
{
    argument.beginStructure();
    argument << value.code;
    argument << value.name;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWorldCountry & value)
{
    argument.beginStructure();
    argument >> value.code;
    argument >> value.name;
    argument.endStructure();
    return argument;
}

