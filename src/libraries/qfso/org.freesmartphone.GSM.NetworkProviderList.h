#ifndef ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H
#define ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H

#include <QtDBus>
#include <QList>
#include "org.freesmartphone.GSM.NetworkProvider.h"

class OrgFreesmartphoneGSMNetworkProviderList : public QList<OrgFreesmartphoneGSMNetworkProvider>
{
public:
    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMNetworkProviderList)

#endif // ORG_FREESMARTPHONE_GSM_NETWORK_PROVIDER_LIST_H
