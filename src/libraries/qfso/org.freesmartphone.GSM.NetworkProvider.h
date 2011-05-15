#ifndef ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_H
#define ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_H

#include <QtDBus>

class OrgFreesmartphoneGSMNetworkProvider
{
public:
    explicit OrgFreesmartphoneGSMNetworkProvider();

    QString status;
    QString shortname;
    QString longname;
    QString mccmnc;
    QString act;

    friend QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMNetworkProvider & provider);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMNetworkProvider & provider);

    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMNetworkProvider)

#endif // ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_H
