/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
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

#include "hsointerface.h"
#include "config.h"
#include "dialstring.h"

#include <qtopialog.h>
#include <qtopianamespace.h>
#include <qtopiaipcadaptor.h>

#include <QDebug>
#include <QFile>

#include <errno.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

HsoInterface::HsoInterface( const QString& confFile)
    : state( Initialize )
    , configIface(0)
    , ifaceStatus(Unknown)
#ifndef QTOPIA_NO_FSO
    , gsmDev("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , gsmPdp("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
    , fsoEnabled(false)
#endif    
    , netSpace( 0 )
{
    qLog(Network) << "Creating HsoInterface instance";
    configIface = new DialupConfig( confFile );

#ifndef QTOPIA_NO_FSO
    fsoEnabled = !strcmp("Fso", getenv( "QTOPIA_PHONE" ));      // are we using FSO as backend?
#endif    
}

HsoInterface::~HsoInterface()
{
    qLog(Network) << "Deleting HsoInterface instance";
    if (configIface)
        delete configIface;
    configIface = 0;
}

QtopiaNetworkInterface::Status HsoInterface::status()
{
    if ( ifaceStatus == QtopiaNetworkInterface::Unknown) {
        return ifaceStatus;
    }

    netSpace->setAttribute( "State", ifaceStatus );
    return ifaceStatus;
}

void HsoInterface::initialize()
{
    if ( !netSpace ) {
        const uint ident = qHash( configIface->configFile() );
        QString path = QString("/Network/Interfaces/%1").arg(ident);
        netSpace = new QValueSpaceObject( path, this );
        netSpace->setAttribute( "Config", configIface->configFile() );
        netSpace->setAttribute( "State", QtopiaNetworkInterface::Unknown );
        netSpace->setAttribute( "Error", QtopiaNetworkInterface::NotInitialized );
        netSpace->setAttribute( "ErrorString", tr("Interface hasn't been initialized yet.") );
        netSpace->setAttribute( "NetDevice", QVariant() );
        netSpace->setAttribute( "UpdateTrigger", 0 );
    }

    ifaceStatus = QtopiaNetworkInterface::Down;
    netSpace->setAttribute( "State", ifaceStatus );
}

void HsoInterface::cleanup()
{
    if ( ifaceStatus != QtopiaNetworkInterface::Unknown ) {
        ifaceStatus = QtopiaNetworkInterface::Unknown;
        netSpace->setAttribute( "State", ifaceStatus );
    } else {
        return;
    }
}

bool HsoInterface::setDefaultGateway()
{
    QStringList params;
    params << "install";
    params << "dns";
    if ( configIface->property("Serial/UsePeerDNS").toString() == "n" ) {
        params << configIface->property("Properties/DNS_1").toString();
        params << configIface->property("Properties/DNS_2").toString();
    }
 
    return true;
}

bool HsoInterface::start( const QVariant /*options*/ )
{
    qDebug() << "================ HsoInterface::start()";
    
    ifaceStatus = QtopiaNetworkInterface::Pending;
    netSpace->setAttribute("State", ifaceStatus);

    bool ok = true;

    ifaceStatus = ok ?
        QtopiaNetworkInterface::Up : QtopiaNetworkInterface::Down;
    
    netSpace->setAttribute("State", ifaceStatus);
    
    return true;
}

bool HsoInterface::stop()
{
    qDebug() << "================ HsoInterface::stop()";
    ifaceStatus = QtopiaNetworkInterface::Down;
    netSpace->setAttribute("State", ifaceStatus);
    return true;
}

QString HsoInterface::device() const
{
    return "hso0";
}

QtopiaNetwork::Type HsoInterface::type() const
{
    return QtopiaNetwork::toType( configIface->configFile() );
}

bool HsoInterface::isAvailable()
{
    return true;
}

bool HsoInterface::isActive()
{
    return true;
}

QtopiaNetworkConfiguration * HsoInterface::configuration()
{
    return configIface;
}

void HsoInterface::setProperties( const QtopiaNetworkProperties& properties )
{
    configIface->writeProperties(properties);
}
