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
    
    qDebug() << "======= HsoInterface ctor";
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
    qDebug() << "====== HsoInterface::status() ifaceStatus=" << ifaceStatus;
    
    if ( ifaceStatus == QtopiaNetworkInterface::Unknown) {
        return ifaceStatus;
    }

    QtopiaNetworkInterface::Status status = QtopiaNetworkInterface::Unavailable;
    if ( isAvailable() ) {
        status = QtopiaNetworkInterface::Down;

        if ( ifaceStatus == QtopiaNetworkInterface::Pending ||
                ifaceStatus == QtopiaNetworkInterface::Demand )
            // these states are updated by timerEvent
            status = ifaceStatus;
        else if ( isActive() )
            status = QtopiaNetworkInterface::Up;
    }
    
    qDebug() << "====== HsoInterface::status()=" <<  status;

    ifaceStatus = status;
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

    qDebug() << "HsoInterface::initialize()";
    if ( isAvailable() ) {
        qDebug() << "1";
        if ( isActive() )
            ifaceStatus = QtopiaNetworkInterface::Up;
        else
            ifaceStatus = QtopiaNetworkInterface::Down;
    } else {
        ifaceStatus = QtopiaNetworkInterface::Unavailable;
    }
    
    qDebug() << "ifaceStatus=" << ifaceStatus;

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
    qDebug() << "=== HsoInterface::isActive()";
    
    //TODO support for IPv4 only (PF_INET6)
    int inetfd = socket( PF_INET, SOCK_DGRAM, 0 );
    if ( inetfd == -1 )
        return false;

    int flags = 0;
    struct ifreq ifreqst;
    strcpy( ifreqst.ifr_name, "hso0" );
    int ret = ioctl( inetfd, SIOCGIFFLAGS, &ifreqst );
    if ( ret == -1 ) {
        int error = errno;
        qLog(Network) << "HsoInterface: " << strerror( error );
        ::close( inetfd );
        return false;
    }


    flags = ifreqst.ifr_flags;
    qDebug() << "================== flags=" << flags;
    
    if ( ( flags & IFF_UP ) == IFF_UP) {
        qLog(Network) << "HsoInterface: hso0 is up and running";
        ::close( inetfd );
        return true;
    }

    qLog(Network) << "HsoInterface: device is offline" ;
    ::close( inetfd );
    return false;
}

QtopiaNetworkConfiguration * HsoInterface::configuration()
{
    return configIface;
}

void HsoInterface::setProperties( const QtopiaNetworkProperties& properties )
{
    configIface->writeProperties(properties);
}
