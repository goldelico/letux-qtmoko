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

#include "htcbutdriverplugin.h"
#include "htcbuthandler.h"

#include <qtopiaglobal.h>
#include <qtopialog.h>

htcButDriverPlugin::htcButDriverPlugin( QObject *parent )
    : QKbdDriverPlugin( parent )
{}

htcButDriverPlugin::~htcButDriverPlugin()
{}

QWSKeyboardHandler* htcButDriverPlugin::create(const QString& driver, const QString& device)
{
    if( driver.toLower() == "htcbuthandler" ) {
        return new htcButHandler(device);
    }
    return 0;
}

QWSKeyboardHandler* htcButDriverPlugin::create( const QString& driver)
{
    return create(driver, QString());
}

QStringList htcButDriverPlugin::keys() const
{
    return QStringList() << "htcbuthandler";
}

QTOPIA_EXPORT_QT_PLUGIN(htcButDriverPlugin)
