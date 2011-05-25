#include "qfsoworldcountry.h"

QFsoWorldCountry::QFsoWorldCountry()
{
}

void QFsoWorldCountry::registerMetaType()
{
    qRegisterMetaType<QFsoWorldCountry>("QFsoWorldCountry");
    qDBusRegisterMetaType<QFsoWorldCountry>();
    qRegisterMetaType<QFsoWorldCountryList>("QFsoWorldCountryList");
    qDBusRegisterMetaType<QFsoWorldCountryList>();
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

