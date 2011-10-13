/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2011 qtmoko project
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

#include "rotation.h"

#include <qtopiaapplication.h>
#include <QApplication>
#include <QtopiaServiceRequest>

#include <QDir>
#include <QtopiaIpcEnvelope>
#include <stdlib.h>
#include <QDesktopWidget>
#include <QWSDisplay>
#include <QValueSpaceItem>
#include <QKeyEvent>
#include <QTimer>

RotationSettings::RotationSettings( QWidget* parent,  Qt::WFlags fl )
    : QDialog( parent, fl ),
      changeRotTo(-1)
{
//    setupUi(this);
}

RotationSettings::~RotationSettings()
{
    releaseKeyboard();
}

void RotationSettings::setnewRotation(int r)
{
    QtopiaServiceRequest svreq("RotationManager", "setCurrentRotation(int)");
    svreq << r;
    svreq.send();
}

