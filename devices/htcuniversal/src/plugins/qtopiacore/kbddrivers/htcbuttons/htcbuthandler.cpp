/****************************************************************************
 **
 ** Copyright (C) 2000-2006 TROLLTECH ASA. All rights reserved.
 **
 ** This file is part of the Phone Edition of the Qtopia Toolkit.
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

#include "htcbuthandler.h"

#include <QFile>
#include <QTextStream>
#include <QSocketNotifier>
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

htcButHandler::htcButHandler(const QString &device)
{
  setObjectName("HTC buttons Handler");
  butFD = ::open(device.toLocal8Bit().constData(), O_RDONLY, 0);
  if (butFD >= 0) {
    m_notify = new QSocketNotifier(butFD, QSocketNotifier::Read, this);
    connect(m_notify, SIGNAL(activated(int)), this, SLOT(processEvent()));
  } else
    qWarning("Cannot open %s", device.toLocal8Bit().constData());
}

htcButHandler::~htcButHandler() { }

void htcButHandler::processEvent()
{
  struct input_event event;
  
  int n = read(butFD, &event, sizeof(event));
  if (n != sizeof(event)) {
    qWarning("read event failed");
    return;
  }

  processEvent(event);
}

void htcButHandler::processEvent(struct input_event event)
{
  int unicode = 0xffff;
  int key_code = 0;

  switch(event.code) {
  case KEY_RECORD:
    key_code = Qt::Key_MediaRecord;
    goto keyevent;
  case KEY_CAMERA:
    key_code = Qt::Key_F4;
    goto keyevent;
  case KEY_VOLUMEDOWN:
    key_code = Qt::Key_VolumeDown;
    goto autorepeat;
  case KEY_VOLUMEUP:
    key_code = Qt::Key_VolumeUp;
    goto autorepeat;
  case KEY_POWER:
    key_code = Qt::Key_F34;
    goto keyevent;
  case KEY_UP:
    key_code = Qt::Key_Up;
    goto autorepeat;
  case KEY_RIGHT:
    key_code = Qt::Key_Right;
    goto autorepeat;
  case KEY_DOWN:
    key_code = Qt::Key_Down;
    goto autorepeat;
  case KEY_LEFT:
    key_code = Qt::Key_Left;
    goto autorepeat;
  case KEY_KPENTER:
    key_code = Qt::Key_Select;
    goto keyevent;
  case KEY_F10:
    {
      QFile light
	("/sys/devices/platform/asic3-leds.0/leds:htcuniversal:flashl/brightness");
      light.open((QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate));
      if (event.value)
	QTextStream(&light) << "1" << endl;
      else
	QTextStream(&light) << "0" << endl;
    }
    return;
  case SW_LID:
    return;
  case SW_TABLET_MODE:
    return;
  default:
    return;
  }

 autorepeat:
  if (event.value)
    beginAutoRepeat(unicode, key_code, Qt::NoModifier);
  else
    endAutoRepeat();
 keyevent:
  processKeyEvent(unicode, key_code, Qt::NoModifier, event.value != 0, false);
}
