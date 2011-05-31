#ifndef QFSONETWORKPROVIDER_H
#define QFSONETWORKPROVIDER_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoNetworkProvider
{
public:
    explicit QFsoNetworkProvider();
    QString status;
    QString shortname;
    QString longname;
    QString mccmnc;
    QString act;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoNetworkProvider & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoNetworkProvider & value);
};

Q_DECLARE_METATYPE(QFsoNetworkProvider)

typedef QList<QFsoNetworkProvider> QFsoNetworkProviderList;
Q_DECLARE_METATYPE(QFsoNetworkProviderList)

#endif // QFSONETWORKPROVIDER_H

