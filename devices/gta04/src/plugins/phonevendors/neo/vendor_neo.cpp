/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak <psonek2@seznam.cz>
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

#include "vendor_neo_p.h"
#include <qmodemindicators.h>
#include <qatutils.h>
#include <qatresultparser.h>
#include <QProcess>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QSettings>

#include <alsa/asoundlib.h>

#include <qmodemcallvolume.h>
#include <qmodemsiminfo.h>
#include <qmodemcellbroadcast.h>

NeoModemService::NeoModemService
    (const QString & service, QSerialIODeviceMultiplexer * mux,
     QObject * parent)
:QModemService(service, mux, parent)
{
    qDebug() << "Gta04ModemService::constructor";

    // Turn on dynamic signal quality notifications.
    // Register for "_OSIGQ" notifications to get signal quality updates.
    primaryAtChat()->registerNotificationType
        ("_OSIGQ:", this, SLOT(sigq(QString)));

    chat("AT_OSQI=1");          // unsolicited reporting of antenna signal strength, e.g. "_OSIGQ: 3,0"
    chat("AT_OPCMENABLE=1");    // enable the PCM interface for voice calls
}

NeoModemService::~NeoModemService()
{
}

void NeoModemService::initialize()
{
    // Disable cell broadcast, GTA04 modem probably does not know AT+CSCB commands
    suppressInterface < QCellBroadcast > ();

    QModemService::initialize();

    // Disable signal and battery polling, just do initial signal check
    indicators()->setPolling(false, true, false);
}

void NeoModemService::sigq(const QString & msg)
{
    // Automatic signal quality update, in the range 0-31, e.g. _OSIGQ: 7,0
    uint posn = 8;
    uint rssi = QAtUtils::parseNumber(msg, posn);
    indicators()->setSignalQuality((int)rssi, 31);
}

void NeoModemService::reset()
{
    qLog(Modem) << "Gta04ModemService::reset()";
}

void NeoModemService::suspend()
{
    qLog(Modem) << " Gta04ModemService::suspend()";
    chat("AT_OSQI=0");          // unsolicited reporting of antenna signal strength, e.g. "_OSIGQ: 3,0"
    suspendDone();
}

void NeoModemService::wake()
{
    qLog(Modem) << " Gta04ModemService::wake()";
    chat("AT_OSQI=1");          // unsolicited reporting of antenna signal strength, e.g. "_OSIGQ: 3,0"
    wakeDone();
}

bool NeoModemService::supportsAtCced()
{
    return false;
}

// Each char of output operator name is 4 chars in input name. The 4 chars is
// integer string of unicode value. E.g.
// 0056006f006400610066006f006e006500200043005a -> Vodafone CZ
QString NeoModemService::decodeOperatorName(QString name)
{
    QString str;
    str.resize(name.size() / 4);
    for (int i = 0; i < str.size(); i++) {
        QString numStr = name.mid(i * 4, 4);
        bool ok;
        int num = numStr.toInt(&ok, 16);
        str[i] = QChar(num);
    }
    return str;
}
