#include "qfsosimhomezone.h"

QFsoSIMHomeZone::QFsoSIMHomeZone()
{
}

void QFsoSIMHomeZone::registerMetaType()
{
    qRegisterMetaType<QFsoSIMHomeZone>("QFsoSIMHomeZone");
    qDBusRegisterMetaType<QFsoSIMHomeZone>();
    qRegisterMetaType<QFsoSIMHomeZoneList>("QFsoSIMHomeZoneList");
    qDBusRegisterMetaType<QFsoSIMHomeZoneList>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMHomeZone & value)
{
    argument.beginStructure();
    argument << value.name;
    argument << value.x;
    argument << value.y;
    argument << value.radius;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMHomeZone & value)
{
    argument.beginStructure();
    argument >> value.name;
    argument >> value.x;
    argument >> value.y;
    argument >> value.radius;
    argument.endStructure();
    return argument;
}

