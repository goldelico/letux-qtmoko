#include "qfsonetworkprovider.h"

QFsoNetworkProvider::QFsoNetworkProvider()
{
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoNetworkProvider & value)
{
    argument.beginStructure();
    argument << value.status;
    argument << value.shortname;
    argument << value.longname;
    argument << value.mccmnc;
    argument << value.act;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoNetworkProvider & value)
{
    argument.beginStructure();
    argument >> value.status;
    argument >> value.shortname;
    argument >> value.longname;
    argument >> value.mccmnc;
    argument >> value.act;
    argument.endStructure();
    return argument;
}

