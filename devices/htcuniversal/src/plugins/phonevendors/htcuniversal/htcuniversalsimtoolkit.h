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

#ifndef HTCUNIVERSALSIMTOOLKIT_H
#define HTCUNIVERSALSIMTOOLKIT_H

#include <qmodemservice.h>
#include <qmodemsimtoolkit.h>

class HtcuniversalSimToolkit : public QModemSimToolkit
{
  Q_OBJECT
    public:
  HtcuniversalSimToolkit(QModemService *service);
  ~HtcuniversalSimToolkit();

  public slots:
    void initialize();
  void begin();
  void sendResponse(const QSimTerminalResponse& resp);
  void sendEnvelope(const QSimEnvelope& env);

  private slots:
    void sataNotification(const QString& msg);
  void satnNotification(const QString& msg);

 private:
  QSimCommand lastCommand;
  QByteArray lastCommandBytes;
  QSimCommand mainMenu;
  QByteArray mainMenuBytes;
  bool lastResponseWasExit;
};

static bool supportsStk = false;

#endif /* HTCUNIVERSALSIMTOOLKIT_H */
