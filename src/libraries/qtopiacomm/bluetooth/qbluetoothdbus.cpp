/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak.
**
** Contact: psonek2@seznam.cz
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#include "qbluetoothnamespace_p.h"
#include "qbluetoothdbus.h"

#include <QStringList>
#include <QDateTime>

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QSet>
#include <QDebug>

#include <qglobal.h>
#include <qtopialog.h>
#include <stdio.h>
#include <string.h>

#include <QMetaObject>

QBluetoothDbusIface::QBluetoothDbusIface(const QString &service, const QString &path, const char *interface,
                        const QDBusConnection &connection, QObject *parent) :
        QDBusInterface(service, path, interface, connection, parent)
{
}

template <class T>
        bool QBluetoothDbusIface::call(const QString & method,
                                       QList<QVariant> args,
                                       QDBusReply<T> & reply,
                                       bool async,
                                       QObject * receiver,
                                       const char * returnMethod,
                                       const char * errorMethod)
{
    if(!isValid()) {
        qWarning() << "Dbus interface " << path() << " is not valid";
        return true;
    }

    QString methodStr(path() + "->" + method + "(");
    for(int i = 0; i < args.count(); i++)
    {
        QVariant arg = args.at(i);
        if(i > 0)
            methodStr += ", ";
        if(arg.canConvert<QDBusVariant>())
            methodStr += arg.value<QDBusVariant>().variant().toString();
        else
            methodStr += args.at(i).toString();
    }
    methodStr += ")";
    qLog(Bluetooth) << "calling " << methodStr;
    
    if(async) {
        if(receiver == NULL)
            receiver = this;
        if(returnMethod == NULL)
            returnMethod = SLOT(asyncReply(QDBusMessage));
        if(errorMethod == NULL)
            errorMethod = SLOT(asyncErrorReply(QDBusError,QDBusMessage));

        return callWithCallback(method, args, receiver, returnMethod, errorMethod);
    }
    else {
        reply = callWithArgumentList(QDBus::AutoDetect, method, args);
        if(reply.isValid())
            return true;
    }

    qWarning() << "Method call " << methodStr << " failed: " << reply.error();
    return false;
}

QVariant QBluetoothDbusIface::getProperty(QString name)
{
    QList<QVariant> args;   
    QDBusReply< QMap<QString,QVariant> > reply;
    
    call("GetProperties", args, reply);

    if(!reply.isValid())
        return QVariant();
    
    QVariant value = reply.value().value(name);
    qLog(Bluetooth) << "        " << name + "=" + value.toString();
    return value;
}

bool QBluetoothDbusIface::setProperty(QString name, QVariant value)
{
    QList<QVariant> args;
    QDBusReply<void> reply;

    args << name;
    args << qVariantFromValue(QDBusVariant(value));

    return call<void>("SetProperty", args, reply);
}

bool QBluetoothDbusIface::setPropertyAsync(QString name,
                                           QVariant value,
                                           QObject * receiver,
                                           const char * returnMethod,
                                           const char * errorMethod)
{
    QList<QVariant> args;
    QDBusReply<void> reply;

    args << name;
    args << qVariantFromValue(QDBusVariant(value));
    
    return call("SetProperty", args, reply, true, receiver, returnMethod, errorMethod);
}


void QBluetoothDbusIface::asyncReply(const QDBusMessage &)
{
}

void QBluetoothDbusIface::asyncErrorReply(const QDBusError &error, const QDBusMessage &)
{
    qWarning() << "asyncErrorReply " << error;
}

#include "qbluetoothdbus.moc"
