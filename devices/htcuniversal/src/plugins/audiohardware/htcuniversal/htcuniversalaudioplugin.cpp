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

#include "htcuniversalaudioplugin.h"

#include <QAudioState>
#include <QAudioStateInfo>
#include <QValueSpaceItem>
#include <QtopiaIpcAdaptor>
#include <QtopiaIpcEnvelope>

#include <QDebug>
#include <qplugin.h>
#include <qtopialog.h>

#include <stdio.h>
#include <stdlib.h>
#include <QProcess>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <QDir>

static inline bool setAudioMode(int mode)
{
  QString m_mode;
  switch (mode) {
  case 0:
    m_mode = "stereoout";
    break;
  case 1:
    m_mode = "gsmhandset";
    break;
  case 2:
    m_mode = "gsmheadset";
    break;
  case 3:
    m_mode = "gsmbluetooth";
    break;
  case 4:
    m_mode = "gsmspeakerout";
    break;
  };

  QString confDir;
  if( QDir("/etc/alsa").exists())
    confDir="/etc/alsa/";
  else
    confDir="/etc/";

//   QString cmd = "/usr/sbin/alsactl -f "+confDir+m_mode+".state restore";
//   qLog(AudioState)<< "cmd="<<cmd;
//   int result = system(cmd.toLocal8Bit());

//   qLog(AudioState)<< "setAudioMode "<< QString( "/etc/alsa/%1.state").arg(m_mode);
//   if(result == 0)
//     return true;
//   qLog(AudioState)<< QString("Setting audio mode to: %1 failed").arg( m_mode);
//   return false;

//   QtopiaIpcEnvelope e("QPE/AudioVolumeManager/HtcuniversalVolumeService", "changeAmpModeVS()");
  return true;
}

class HandsetAudioState : public QAudioState
{
  Q_OBJECT

public:
  HandsetAudioState(bool isPhone,QObject *parent = 0);

  QAudioStateInfo info() const;
  QAudio::AudioCapabilities capabilities() const;

  bool isAvailable() const;
  bool enter(QAudio::AudioCapability capability);
  bool leave();

private:
  QAudioStateInfo m_info;
  bool m_isPhone;
};

HandsetAudioState::HandsetAudioState(bool isPhone, QObject *parent)
  : QAudioState(parent)
{
  qLog(AudioState)<<"HandsetAudioState"<<isPhone;
  m_isPhone = isPhone;

  m_info.setDomain("Phone");
  m_info.setProfile("PhoneSpeaker");
  m_info.setDisplayName(tr("Handset"));

  m_info.setPriority(50);
}

QAudioStateInfo HandsetAudioState::info() const
{
  return m_info;
}

QAudio::AudioCapabilities HandsetAudioState::capabilities() const
{
  return QAudio::InputOnly | QAudio::OutputOnly;
}

bool HandsetAudioState::isAvailable() const
{
  return true;
}

bool HandsetAudioState::enter(QAudio::AudioCapability capability)
{
  qLog(AudioState)<<"HandsetAudioState::enter"<<"isPhone";

  Q_UNUSED(capability);
    
  return setAudioMode( 1);
}

bool HandsetAudioState::leave()
{
  return true;
}

class MediaSpeakerAudioState : public QAudioState
{
  Q_OBJECT

public:
  MediaSpeakerAudioState(bool isPhone, QObject *parent = 0);

  QAudioStateInfo info() const;
  QAudio::AudioCapabilities capabilities() const;

  bool isAvailable() const;
  bool enter(QAudio::AudioCapability capability);
  bool leave();

private:
  QAudioStateInfo m_info;
  bool m_Phone;
};

MediaSpeakerAudioState::MediaSpeakerAudioState(bool isPhone, QObject *parent)
  : QAudioState(parent)
{
  m_Phone = isPhone;
  qLog(AudioState)<<"MediaSpeakerAudioState"<<isPhone;

  m_info.setDomain("Media");
  m_info.setProfile("MediaSpeaker");
  m_info.setDisplayName(tr("Stereo Speaker"));

  m_info.setPriority(150);
}

QAudioStateInfo MediaSpeakerAudioState::info() const
{
  return m_info;
}

QAudio::AudioCapabilities MediaSpeakerAudioState::capabilities() const
{
  return QAudio::OutputOnly;
}

bool MediaSpeakerAudioState::isAvailable() const
{
  return true;
}

bool MediaSpeakerAudioState::enter(QAudio::AudioCapability capability)
{
  qLog(AudioState)<<"MediaSpeakerAudioState::enter"<<"isPhone";

  Q_UNUSED(capability);

  return setAudioMode( 0);
}

bool MediaSpeakerAudioState::leave()
{
  qLog(AudioState)<<"MediaSpeakerAudioState::leave";
  return true;
}

class HeadphonesAudioState : public QAudioState
{
  Q_OBJECT

public:
  HeadphonesAudioState(bool isPhone, QObject *parent = 0);

  QAudioStateInfo info() const;
  QAudio::AudioCapabilities capabilities() const;

  bool isAvailable() const;
  bool enter(QAudio::AudioCapability capability);
  bool leave();

private slots:
void onHeadsetModified();

private:
  QAudioStateInfo m_info;
  bool m_isPhone;
  QValueSpaceItem *m_headset;
};

