/****************************************************************************
 **
 ** Copyright (C) 2000-2007 TROLLTECH ASA. All rights reserved.
 **
 ** This file is part of the Opensource Edition of the Qtopia Toolkit.
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
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QObject>

#include <unistd.h>

QTOPIABASE_EXPORT int qpe_sysBrightnessSteps()
{
  QFile maxBrightness("/sys/class/backlight/corgi-bl/max_brightness");
  QString strvalue;

  if( !maxBrightness.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "/sys/class/backlight/corgi-bl/max_brightness not opened";
  } else {
    QTextStream in(&maxBrightness);
    in >> strvalue;
    maxBrightness.close();
  }

  return strvalue.toInt();
}

/*static void fb_power(bool on)
{
  QFile power("/sys/class/lcd/pxa2xx-fb/power");
  if (power.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream out(&power);
    out << QString::number(on ? 0 : 4);
    power.close();
  }
}*/

QTOPIABASE_EXPORT void qpe_setBrightness(int b)
{
  static int previous = 0;
  static int max = qpe_sysBrightnessSteps(); /* get it once */

  if (b < 0)
    b = max;

  /*if (b > 1 && previous == 0) {
    fb_power(true); */
//     sleep(1); /* ugly: let it a little of time to avoid an lcd flash */
  //}

  QFile brightness("/sys/class/backlight/corgi-bl/brightness");
  if(!brightness.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    qWarning() << "/sys/class/backlight/corgi-bl/brightness not opened";
  } else {
    QTextStream out(&brightness);
    out << QString::number(b);
    brightness.close();
  }

  /*if (b == 0)
    fb_power(false);

  previous = b;*/
}
