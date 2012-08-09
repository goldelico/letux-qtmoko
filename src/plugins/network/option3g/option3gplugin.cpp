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

#include "option3gplugin.h"

#include <QPointer>
#include <qtopialog.h>
#include <qtopiaapplication.h>

Option3gPlugin::Option3gPlugin()
{
    //Load translation for libqtopianetwork
    QtopiaApplication::loadTranslations( "libqtopianetwork" );
}

Option3gPlugin::~Option3gPlugin()
{
    qLog(Network) << "Deleting Option3gPlugin (" << instances.count()
        << " instances)";
    //cleanup all interface instances
    while ( !instances.isEmpty() )
    {
        QPointer<QtopiaNetworkInterface> impl = instances.takeFirst();
        if (impl)
            delete impl;
    }
}

QPointer<QtopiaNetworkInterface> Option3gPlugin::network( const QString& confFile)
{
    qLog(Network) << "new Dialup interface instance requested -> " << confFile;
    QPointer<QtopiaNetworkInterface> impl = new DialupImpl( confFile );
    instances.append(impl);

    return impl;
}

QtopiaNetwork::Type Option3gPlugin::type() const
{
    //can handle all type of GPRS and analog dialup connection
    //pcmcia, internal device and specifically named devices
    //see dialup.cpp (DialupImpl::isAvailable())
    return ( QtopiaNetwork::Dialup |
             QtopiaNetwork::GPRS |
#ifdef QTOPIA_CELL
             QtopiaNetwork::PhoneModem |
#endif
             QtopiaNetwork::PCMCIA |
             QtopiaNetwork::NamedModem );
}

QByteArray Option3gPlugin::customID() const
{
    return QByteArray();
}
QTOPIA_EXPORT_PLUGIN( Option3gPlugin );
