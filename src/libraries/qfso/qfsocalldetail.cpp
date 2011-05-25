#include "qfsocalldetail.h"

QFsoCallDetail::QFsoCallDetail()
{
}

void QFsoCallDetail::registerMetaType()
{
    qRegisterMetaType<QFsoCallDetail>("QFsoCallDetail");
    qDBusRegisterMetaType<QFsoCallDetail>();
    qRegisterMetaType<QFsoCallDetailList>("QFsoCallDetailList");
    qDBusRegisterMetaType<QFsoCallDetailList>();
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

