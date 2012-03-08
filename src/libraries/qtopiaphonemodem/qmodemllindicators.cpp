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

#include <QFile>
#include <QDebug>
#include <qtopialog.h>
#include <qmodemllindicators.h>

// Turn on/off led. Device can be "gta02" or "gta04".
// GTA04: color can be "green" or "red" type can be "power" or "aux"
// GTA02: supports "blue power" "orange power" and "red aux"
static void setLed(const char *device, const char *color,
                   const char *type, bool value)
{
    char filename[255];
    sprintf(filename, "/sys/class/leds/%s:%s:%s/brightness", device, color, type);  // e.g. /sys/class/leds/gta04:green:power
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "setLed failed" << f.errorString();
        return;
    }
    f.write(value ? "255" : "0");
    f.close();
}

#ifdef QT_QWS_GTA04

static void missedCallLedOn()
{
    setLed("gta04", "green", "power", true);
}

static void missedCallLedOff()
{
    setLed("gta04", "green", "power", false);
}

#elif QT_QWS_NEO

static void missedCallLedOn()
{
    setLed("gta02", "blue", "power", true);
}

static void missedCallLedOff()
{
    setLed("gta02", "blue", "power", false);
}

#else

static void missedCallLedOn()
{
}

static void missedCallLedOff()
{
}

#endif

// Called when ringing() function is called
void QModemLLIndicators::Ringing()
{
    missedCallLedOn();          // consider the called missed now
}

// Called on QModemCall.hangup()
void QModemLLIndicators::MissedCallsChanged(int num)
{
    static int numMissed = 0;
    qLog(Modem) << "MissedCallsChanged " << numMissed << "->" << num;
    if (numMissed > 0 && num == 0) {
        missedCallLedOff();     // clear missed call led
    }
    numMissed = num;
}

// Called on QModemCall.accept()
void QModemLLIndicators::Hangup()
{
    missedCallLedOff();         // clear missed call led - user should have noticed it when he hangs up
}

// Called user checks missed calls
void QModemLLIndicators::Accept()
{
    missedCallLedOff();         // clear missed call led when users accepts the call
}