HeadphonesAudioState::HeadphonesAudioState(bool isPhone, QObject *parent)
  : QAudioState(parent)
{
  m_isPhone = isPhone;
  qLog(AudioState)<<"isPhone?"<<isPhone;

  if (isPhone) {
    m_info.setDomain("Phone");
    m_info.setProfile("PhoneHeadphones");
    m_info.setDisplayName(tr("Headphones"));
  } else {
    m_info.setDomain("Media");
    m_info.setProfile("MediaHeadphones");
    m_info.setDisplayName(tr("Headphones"));
  }

  m_info.setPriority(25);

  m_headset = new QValueSpaceItem("/Hardware/Accessories/PortableHandsfree/Present", this);
  connect( m_headset, SIGNAL(contentsChanged()),
	   this, SLOT(onHeadsetModified()));

  //    adaptor = new QtopiaIpcAdaptor("QPE/HtcuniversalModem", this );
}

QAudioStateInfo HeadphonesAudioState::info() const
{
  return m_info;
}

QAudio::AudioCapabilities HeadphonesAudioState::capabilities() const
{
  return QAudio::InputOnly | QAudio::OutputOnly;
}

void HeadphonesAudioState::onHeadsetModified()
{
  qLog(AudioState)<<"HeadphonesAudioState::onHeadsetModified()";
  bool avail = m_headset->value().toBool();

  if(avail) {
    this->enter( QAudio::OutputOnly);
  } else {
    this->leave();
  }

  emit availabilityChanged(avail);
}

bool HeadphonesAudioState::isAvailable() const
{
  return m_headset->value().toBool();
}

bool HeadphonesAudioState::enter(QAudio::AudioCapability capability)
{
  Q_UNUSED(capability)
    qLog(AudioState)<<"HeadphonesAudioState::enter"<<"isPhone"<<m_isPhone;

  qLog(AudioState) << "HeadphonesAudioState::enter" << capability;

  QtopiaIpcEnvelope e("QPE/AudioVolumeManager/HtcuniversalVolumeService", "setAmp(QString)");
  e << QString("Headphones");

  m_info.setPriority(25);

  return true;
}

bool HeadphonesAudioState::leave()
{
  qLog(AudioState)<<" HeadphonesAudioState::leave()"<<m_isPhone;

  if(m_isPhone) {

    QtopiaIpcEnvelope e("QPE/AudioVolumeManager/HtcuniversalVolumeService", "setAmp(QString)");
    e << QString("Stereo Speakers + Headphones");

  } else {

    QtopiaIpcEnvelope e("QPE/AudioVolumeManager/HtcuniversalVolumeService", "setAmp(QString)");
    e << QString("Stereo Speakers + Headphones");
  }
  m_info.setPriority(200);

  return true;
}

class SpeakerphoneAudioState : public QAudioState
{
  Q_OBJECT

public:
  SpeakerphoneAudioState(QObject *parent = 0);

  QAudioStateInfo info() const;
  QAudio::AudioCapabilities capabilities() const;

  bool isAvailable() const;
  bool enter(QAudio::AudioCapability capability);
  bool leave();

private:
  QAudioStateInfo m_info;
};

SpeakerphoneAudioState::SpeakerphoneAudioState(QObject *parent)
  : QAudioState(parent)
{
  m_info.setDomain("Phone");
  m_info.setProfile("PhoneSpeakerphone");
  m_info.setDisplayName(tr("Speakerphone"));
  m_info.setPriority(100);
}

QAudioStateInfo SpeakerphoneAudioState::info() const
{
  return m_info;
}

QAudio::AudioCapabilities SpeakerphoneAudioState::capabilities() const
{
  return QAudio::InputOnly | QAudio::OutputOnly;
}

bool SpeakerphoneAudioState::isAvailable() const
{
  return true;
}

bool SpeakerphoneAudioState::enter(QAudio::AudioCapability capability)
{
  //handset
  Q_UNUSED(capability);
  qLog(AudioState)<< " SpeakerphoneAudioState::enter";
  return setAudioMode( 4);
}

bool SpeakerphoneAudioState::leave()
{
  qLog(AudioState)<<" SpeakerphoneAudioState::leave";
  return true;
}

class HtcuniversalAudioPluginPrivate
{
public:
  QList<QAudioState *> m_states;
};

HtcuniversalAudioPlugin::HtcuniversalAudioPlugin(QObject *parent)
  : QAudioStatePlugin(parent)
{
  m_data = new HtcuniversalAudioPluginPrivate;

  m_data->m_states.push_back(new HandsetAudioState(this));

  m_data->m_states.push_back(new MediaSpeakerAudioState(this));

  m_data->m_states.push_back(new HeadphonesAudioState(false, this));
  m_data->m_states.push_back(new HeadphonesAudioState(true, this));

  m_data->m_states.push_back(new SpeakerphoneAudioState(this));
}

HtcuniversalAudioPlugin::~HtcuniversalAudioPlugin()
{
  for (int i = 0; m_data->m_states.size(); i++)
    delete m_data->m_states.at(i);
  delete m_data;
}

QList<QAudioState *> HtcuniversalAudioPlugin::statesProvided() const
{
  return m_data->m_states;
}

Q_EXPORT_PLUGIN2(htcuniversalaudio_plugin, HtcuniversalAudioPlugin)

#include "htcuniversalaudioplugin.moc"
