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

#include "htcuniversalcallvolume.h"

HtcuniversalCallVolume::HtcuniversalCallVolume(QModemService *service)
  : QModemCallVolume(service)
{
  this->service = service;

  setValue("MinimumSpeakerVolume", 0);
  setValue("MaximumSpeakerVolume", 5);
  setSpeakerVolume(3);

  setValue("MinimumMicrophoneVolume", 0);
  setValue("MaximumMicrophoneVolume", 0);
  setMicrophoneVolume(0);
}

HtcuniversalCallVolume::~HtcuniversalCallVolume() { }

bool HtcuniversalCallVolume::hasDelayedInit() const
{
  return false;
}

void HtcuniversalCallVolume::setSpeakerVolume(int volume)
{
  int boundedVolume = qBound(-10, volume, 15);

  setValue("SpeakerVolume", boundedVolume);
  service->chat(QString("AT@VOLSET=%1").arg((int)((boundedVolume + 10) / 5)));
  emit speakerVolumeChanged(boundedVolume);
}

void HtcuniversalCallVolume::setMicrophoneVolume(int volume)
{
  int boundedVolume = qBound(value("MinimumMicrophoneVolume").toInt(), volume,
			     value("MaximumMicrophoneVolume").toInt());

  setValue("MicrophoneVolume", boundedVolume);
  emit microphoneVolumeChanged(boundedVolume);
}
