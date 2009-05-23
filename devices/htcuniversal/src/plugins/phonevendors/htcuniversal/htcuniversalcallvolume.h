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

#ifndef HTCUNIVERSALCALLVOLUME_H
#define HTCUNIVERSALCALLVOLUME_H

#include <qmodemcallvolume.h>
#include <qmodemservice.h>

class HtcuniversalCallVolume : public QModemCallVolume
{
  Q_OBJECT
    public:
    
  explicit HtcuniversalCallVolume(QModemService *service);
  ~HtcuniversalCallVolume();

  public slots:
    virtual void setSpeakerVolume( int volume );
    virtual void setMicrophoneVolume( int volume );

 protected:
    bool hasDelayedInit() const;

 private:
  QModemService *service;
};

#endif /* HTCUNIVERSALCALLVOLUME_H */
