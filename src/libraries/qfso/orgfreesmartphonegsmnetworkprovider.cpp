#include "orgfreesmartphonegsmnetworkprovider.h"

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
    argument << provider.status;
    argument << provider.shortname;
    argument << provider.longname;
    argument << provider.mccmnc;
    argument << provider.act;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMNetworkProvider & provider)
{
    argument.beginStructure();
    argument >> provider.status;
    argument >> provider.shortname;
    argument >> provider.longname;
    argument >> provider.mccmnc;
    argument >> provider.act;
    argument.endStructure();
    return argument;
}
