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

#include <custom.h>
#include <qtopianamespace.h>
#include <QtopiaServiceRequest>
#include <QtopiaIpcEnvelope>

#include <qwindowsystem_qws.h>
#include <QValueSpaceItem>
#include <QValueSpaceObject>
#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QFile>
#include <QFileInfo>

#include <QTextStream>
#include <QDebug>

QTOPIABASE_EXPORT int qpe_sysBrightnessSteps()
{
    return 2;
}

QTOPIABASE_EXPORT void qpe_setBrightness(int b)
{
    QFile f("/sys/devices/omapdss/display2/enabled");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "qpe_setBrightness: " + f.errorString();
        return;
    }
    QTextStream out(&f);
    out << (b == 1 ? "0" : "1"); // 1 == first step == dim, everything else is bright
    f.close();
}
