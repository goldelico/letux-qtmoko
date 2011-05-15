#include "org.freesmartphone.GSM.NetworkProviderList.h"

OrgFreesmartphoneGSMNetworkProviderList::OrgFreesmartphoneGSMNetworkProviderList()
{
}

void OrgFreesmartphoneGSMNetworkProviderList::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMNetworkProviderList>("OrgFreesmartphoneGSMNetworkProviderList");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMNetworkProviderList>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMNetworkProviderList & list)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMNetworkProviderList & list)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}
