/****************************************************************************
**
** Copyright (C) 2000-2006 TROLLTECH ASA. All rights reserved.
**
** This file is part of the Phone Edition of the Qtopia Toolkit.
**
** This software is licensed under the terms of the GNU General Public
** License (GPL) version 2.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "openezxkbddriverplugin.h"
#include "openezxkbdhandler.h"

#include <qtopiaglobal.h>
#include <qtopialog.h>

openezxKbdDriverPlugin::openezxKbdDriverPlugin( QObject *parent )
    : QKbdDriverPlugin( parent )
{}

openezxKbdDriverPlugin::~openezxKbdDriverPlugin()
{}

QWSKeyboardHandler* openezxKbdDriverPlugin::create(const QString &driver, 
                                               const QString &device)
{
    qWarning("openezxKbdDriverPlugin:create()");
    if( driver.toLower() == "openezxkbdhandler" ) {
        return new openezxKbdHandler(device);
    }
    return 0;

}

QWSKeyboardHandler* openezxKbdDriverPlugin::create( const QString& name)
{
    return create( name, QString());
}

QStringList openezxKbdDriverPlugin::keys() const
{
    return QStringList() << "openezxkbdhandler";
}

QTOPIA_EXPORT_QT_PLUGIN(openezxKbdDriverPlugin)
