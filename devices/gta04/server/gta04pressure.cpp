/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2012 Radek Polak.
** Copyright (C) 2012 Neil Jerram.
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

#include "gta04pressure.h"

#include <QSocketNotifier>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
#include <QProcess>
#include <QValueSpaceObject>
#include <QValueSpaceItem>
#include <QLocale>

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
#include <glob.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <QTcpSocket>
#include <QtDebug>

QTOPIA_TASK(Gta04Pressure, Gta04Pressure);

static char pressurefile[256];

int get_pressure(char *buf, int buflen) {
  if (pressurefile[0] == 0) {
    glob_t glob_data;
    // int i;
    memset(&glob_data, 0, sizeof(glob_data));
    if (!glob("/sys/bus/iio/devices/iio:device*/in_pressure*_input", 0,
         NULL, &glob_data)) {
      if (glob_data.gl_pathv[0] != NULL) {
        if (strlen(glob_data.gl_pathv[0]) < sizeof(pressurefile)) {
          strcpy(pressurefile, glob_data.gl_pathv[0]);
        }
      }
      globfree(&glob_data);
    }
  }
  if (pressurefile[0] != 0) {
    FILE *f = fopen(pressurefile, "r");
    if (f == NULL) {
      pressurefile[0] = 0;
      return 0;
    }
    if (fgets(buf, buflen, f)) {
      buf[strcspn(buf,"\r\n")] = 0;
      fclose(f);
      return 1;
    }
    fclose(f);
    return 0;
  } 
  return 0;
}

Gta04Pressure::Gta04Pressure()
:
timer(this)
{
    qLog(Hardware) << "gta04 pressure";

    pressure_space = new QValueSpaceObject("/UI/Pressure", this);

    // Arrange to update pressure every 10 minutes.
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    timer.start(10 * 60 * 1000);

    // Also take an initial pressure measurement right now. 
    QTimer::singleShot(1, this, SLOT(updateStatus()));
}

Gta04Pressure::~Gta04Pressure()
{
    delete pressure_space;
}


void Gta04Pressure::updateStatus()
{
    char buf[64];
    if (!get_pressure(buf,sizeof(buf)))
        return;
    QLocale loc(QLocale::C);
    QString pressure_str(buf);
    double pressure = loc.toDouble(pressure_str, 0);
    QString pa = QString::number((int) (pressure * 1000.0));
    QString mb = QString::number(pressure * 10.0,'f', 1);

    pressure_space->setAttribute("Pa", pa);
    pressure_space->setAttribute("mb", mb);

    // Also export the BMP085-reported temperature.  Note that this is
    // a GTA04-internal temperature, not the ambient environmental
    // temperature; hence the name "CelsiusInternal".  Therefore it's
    // not of primary interest to most users, but would be interesting
    // to those who want to monitor the inside of their GTA04.
    // There's nothing in the UI that shows CelsiusInternal by
    // default, but it's easy for interested people to modify their
    // theme to include it.
    //tempStr = tempStr.trimmed();
    //pressure_space->setAttribute("CelsiusInternal",
    //				 QString::number((tempStr.toInt() + 5) / 10));
}
