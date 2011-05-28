/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak.
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
#include "fsoplugin.h"
#include "fsomodemservice.h"

QTOPIA_EXPORT_PLUGIN( FsoPluginImpl )

FsoPluginImpl::FsoPluginImpl()
{
}

FsoPluginImpl::~FsoPluginImpl()
{
}

bool FsoPluginImpl::supports( const QString& manufacturer )
{
    return manufacturer.contains( "FSO" );
}

QModemService *FsoPluginImpl::create
    ( const QString& service, QSerialIODeviceMultiplexer *mux, QObject *parent )
{
    return new FsoModemService( service, mux, parent );
}
