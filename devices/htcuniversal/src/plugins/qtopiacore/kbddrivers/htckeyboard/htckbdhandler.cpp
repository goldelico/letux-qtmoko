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

#include "htckbdhandler.h"

#include <QFile>
#include <QTextStream>
#include <QSocketNotifier>
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>

#include <htckbdmap.h>

#include <linux/input.h>

htcKbdHandler::htcKbdHandler(const QString &device)
  : modifiers(Qt::NoModifier), capsLock(false)
{
  setObjectName("Htc keyboard Handler");

  kbdFD = ::open(device.toLocal8Bit().constData(), O_RDONLY, 0);
  if (kbdFD >= 0) {
    m_notify = new QSocketNotifier(kbdFD, QSocketNotifier::Read, this);
    connect(m_notify, SIGNAL(activated(int)), this, SLOT(processEvent()));
  } else
    qWarning("Cannot open %s", device.toLocal8Bit().constData());
}

htcKbdHandler::~htcKbdHandler() { }

void htcKbdHandler::processEvent()
{
#define MAX_EVENT 10
  struct input_event events[MAX_EVENT];
  unsigned int i;

  int n = read(kbdFD, &events, sizeof(struct input_event) * MAX_EVENT);
  for (i = 0 ; i < n / sizeof(struct input_event) ; ++i)
    processEvent(events[i]);
}

void htcKbdHandler::processEvent(struct input_event event)
{
  static struct input_event previous = { {0, 0}, 0, 0, 0};
  static uint previous_nb = 0;
  struct QWSKeyMap key;

  if (event.code > keyMSize || event.code == 0)
    return;
#define MIN_REPEAT 5

  key = htcuniversalKeyMap[event.code];

  if ((event.code == previous.code &&
       (previous.value == 0 ? !event.value : event.value)) && 
      (key.key_code == Qt::Key_Control || key.key_code == Qt::Key_Shift ||
       key.key_code == Qt::Key_CapsLock || key.key_code == Qt::Key_Alt ||
       previous_nb++ <= MIN_REPEAT))
    return;
  if (event.code != previous.code)
    previous_nb = 0;

  if (key.key_code == Qt::Key_Control)
    modifiers ^= Qt::ControlModifier;
  if (key.key_code == Qt::Key_Shift)
    modifiers ^= Qt::ShiftModifier;
  if (key.key_code == Qt::Key_Alt)
    modifiers ^= Qt::AltModifier;
  if (key.key_code == Qt::Key_CapsLock && event.value == 0)
    capsLock = !capsLock;

  ushort unicode = key.unicode;
  if (modifiers & Qt::ShiftModifier && !capsLock)
    unicode = key.shift_unicode;
  else if (modifiers & Qt::ControlModifier)
    unicode = key.ctrl_unicode;
  else if (modifiers & Qt::AltModifier)
    unicode = key.alt_unicode;
  else if (capsLock && !(modifiers & Qt::ShiftModifier))
    unicode = key.shift_unicode;
  processKeyEvent(unicode, key.key_code, modifiers, event.value != 0, false);

  previous = event;
}
