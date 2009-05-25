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

#include "openezxkbdhandler.h"

#include <QFile>
#include <QTextStream>
#include <QScreen>
#include <QSocketNotifier>
#include <QDebug>

#include <qtopialog.h>

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <QtCore/qstringlist.h>

struct openezxInput {
    unsigned int   dummy1;
    unsigned int   dummy2;
    unsigned short type;
    unsigned short code;
    unsigned int   value;
};

openezxKbdHandler::openezxKbdHandler(const QString &device)
{
    qWarning( "***Loaded openezx keyboard plugin!");
    setObjectName( "openezx Keyboard Handler" );
    devName = device;

    if (devName.isNull()) {
      getEnvDevice(); 
      if (devName.isNull()) {
         qWarning( "openezxKbdHandler(): no device specified. defaulting to input/event3" );
         devName = "/dev/input/event1";
      } 
    }

    kbdFD = ::open(devName.toLocal8Bit().constData(), O_RDONLY, 0);
    if (kbdFD >= 0) {
      qWarning("Set device %s to handle KEYBOARD", devName.toLocal8Bit().constData());
      m_notify = new QSocketNotifier( kbdFD, QSocketNotifier::Read, this );
      connect( m_notify, SIGNAL(activated(int)), this, SLOT(readKbdData()));
    } else {
      qWarning("Cannot open (%s)  for keyboard (%s)", 
                devName.toLocal8Bit().constData(), strerror(errno));
      return;
    }
	shift=false;
        alt=false;
        ctrl=false;
}

openezxKbdHandler::~openezxKbdHandler()
{
}

void openezxKbdHandler::getEnvDevice() 
{
  QString keyboards = QString::fromLatin1(qgetenv("QWS_KEYBOARD"));

  if (keyboards.isEmpty())
     return ;

  QString device;
  QString type;
  QStringList keyboard = keyboards.split(QLatin1Char(' '));
  for (int i = keyboard.size() - 1; i >= 0; --i) {
     const QString spec = keyboard.at(i);
     int colon=spec.indexOf(QLatin1Char(':'));
     if (colon>=0) {
         type = spec.left(colon);
         device = spec.mid(colon+1);
     } else {
         type = spec;
         device = "";
     }
     if ( type.toLower() == "openezxkbdhandler" ) {
       devName = device;
       return;
     }
  } 
  return ;
}

void openezxKbdHandler::readKbdData()
{
    openezxInput event;

    int n = read(kbdFD, &event, sizeof(openezxInput));
    if(n !=16) {
      qWarning("keypressed: n=%03d",n);
      return;
    }  

    int modifiers=0;
    int unicode=0xffff;
    int key_code=0;

    switch(event.code)
    {
           // Keypad
            case 0x52: key_code = Qt::Key_0; unicode  = 0x30; break;
            case 0x4F: key_code = Qt::Key_1; unicode  = 0x31; break;
            case 0x50: key_code = Qt::Key_2; unicode  = 0x32; break;
            case 0x51: key_code = Qt::Key_3; unicode  = 0x33; break;
            case 0x4B: key_code = Qt::Key_4; unicode  = 0x34; break;
            case 0x4C: key_code = Qt::Key_5; unicode  = 0x35; break;
            case 0x4D: key_code = Qt::Key_6; unicode  = 0x36; break;
            case 0x47: key_code = Qt::Key_7; unicode  = 0x37; break;
            case 0x48: key_code = Qt::Key_8; unicode  = 0x38; break;
            case 0x49: key_code = Qt::Key_9; unicode  = 0x39; break;

            case 0x37: key_code = Qt::Key_Asterisk; unicode  = 0x2A; break;
            case 0x53: key_code = Qt::Key_NumberSign; unicode  = 0x23; break;

            // Navigation+
            case 0xA9: key_code = Qt::Key_Call; break;
            case 0xDF: key_code = Qt::Key_Hangup; break;
            case 0x66: key_code = Qt::Key_Back; break;  // HOME BUTTON GO TO HOMESCREEN
            case 0x8B: key_code = Qt::Key_Back; break;  // BETWEEN CALL HANGUP TO GO ONE STEP BACK
            case 0x67: key_code = Qt::Key_Up; break;
            case 0x6C: key_code = Qt::Key_Down; break;
            case 0x69: key_code = Qt::Key_Left; break;
            case 0x6A: key_code = Qt::Key_Right; break;
            case 0x60: key_code = Qt::Key_Select; break;

            // Keys on left hand side of device
            case 0x6D: key_code = Qt::Key_VolumeUp; break;
            case 0x68: key_code = Qt::Key_VolumeDown; break;
            case 0x1c:  key_code = Qt::Key_F34; break; // HIT THE VOLUMEKEY FOR SHUTDOWN MENU

            // Camera
            case 0xD4: key_code = Qt::Key_F4; break; 

            // Record voice used as unlock phone for now
            case 0xA7: key_code = Qt::Key_F29; break; // (F35 for recording) IS NOW USED FOR UNLOCKING PHONE

            // Close/Open the flip
            case 0x8E: key_code = Qt::Key_Hangup; break;  // FLIP CLOSE

            // Key on headphones
//            case 0x33: key_code = Qt::Key_F28; break;

    }
    if(shift) modifiers |= Qt::ShiftModifier;
    if(ctrl) modifiers |= Qt::ControlModifier;
    if(alt) modifiers |= Qt::AltModifier;

    processKeyEvent(unicode, key_code, (Qt::KeyboardModifiers)modifiers,
      event.value!=0, false);
}
