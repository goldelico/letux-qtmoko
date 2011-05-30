#include "qfsodbusabstractinterface.h"

QFsoDbusAbstractInterface::QFsoDbusAbstractInterface(const QString &service, const QString &path, const char *interface,
                                                     const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, interface, connection, parent)
{
}

QFsoDbusAbstractInterface::~QFsoDbusAbstractInterface()
{
}

QDBusPendingCall QFsoDbusAbstractInterface::fsoAsyncCall(const QString &method, const QList<QVariant> &args)
{
//    qDebug() << "calling " << method;
    return asyncCallWithArgumentList(method, args);
}
