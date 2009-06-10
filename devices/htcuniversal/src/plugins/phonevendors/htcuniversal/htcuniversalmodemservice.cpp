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

#include "htcuniversalmodemservice.h"

#include <qatutils.h>
#include <qmodemindicators.h>
#include <qmodemsimfiles.h>
#include <qmodemcallsettings.h>
#include <qmodemservicenumbers.h>

#include "htcuniversalbandselection.h"
#include "htcuniversalcallvolume.h"
#include "htcuniversalpinmanager.h"
#include "htcuniversalplugin.h"
#include "htcuniversalpreferrednetworkoperators.h"
#include "htcuniversalsimtoolkit.h"
#include "htcuniversalvibrateaccessory.h"
#include "htcuniversalcallprovider.h"
#include "htcuniversalmodemnetworkregistration.h"
#include "htcuniversalrffunctionality.h"
#include "htcuniversalgsmpower.h"

HtcuniversalModemService::HtcuniversalModemService
(const QString& service, QSerialIODeviceMultiplexer *mux,
  QObject *parent)
  : QModemService(service, mux, parent), manager(0), battery(0)
{
  connect(this, SIGNAL(resetModem()), this, SLOT(reset()));

  primaryAtChat()->registerNotificationType
    ("@HTCCSQ:", this, SLOT(signalQualityNotification(QString)));

  manager = new QHardwareManager("QPowerSource");
  connect(manager, SIGNAL(providerAdded(QString)),
	  this, SLOT(newBatteryAdded(QString)));

  indicators()->setBatteryCharge(100, 100, QModemIndicators::PoweredByBattery);
}

HtcuniversalModemService::~HtcuniversalModemService()
{
  delete manager;
}

void HtcuniversalModemService::initialize()
{
  HtcuniversalGsmPower::getInstance(this)->init();
  HtcuniversalGsmPower::getInstance(this)->enableNotifications();
  chat("AT+CSCA?");

  if (!supports<QPinManager>())
    addInterface(new HtcuniversalPinManager(this));

  if (!supports<QBandSelection>())
    addInterface(new HtcuniversalBandSelection(this));

  if (!callProvider())
    setCallProvider(new HtcuniversalCallProvider(this));

  if (!supports<QVibrateAccessory>())
    addInterface(new HtcuniversalVibrateAccessory(this));

  if (!supports<QCallVolume>())
    addInterface(new HtcuniversalCallVolume(this));

  if (!supports<QModemPreferredNetworkOperators>())
    addInterface(new HtcuniversalPreferredNetworkOperators(this));

  if (!supports<QModemNetworkRegistration>())
    addInterface(new HtcuniversalModemNetworkRegistration(this));

  if (!supports<QModemSimFiles>())
    addInterface(new QModemSimFiles(this));

  if (!supports<QModemCallSettings>())
    addInterface(new QModemCallSettings(this));

  if (!supports<QModemServiceNumbers>())
    addInterface(new QModemServiceNumbers(this));

  if (!supports<QPhoneRfFunctionality>())
    addInterface(new HtcuniversalRfFunctionality(this));

  QModemService::initialize();
}

void HtcuniversalModemService::signalQualityNotification(const QString &msg)
{
  // parse the proprietary unsollicited signal quality notification (@HTCCSQ).
  uint posn = 8;
  uint rssi = QAtUtils::parseNumber(msg, posn);
  indicators()->setSignalQuality((int)rssi, 4);
}

void HtcuniversalModemService::configureDone(bool ok)
{
  supportsStk = ok;
}

void HtcuniversalModemService::reset()
{
  qWarning("reset CALLED !!!");
//   chat("AT+COPS=0"); // force auto operations
}

void HtcuniversalModemService::suspend()
{
  HtcuniversalGsmPower::getInstance(this)->disableNotifications();
  QTimer::singleShot(500, this, SLOT(suspendDone()));
}

void HtcuniversalModemService::wake()
{
  HtcuniversalGsmPower::getInstance(this)->enableNotifications();
  wakeDone();
}

void HtcuniversalModemService::newBatteryAdded(const QString &msg)
{
  if (msg == "DS2760Battery") {
    battery = new QPowerSource(msg);
    connect(battery, SIGNAL(chargeChanged(int)),
	    this, SLOT(chargeChanged(int)));
    connect(battery, SIGNAL(chargingChanged(bool)),
	    this, SLOT(chargingChanged(bool)));
  }
}

void HtcuniversalModemService::chargeChanged(int charge)
{
  indicators()->setBatteryCharge(charge, 100,
				 battery->charging() ?
				 QModemIndicators::NotPoweredByBattery :
				 QModemIndicators::PoweredByBattery);
}

void HtcuniversalModemService::chargingChanged(bool isCharging)
{
  indicators()->setBatteryCharge(battery->charge(), 100,
				 isCharging ?
				 QModemIndicators::NotPoweredByBattery :
				 QModemIndicators::PoweredByBattery);
}
