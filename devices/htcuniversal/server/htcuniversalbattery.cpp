/****************************************************************************
 **
 ** Copyright (C) 2007-2007 TROLLTECH ASA. All rights reserved.
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

#include "htcuniversalbattery.h"
#include "qtopiaserverapplication.h"

#include <QPowerSourceProvider>
#include <QTimer>
#include <QFileMonitor>

#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct HtcuniversalDetect {
  unsigned int   dummy1;
  unsigned int   dummy2;
  unsigned short type;
  unsigned short code;
  unsigned int   value;
};

HtcuniversalBattery::HtcuniversalBattery(QObject *parent)
  : QObject(parent), ac(0), battery(0)
{
  QString inStr;

  ac = new QPowerSourceProvider(QPowerSource::Wall, "PrimaryAC", this);
  battery = new QPowerSourceProvider(QPowerSource::Battery, "DS2760Battery",
				     this);

  /* let's initialize full and empty value */
  fullCharge = 0;
  emptyCharge = 0;
  inStr = readValue(BATTERY_CLASS "/charge_full_design");
  if ( inStr != NULL ) {
    fullCharge = inStr.toInt();
  } else {
    qWarning()<<"HtcuniversalBattery: failed opening charge_full_design file";
  }
  inStr = readValue(BATTERY_CLASS "/charge_empty");
  if ( inStr != NULL) {
    emptyCharge = inStr.toInt();
  } else {
    qWarning()<<"HtcuniversalBattery: failed opening charge_empty file";
  }

  /* tsdogs: the iNotify or whatever method 
   * won't work with sys entries ...        */
  /*QFileMonitor *tmpMon = new QFileMonitor(BATTERY_CLASS "/status", 
                                 QFileMonitor::Poll, this);
  connect(tmpMon, SIGNAL( fileChanged( const QString & )),
            this, SLOT(updateUniversalStatus()));
  tmpMon = new QFileMonitor(BATTERY_CLASS "/charge_now", 
                                 QFileMonitor::Poll, this);
  connect(tmpMon, SIGNAL( fileChanged( const QString & )),
            this, SLOT(updateUniversalStatus()));*/

  startTimer(10 * 1000);
  QTimer::singleShot( 1000, this, SLOT(updateUniversalStatus()));
}

/* get the battery status */
void HtcuniversalBattery::updateUniversalStatus()
{
  QString value;

  value = readValue(AC_CLASS "/online");
  if ( value != NULL) {
    ac->setAvailability( (value.toInt() == 1) ?
           QPowerSource::Available :
          QPowerSource::NotAvailable);
  } else {
    ac->setAvailability(QPowerSource::Failed);
    qWarning()<<"HtcuniversalBattery:" <<
        "failed opening AC online file...";
  }

  /* should we try again reading full charge ??? */
  if (fullCharge <= 0) {
    battery->setAvailability(QPowerSource::NotAvailable);
    battery->setCharging(false);
    battery->setCharge(-1);
    return;
  }

  value = readValue(BATTERY_CLASS "/status");
  if ( value != NULL ) {
    battery->setAvailability(QPowerSource::Available);
    if (value.trimmed().toUpper() == "CHARGING") {
      battery->setCharging(true);
    } else {
      battery->setCharging(false);
    }
 
    value = readValue(BATTERY_CLASS "/charge_now");
    if ( value != NULL ) {
      /* let's calculate the percentage of battery charge we have */
      percentCharge = (int)round (value.toDouble() * 100 /
                              (fullCharge - emptyCharge));
      battery->setCharge(percentCharge > 100 ? 100 : percentCharge);
      qLog(PowerManagement)<<"setCharge "<<percentCharge;
    } else {
      battery->setCharge(-1);
      qWarning()<<"HtcuniversalBattery:" <<
        "failed opening battery charge_now file...";
    }
  } else {
    battery->setAvailability(QPowerSource::Failed);
    qWarning()<<"HtcuniversalBattery:" <<
        "failed opening battery status file...";
  }
}

/*! \internal */
QString HtcuniversalBattery::readValue( QString file ) {
  QString inStr;
  QFile f(file);

  if ( f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
    QTextStream in(&f);
    in >> inStr;
    f.close();
    return inStr;
  }
  return NULL;
}

/*! \internal */
void HtcuniversalBattery::timerEvent(QTimerEvent *)
{
  updateUniversalStatus();
}

QTOPIA_TASK(HtcuniversalBattery, HtcuniversalBattery);
