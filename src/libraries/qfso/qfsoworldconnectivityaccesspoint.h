#ifndef QFSOWORLDCONNECTIVITYACCESSPOINT_H
#define QFSOWORLDCONNECTIVITYACCESSPOINT_H
#include <QtDBus>

class QFsoWorldConnectivityAccessPoint
{
public:
    explicit QFsoWorldConnectivityAccessPoint();
    QString description;
    QString apn;
    QString username;
    QString password;
    QString dns1;
    QString dns2;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWorldConnectivityAccessPoint & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWorldConnectivityAccessPoint & value);
};

Q_DECLARE_METATYPE(QFsoWorldConnectivityAccessPoint)

typedef QList<QFsoWorldConnectivityAccessPoint> QFsoWorldConnectivityAccessPointList;
Q_DECLARE_METATYPE(QFsoWorldConnectivityAccessPointList)

#endif // QFSOWORLDCONNECTIVITYACCESSPOINT_H

