/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2012 Radek Polak.
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

#include "neohardware.h"

#include <QSocketNotifier>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
#include <QProcess>
#include <QtopiaIpcAdaptor>

#include <qcontentset.h>
#include <qtopiaapplication.h>
#include <qtopialog.h>
#include <qtopiaipcadaptor.h>

#include <qbootsourceaccessory.h>
#include <qtopiaipcenvelope.h>

#include <qtopiaserverapplication.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/input.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <QTcpSocket>
#include <QtDebug>

QTOPIA_TASK(NeoHardware, NeoHardware);

NeoHardware::NeoHardware()
:  
vsoPortableHandsfree("/Hardware/Accessories/PortableHandsfree"),
vsoUsbCable("/Hardware/UsbGadget"), vsoNeoHardware("/Hardware/Neo")
{
    adaptor = new QtopiaIpcAdaptor("QPE/NeoHardware");
    qLog(Hardware) << "gta04 hardware";

    cableConnected(getCableStatus());

    vsoPortableHandsfree.setAttribute("Present", false);
    vsoPortableHandsfree.sync();

    // Handle Audio State Changes
    audioMgr = new QtopiaIpcAdaptor("QPE/AudioStateManager", this);

    QtopiaIpcAdaptor::connect(adaptor, MESSAGE(headphonesInserted(bool)),
                              this, SLOT(headphonesInserted(bool)));

    QtopiaIpcAdaptor::connect(adaptor, MESSAGE(cableConnected(bool)),
                              this, SLOT(cableConnected(bool)));
}

NeoHardware::~NeoHardware()
{
}

void NeoHardware::headphonesInserted(bool b)
{
    qLog(Hardware) << __PRETTY_FUNCTION__ << b;
    vsoPortableHandsfree.setAttribute("Present", b);
    vsoPortableHandsfree.sync();
    if (b) {
        QByteArray mode("Headphone");
        audioMgr->send("setProfile(QByteArray)", mode);
    } else {
        QByteArray mode("MediaSpeaker");
        audioMgr->send("setProfile(QByteArray)", mode);
    }
}

void NeoHardware::cableConnected(bool b)
{
    qLog(Hardware) << __PRETTY_FUNCTION__ << b;
    vsoUsbCable.setAttribute("cableConnected", b);
    vsoUsbCable.sync();
}

void NeoHardware::shutdownRequested()
{
    qLog(PowerManagement) << __PRETTY_FUNCTION__;
    QtopiaServerApplication::instance()->
        shutdown(QtopiaServerApplication::ShutdownSystem);
}

bool NeoHardware::getCableStatus()
{
    qLog(PowerManagement) << __PRETTY_FUNCTION__;
    QFile f("/sys/class/power_supply/twl4030_usb/status");
    if(!f.open(QIODevice::ReadOnly)) {
        qLog(PowerManagement) << "getCableStatus failed " << f.errorString();
        return true;
    }
    QByteArray content = f.readAll();
    f.close();
    return (!content.contains("Discharging"));
}