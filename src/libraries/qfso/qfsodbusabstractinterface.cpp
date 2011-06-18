#include "qfsodbusabstractinterface.h"

QFsoDbusAbstractInterface::QFsoDbusAbstractInterface(const QString &service, const QString &path, const char *interface,
                                                     const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, interface, connection, parent)
{
}

QFsoDbusAbstractInterface::~QFsoDbusAbstractInterface()
{
}

QFsoDBusPendingCall QFsoDbusAbstractInterface::fsoAsyncCall(const QString &method, const QList<QVariant> &args)
{
    QString methodStr(path() + "->" + method + "(");
    for(int i = 0; i < args.count(); i++)
    {
        QVariant arg = args.at(i);
        QString argStr;
        if(i > 0)
            methodStr += ", ";
        if(arg.canConvert<QDBusVariant>())
            argStr = arg.value<QDBusVariant>().variant().toString();
        else
            argStr = args.at(i).toString();

        int lf;
        if((lf = argStr.indexOf('\n')) > 0)
            argStr = argStr.left(lf) + "...";
        methodStr += argStr;
    }
    methodStr += ")";
    qDebug() << "calling " << methodStr;

    QDBusPendingCall call = asyncCallWithArgumentList(method, args);
    QFsoDBusPendingCall res(call);
    res.methodCall = methodStr;
    res.method = method;
    return res;
}
