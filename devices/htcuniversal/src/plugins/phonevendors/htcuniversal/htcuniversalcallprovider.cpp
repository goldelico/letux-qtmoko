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

#include "htcuniversalcallprovider.h"
#include "htcuniversalgsmpower.h"

#include <qatutils.h>
#include <qmodemcall.h>

#include <QDebug>

HtcuniversalCallProvider::HtcuniversalCallProvider(QModemService *service)
  : QModemCallProvider(service), current_id(0)
{
  service->primaryAtChat()->registerNotificationType
    ("@HTCDIS:", this, SLOT(htcdisSlot(QString)));
  connect(HtcuniversalGsmPower::getInstance(), SIGNAL(gsmReset()),
	  this, SLOT(resetModemIdentifier()));
}

HtcuniversalCallProvider::~HtcuniversalCallProvider() { }

void HtcuniversalCallProvider::htcdisSlot(const QString &str)
{
  QModemCall *call;
  if ((call = incomingCall()) != 0)
    missedTimeout(call);
  else
    hangupRemote(0);
}

QModemCallProvider::AtdBehavior HtcuniversalCallProvider::atdBehavior() const
{
  // When ATD reports OK, it is unknown whether the call is dialing or
  // connected and AT+CLCC must be used to determine the status.
  // This is the default.
  return AtdOkIsDialing;
}

void HtcuniversalCallProvider::abortDial(uint id, QPhoneCall::Scope scope)
{
  // Use the ATH command to abort outgoing calls, instead of AT+CHLD=1.
  atchat()->chat("ATH");
}

QString HtcuniversalCallProvider::releaseCallCommand(uint id) const
{
  return "ATH";
}

QString HtcuniversalCallProvider::releaseActiveCallsCommand() const
{
  return "ATH";
}

uint HtcuniversalCallProvider::nextModemIdentifier()
{
  return current_id = ++current_id >= 10 ? 1 : current_id;
}

void HtcuniversalCallProvider::resetModemIdentifier()
{
  current_id = 0;
}
