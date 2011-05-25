#ifndef QFSONETWORKPROVIDER_H
#define QFSONETWORKPROVIDER_H
#include <QtDBus>

class QFsoNetworkProvider
{
public:
    explicit QFsoNetworkProvider();
    QString status;
    QString shortname;
    QString longname;
    QString mccmnc;
    QString act;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoNetworkProvider & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoNetworkProvider & value);
    static void registerMetaType();
};

Q_DECLARE_METATYPE(QFsoNetworkProvider)

typedef QList<QFsoNetworkProvider> QFsoNetworkProviderList;
Q_DECLARE_METATYPE(QFsoNetworkProviderList)

#endif // QFSONETWORKPROVIDER_H

