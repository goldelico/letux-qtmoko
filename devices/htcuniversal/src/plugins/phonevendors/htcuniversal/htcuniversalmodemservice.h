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

#ifndef HTCUNIVERSALMODEMSERVICE_H
#define HTCUNIVERSALMODEMSERVICE_H

#include <qmodemservice.h>
#include <qhardwaremanager.h>
#include <qpowersource.h>

class HtcuniversalModemService : public QModemService
{
  Q_OBJECT
    public:
  HtcuniversalModemService
    (const QString& service, QSerialIODeviceMultiplexer *mux,
      QObject *parent = 0);
  ~HtcuniversalModemService();

  void initialize();

  public slots:
    void signalQualityNotification(const QString &str);

  private slots:
  void configureDone(bool ok);
  void reset();
  void suspend();
  void wake();
  void newBatteryAdded(const QString &msg);
  void chargeChanged(int charge);
  void chargingChanged(bool isCharging);

 private:
  QHardwareManager *manager;
  QPowerSource *battery;
};

#endif /* HTCUNIVERSALMODEMSERVICE_H */
