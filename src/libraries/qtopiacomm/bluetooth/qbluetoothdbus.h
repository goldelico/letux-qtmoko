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

#ifndef QBLUETOOTHDBUS_H
#define QBLUETOOTHDBUS_H

#include <qbluetoothglobal.h>
#include <qbluetoothnamespace.h>

#include <qobject.h>
#include <qglobal.h>
#include <QString>
#include <QList>
#include <QDBusInterface>
#include <QDBusReply>

/*
 * Helper class for calling bluez4 dbus interface, It can log dbus calls and
 * handle errors.
 */
class QBluetoothDbusIface : public QObject
{
    Q_OBJECT

public:
    
    QBluetoothDbusIface(const QString & interface);
    
    template <class T>
            bool call(const QString & method,
                      QList<QVariant> args,
                      QDBusReply<T> & reply = QDBusReply<T>(),
                      bool async = false,
                      QObject * receiver = NULL,
                      const char * returnMethod = NULL,
                      const char * errorMethod = NULL);

    QVariant getProperty(QString name);
    bool setProperty(QString name, QVariant value);
    bool setPropertyAsync(QString name, 
                          QVariant value,
                          QObject * receiver,
                          const char * returnMethod,
                          const char * errorMethod);

private:
    QString m_interface;
    QDBusInterface m_iface;

    void asyncReply(const QDBusMessage &msg);
    void asyncErrorReply(const QDBusError &error, const QDBusMessage &msg);        
};

#endif
