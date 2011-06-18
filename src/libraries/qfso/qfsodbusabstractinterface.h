#ifndef QFSODBUSABSTRACTINTERFACE_H
#define QFSODBUSABSTRACTINTERFACE_H

#include <QtDBus/QtDBus>
#include <qfsodbuspendingcall.h>
#include <qfsodbuspendingreply.h>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

/*
 * Base class for FSO dbus interfaces, useful for logging and similar
 * customizations.
 */
class QFSO_EXPORT QFsoDbusAbstractInterface: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    QFsoDbusAbstractInterface(const QString &service, const QString &path, const char *interface,
                              const QDBusConnection &connection, QObject *parent);

    ~QFsoDbusAbstractInterface();

    QFsoDBusPendingCall fsoAsyncCall(const QString &method, const QList<QVariant> &args);

};

#endif // QFSODBUSABSTRACTINTERFACE_H
