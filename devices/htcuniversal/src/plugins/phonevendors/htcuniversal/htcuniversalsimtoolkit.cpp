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

#include "htcuniversalsimtoolkit.h"

#include <qatutils.h>
#include <qmodempinmanager.h>

HtcuniversalSimToolkit::HtcuniversalSimToolkit(QModemService *service)
  : QModemSimToolkit(service)
{
  supportsStk = false;
  lastCommand.setType(QSimCommand::NoCommand);
  mainMenu = lastCommand;
  lastResponseWasExit = false;

  service->primaryAtChat()->registerNotificationType
    ("%SATA:", this, SLOT(sataNotification(QString)));
  service->primaryAtChat()->registerNotificationType
    ("%SATN:", this, SLOT(satnNotification(QString)));
}

HtcuniversalSimToolkit::~HtcuniversalSimToolkit()
{
}

void HtcuniversalSimToolkit::initialize()
{
  // We don't need to do anything here, because SIM toolkit initialization
  // happens during the detection code.
  QModemSimToolkit::initialize();
}

void HtcuniversalSimToolkit::begin()
{
  if (!supportsStk) {

    // SIM toolkit functionality is not available.
    emit beginFailed();

  } else if (lastCommand.type() == QSimCommand::SetupMenu) {

    // We just fetched the main menu, so return what we fetched.
    emit command(lastCommand);

  } else if (mainMenu.type() == QSimCommand::SetupMenu) {

    // We have a cached main menu from a previous invocation.
    lastCommand = mainMenu;
    lastCommandBytes = mainMenuBytes;
    emit command(mainMenu);

  } else {

    // The SIM toolkit is in an unknown state, so we cannot proceed.
    // If the HTCUNIVERSAL could perform a proper STK reset, we might have
    // been able to do something.
    emit beginFailed();

  }
}

void HtcuniversalSimToolkit::sendResponse(const QSimTerminalResponse& resp)
{
  if (resp.command().type() == QSimCommand::SelectItem &&
       resp.result() == QSimTerminalResponse::BackwardMove) {
    lastResponseWasExit = true;
  } else {
    lastResponseWasExit = false;
  }
  service()->primaryAtChat()->chat
    ("AT@SATR=\"" + QAtUtils::toHex(resp.toPdu()) + "\"");
}

void HtcuniversalSimToolkit::sendEnvelope(const QSimEnvelope& env)
{
  service()->primaryAtChat()->chat
    ("AT@SATE=\"" + QAtUtils::toHex(env.toPdu()) + "\"");
}

void HtcuniversalSimToolkit::sataNotification(const QString& msg)
{
  // SIM toolkit command indication.
  QByteArray bytes = QAtUtils::fromHex(msg.mid(6));
  if (bytes.size() > 0) {

    lastCommandBytes = bytes;
    lastCommand = QSimCommand::fromPdu(bytes);
    if (lastCommand.type() == QSimCommand::SetupMenu) {
      // Cache the main menu, because we won't get it again!
      mainMenuBytes = bytes;
      mainMenu = lastCommand;
    }
    qLog(AtChat)<< "SIM command of type" << (int)(lastCommand.type());
    emitCommandAndRespond(lastCommand);

  } else if (lastResponseWasExit &&
	      mainMenu.type() == QSimCommand::SetupMenu) {

    // We exited from a submenu and we got an empty "%SATA"
    // response.  This is the HTCUNIVERSAL's way of telling us that we
    // now need to display the main menu.  It would have been
    // better if the HTCUNIVERSAL resent the menu to us itself.
    lastCommandBytes = mainMenuBytes;
    lastCommand = mainMenu;
    qLog(AtChat)<< "Simulating SIM command of type"<< (int)(lastCommand.type());
    emit command(lastCommand);

  }
}

void HtcuniversalSimToolkit::satnNotification(const QString&)
{
  // Nothing to do here at present.  Just ignore the %SATN notifications.
}
