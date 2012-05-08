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

#include "htcuniversalvibrateaccessory.h"

#include <qfile.h>
#include <qtextstream.h>

HtcuniversalVibrateAccessory::HtcuniversalVibrateAccessory
(QModemService *service)
  : QVibrateAccessoryProvider(service->service(), service)
{
  setSupportsVibrateOnRing(true);
  setSupportsVibrateNow(false);
}

HtcuniversalVibrateAccessory::~HtcuniversalVibrateAccessory() { }

void HtcuniversalVibrateAccessory::setVibrateOnRing(const bool value)
{
  setVibrateNow(value);
}

void HtcuniversalVibrateAccessory::setVibrateNow(const bool value, quint16 strength, int timeoutMs)
{
  QFile trigger("/sys/class/leds/htcuniversal:vibra/trigger");
  trigger.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
  QTextStream out(&trigger);

  out << (value ? "timer" : "none");

  trigger.close();
  QVibrateAccessoryProvider::setVibrateNow(value);
}
