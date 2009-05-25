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

#include "htcuniversalhardware.h"

#include <QSocketNotifier>
#include <QTimer>
#include <QLabel>
#include <QDesktopWidget>
#include <QProcess>
#include <QtopiaServiceRequest>

#include <qcontentset.h>
#include <qtopiaapplication.h>
#include <qtopialog.h>
#include <qtopiaipcadaptor.h>

#include <qbootsourceaccessory.h>
#include <qtopiaipcenvelope.h>

#include <qtopiaserverapplication.h>
#include <standarddevicefeatures.h>
#include <ui/standarddialogs.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <sys/ioctl.h>


struct HtcuniversalInput {
    unsigned int   dummy1;
    unsigned int   dummy2;
    unsigned short type;
    unsigned short code;
    unsigned int   value;
};

QTOPIA_TASK(HtcuniversalHardware, HtcuniversalHardware);

HtcuniversalHardware::HtcuniversalHardware(QObject *parent) :
	QObject(parent)
{

    qWarning()<<"htcuniversalhardware plugin";
    qWarning()<< "opening event for hinge";

    hingeFd = ::open("/dev/input/event2", O_RDONLY|O_NDELAY, 0);
     if (hingeFd >= 0) {
      hingeNotify = new QSocketNotifier( hingeFd, QSocketNotifier::Read, this );
      connect( hingeNotify, SIGNAL(activated(int)), this, SLOT(readHingeData()));
      qWarning()<<"hinge initialized";
    } else {
      qWarning("Cannot open /dev/input/event2 for hinge (%s)", strerror(errno));
    }
  /* when QPE quits, let's reset to default brightness: how to do this??? */
  /*connect ( QtopiaServerApplication::instance(), SIGNAL(shutdownRequested()), this, SLOT(setBrightness()));*/
}

HtcuniversalHardware::~HtcuniversalHardware()
{
}

void HtcuniversalHardware::setBrightness(int bright) {
    QtopiaServiceRequest eB("QtopiaPowerManager", "setBacklight(int)" );
    eB << bright; 
    eB.send();
}

void HtcuniversalHardware::setRotate(int rot) {
  /* tsdogs: this ain't working yet, let's leave it for future use */
  /*  QtopiaServiceRequest eB("QTransformedScreen", "setTransformation(int)" );
    eB << rot; //rotate value (?)
    eB.send(); */
}

void HtcuniversalHardware::readHingeData()
{

    HtcuniversalInput event;

    int n = read(hingeFd, &event, sizeof(HtcuniversalInput));
    if(n != (int)sizeof(HtcuniversalInput)) {
        return;
    }

    if(event.type != 5)
        return;

    qWarning("keypressed: type=%03d, code=%03d, value=%03d (%s)",
              event.type, event.code,event.value,((event.value)!=0) ? "Down":"Up");

    switch(event.code) {
      case 0x00: { //hinge open/close
        if (event.value != 0x01) {
          /* set to landscape and brightness on */
          this->setBrightness(-1); /* this should turn it on */
        } else {
           /* set to landscape with brightness off */
          this->setBrightness(0);
        }
        break;
      }
      case 0x01: { // hinge portrait/landscape
        if (event.value != 0x01) {
          // set to landscape
          qWarning()<<"rotating 270";
          this->setRotate(4);
        } else {
          // set to portrait
          qWarning()<<"rotating 90";
          this->setRotate(1);
        }
        break;
      }
    };
}

void HtcuniversalHardware::shutdownRequested()
{
    /*qLog(PowerManagement)<<" HtcuniversalHardware::shutdownRequested";

    QtopiaIpcEnvelope e("QPE/AudioVolumeManager/HtcuniversalVolumeService", "setAmpMode(bool)");
    e << false;

    QFile powerFile("/sys/bus/platform/devices/gta01-pm-gsm.0/power_on");
    if( !powerFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning()<<"File not opened";
    } else {
        QTextStream out(&powerFile);
        out << "0";
        powerFile.close();
    }

    QFile btPower("/sys/bus/platform/devices/gta01-pm-bt.0/power_on");
        if( !btPower.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning()<<"File not opened";
    } else {
        QTextStream out(&btPower);
        out <<  "0";
        powerFile.close();
    }


*/
  setBrightness(-3);
  QtopiaServerApplication::instance()->shutdown(QtopiaServerApplication::ShutdownSystem);
}


