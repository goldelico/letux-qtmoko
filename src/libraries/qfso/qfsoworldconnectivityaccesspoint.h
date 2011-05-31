#ifndef QFSOWORLDCONNECTIVITYACCESSPOINT_H
#define QFSOWORLDCONNECTIVITYACCESSPOINT_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoWorldConnectivityAccessPoint
{
public:
    explicit QFsoWorldConnectivityAccessPoint();
    QString description;
    QString apn;
    QString username;
    QString password;
    QString dns1;
    QString dns2;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWorldConnectivityAccessPoint & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWorldConnectivityAccessPoint & value);
};

Q_DECLARE_METATYPE(QFsoWorldConnectivityAccessPoint)

typedef QList<QFsoWorldConnectivityAccessPoint> QFsoWorldConnectivityAccessPointList;
Q_DECLARE_METATYPE(QFsoWorldConnectivityAccessPointList)

#endif // QFSOWORLDCONNECTIVITYACCESSPOINT_H

