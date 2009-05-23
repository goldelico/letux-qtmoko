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

#include "htckbddriverplugin.h"
#include "htckbdhandler.h"

#include <qtopiaglobal.h>
#include <qtopialog.h>

htcKbdDriverPlugin::htcKbdDriverPlugin(QObject *parent)
  : QKbdDriverPlugin(parent)
{}

htcKbdDriverPlugin::~htcKbdDriverPlugin()
{}

QWSKeyboardHandler* htcKbdDriverPlugin::create(const QString &driver, 
                                               const QString &device)
{
  if(driver.toLower() == "htckbdhandler")
    return new htcKbdHandler(device);
  return 0;

}

QWSKeyboardHandler* htcKbdDriverPlugin::create(const QString& name)
{
  return create(name, QString());
}

QStringList htcKbdDriverPlugin::keys() const
{
  return QStringList() << "htckbdhandler";
}

QTOPIA_EXPORT_QT_PLUGIN(htcKbdDriverPlugin)
