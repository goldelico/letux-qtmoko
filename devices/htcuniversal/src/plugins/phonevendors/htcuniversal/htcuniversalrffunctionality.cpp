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

#include "htcuniversalrffunctionality.h"
#include "htcuniversalgsmpower.h"
#include <qatresultparser.h>
#include <unistd.h>
#include <QTimer>

class HtcuniversalCFunUserData : public QAtResult::UserData
{
public:
  HtcuniversalCFunUserData(QPhoneRfFunctionality::Level level)
    : level(level) {}

  QPhoneRfFunctionality::Level level;
};

HtcuniversalRfFunctionality::HtcuniversalRfFunctionality(QModemService *service)
  : QModemRfFunctionality(service), service(service), planeMode(false) { }

HtcuniversalRfFunctionality::~HtcuniversalRfFunctionality() { }

void HtcuniversalRfFunctionality::forceLevelRequest()
{
  service->primaryAtChat()->chat
    ("AT+CFUN?", this, SLOT(cfun(bool,QAtResult)));
}

void HtcuniversalRfFunctionality::setLevel(QPhoneRfFunctionality::Level level)
{
  // Change DisableTransmitAndReceive into Minimum, because htcuniversal
  // doesn't support DisableTransmitAndReceive.
  HtcuniversalGsmPower *gsm = HtcuniversalGsmPower::getInstance(service);
  QPhoneRfFunctionality::Level newLevel;
  if (level == DisableTransmitAndReceive) {
    gsm->disableNotifications();
    newLevel = Minimum;
  }  else
    newLevel = level;
  if (newLevel == Full && planeMode) {
    gsm->turnOn();
    sleep(2);
    service->primaryAtChat()->chat("ATE0V1");
    sleep(2);
    gsm->init();
    gsm->enableNotifications();
  }
  service->primaryAtChat()->chat
    ("AT+CFUN=" + QString::number((int)newLevel),
     this, SLOT(cfunSet(bool,QAtResult)),
     new HtcuniversalCFunUserData(level));
}

void HtcuniversalRfFunctionality::cfun(bool, const QAtResult& result)
{
  QAtResultParser parser(result);
  if (parser.next("+CFUN:")) {
    Level level = (Level)parser.readNumeric();
    if (planeMode && level == Minimum) {
      // If we are actually in plane mode, then we aren't "really"
      // in Minimum, but DisableTransmitAndReceive.
      level = DisableTransmitAndReceive;
    }
    setValue("level", qVariantFromValue(level));
    emit levelChanged();
  }
}

void HtcuniversalRfFunctionality::cfunSet(bool ok, const QAtResult& result)
{
  Level level = ((HtcuniversalCFunUserData *)result.userData())->level;
  planeMode = (level == DisableTransmitAndReceive);

  // Report the results to the client applications.
  if (ok) {
    setValue("level", qVariantFromValue(level));
    emit levelChanged();
  }
  emit setLevelResult((QTelephony::Result)result.resultCode());

  if (ok && planeMode)
    QTimer::singleShot(20000, HtcuniversalGsmPower::getInstance(service),
		       SLOT(turnOff()));
}
