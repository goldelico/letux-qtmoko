#include "org.freesmartphone.GSM.NetworkProviderList.h"

void OrgFreesmartphoneGSMNetworkProviderList::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMNetworkProviderList>("OrgFreesmartphoneGSMNetworkProviderList");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMNetworkProviderList>();
}
