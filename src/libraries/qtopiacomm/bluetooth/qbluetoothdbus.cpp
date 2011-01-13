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

#include "qbluetoothdbus.h"

QBluetoothDbusIface::QBluetoothDbusIface(const QString &service, const QString &path, const char *interface,
                                         const QDBusConnection &connection, QObject *parent) :
QDBusInterface(service, path, interface, connection, parent)
{
}

QVariant QBluetoothDbusIface::getProperty(QString name, QDBusError * error)
{
    QDBusReply< QMap<QString,QVariant> > reply;
    
    btcall("GetProperties", reply);

    if(reply.isValid()) {
        QVariant value = reply.value().value(name);
        qLog(Bluetooth) << "        " << name + "=" + value.toString();
        return value;
    }
           
    if(error) {
        *error = reply.error();
    }
    return QVariant();
}

bool QBluetoothDbusIface::setProperty(QString name, QVariant value, QDBusError * error)
{
    QList<QVariant> args;
    QDBusReply<void> reply;

    args << name;
    args << qVariantFromValue(QDBusVariant(value));

    if(btcall<void>("SetProperty", reply, args))
        return true;
    
    if(error) {
        *error = reply.error();
    }
    return false;
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
    
    return btcall("SetProperty", reply, args, true, receiver, returnMethod, errorMethod);
}


void QBluetoothDbusIface::asyncReply(const QDBusMessage &)
{
}

void QBluetoothDbusIface::asyncErrorReply(const QDBusError &error, const QDBusMessage &)
{
    qWarning() << "asyncErrorReply " << error;
}

#include "qbluetoothdbus.moc"
