/*
 *  Jeremy Compostella <jeremy.compostella@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include "htcuniversalgsmpower.h"
#include <qmodemindicators.h>
#include <QDebug>

HtcuniversalGsmPower *HtcuniversalGsmPower::instance = 0;

HtcuniversalGsmPower* HtcuniversalGsmPower::getInstance(QModemService *service)
{
  if (instance)
    return instance;
  else if (service)
    return instance = new HtcuniversalGsmPower(service);
  qWarning("getInstance() with service == 0 inconsistent");
  return 0;
}

void HtcuniversalGsmPower::init()
{
  if (!on)
    return;
  chat("AT+CMEE=1;+CRC=1;+CR=1;+CREG=1;+FCLASS=0;+CMGF=1;+CSCS=\"HEX\"");
  chat("AT+CSSN=0,1;+CLIP=1;+COLP=0;+CCWA=1;+CMUT=0;+CLIR=0;+CNMI=1,2,2,2,0");
  chat("AT+CGREG=1;+CUSD=1");
  chat("AT+CGEQREQ=1,4,0,0,0,0,2,0,\"0E0\",\"0E0\",3,0,0");
  chat("AT+CGAATT=2,1,1");
  chat("AT+BANDSEL=2,0");
}

void HtcuniversalGsmPower::turnOff()
{
  QTextStream(powerFile) << "off" << endl;
  on = false;
  service->indicators()->setSignalQuality(0, 4);
}

void HtcuniversalGsmPower::turnOn()
{
  if (on)
    return;
  QTextStream(powerFile) << "on" << endl;
  on = true;
  emit gsmReset();
}

void HtcuniversalGsmPower::enableNotifications()
{
  if (!on)
    return;
  chat("AT@HTCCSQ=1");
  chat("AT@HTCDIS=1");
  chat("AT@HTCSAP=1");
  chat("AT@USBPOWER=1");
}

void HtcuniversalGsmPower::disableNotifications()
{
  if (!on)
    return;
  chat("AT@HTCCSQ=0");
  chat("AT@HTCDIS=0");
  chat("AT@HTCSAP=0");
  chat("AT@USBPOWER=0");
}

HtcuniversalGsmPower::HtcuniversalGsmPower(QModemService *service)
  : service(service), on(true)
{
#define powerFilename "/sys/class/power_driver/htcuniversal_phone/power_state"
  powerFile = new QFile(powerFilename);
  if(!powerFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    qWarning() << powerFilename << " not opened";
#undef powerFilename
}

void HtcuniversalGsmPower::chat(const char *str)
{
  service->primaryAtChat()->chat(str);
}
