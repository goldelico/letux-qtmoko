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

#ifndef HTCUNIVERSALCALLPROVIDER_H
#define HTCUNIVERSALCALLPROVIDER_H

#include <qmodemservice.h>
#include <qmodemcallprovider.h>
#include <qtimer.h>

class HtcuniversalCallProvider : public QModemCallProvider
{
  Q_OBJECT
    public:
  HtcuniversalCallProvider(QModemService *service);
  ~HtcuniversalCallProvider();
  virtual uint nextModemIdentifier();

  public slots:
    void htcdisSlot(const QString &str);

  private slots:
    void resetModemIdentifier();

 protected:
  QModemCallProvider::AtdBehavior atdBehavior() const;
  void abortDial(uint id, QPhoneCall::Scope scope);
  QString releaseCallCommand(uint id) const;
  QString releaseActiveCallsCommand() const;
  uint current_id;
};

#endif /* HTCUNIVERSALCALLPROVIDER_H */
