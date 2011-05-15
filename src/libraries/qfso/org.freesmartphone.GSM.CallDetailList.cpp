#include "org.freesmartphone.GSM.CallDetailList.h"

OrgFreesmartphoneGSMCallDetailList::OrgFreesmartphoneGSMCallDetailList()
{
}

void OrgFreesmartphoneGSMCallDetailList::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMCallDetailList>("OrgFreesmartphoneGSMCallDetailList");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMCallDetailList>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMCallDetailList & list)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMCallDetailList & list)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}
