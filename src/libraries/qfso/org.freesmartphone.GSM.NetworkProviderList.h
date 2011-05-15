#ifndef ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H
#define ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H

#include <QtDBus>

class OrgFreesmartphoneGSMNetworkProviderList
{
public:
    explicit OrgFreesmartphoneGSMNetworkProviderList();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMNetworkProviderList &list);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMNetworkProviderList &list);

    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMNetworkProviderList)

#endif // ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H
