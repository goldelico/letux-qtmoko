#include "org.freesmartphone.GSM.NetworkProvider.h"

OrgFreesmartphoneGSMNetworkProvider::OrgFreesmartphoneGSMNetworkProvider()
{
}

void OrgFreesmartphoneGSMNetworkProvider::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMNetworkProvider>("OrgFreesmartphoneGSMNetworkProvider");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMNetworkProvider>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMNetworkProvider & provider)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMNetworkProvider & provide)
{
    argument.beginStructure();
    argument.endStructure();

    return argument;
}
