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

#include "htcuniversalmultiplexer.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <qmultiportmultiplexer.h>
#include <qgsm0710multiplexer.h>
#include <qserialport.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>

QTOPIA_EXPORT_PLUGIN( HtcuniversalMultiplexerPlugin);

HtcuniversalMultiplexerPlugin::HtcuniversalMultiplexerPlugin(QObject* parent)
  : QSerialIODeviceMultiplexerPlugin(parent) { }

HtcuniversalMultiplexerPlugin::~HtcuniversalMultiplexerPlugin() { }


bool HtcuniversalMultiplexerPlugin::detect(QSerialIODevice *device)
{
  // make sure modem is in a valid state
#define resetFilename "/sys/class/power_driver/htcuniversal_phone/power_reset"
  QFile resetFile(resetFilename);
  if(!resetFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    qWarning() << resetFilename << " not opened";
#undef resetFilename
  } else {
    QTextStream out(&resetFile);
    out << "reset";
    resetFile.close();
  }

  // Issue an innocuous command to wake up the device.
  // It will respond with either "OK" or "AT-Command Interpreter ready".

  QSerialIODeviceMultiplexer::chat(device, "ATZ");
  QSerialIODeviceMultiplexer::chat(device, "ATE0V1");
  QSerialIODeviceMultiplexer::chat(device, "AT+CMUX");
  QSerialIODeviceMultiplexer::chat(device, "AT@USBPOWER=1");

  sleep(2); /* sleep a bit to let usb powering start */

  return true;
}

QSerialIODeviceMultiplexer *HtcuniversalMultiplexerPlugin::create
  (QSerialIODevice *device)
{
  QMultiPortMultiplexer *mux = new QMultiPortMultiplexer(device);
  QSerialIODevice *primary = mux->channel("primary");

  // Add secondary as primary
  mux->addChannel("secondary", primary);

  // Add the data channel.
  QSerialPort *data = QSerialPort::create("/dev/ttyUSB0");
  mux->addChannel("data", data);

  // Add the data setup channel, which is the same as "data".
  mux->addChannel("datasetup", device);
  return mux;
}
