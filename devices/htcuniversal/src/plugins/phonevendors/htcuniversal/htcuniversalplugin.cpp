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

#include "htcuniversalplugin.h"
#include "htcuniversalmodemservice.h"

QTOPIA_EXPORT_PLUGIN(HtcuniversalPluginImpl);

HtcuniversalPluginImpl::HtcuniversalPluginImpl() { }

HtcuniversalPluginImpl::~HtcuniversalPluginImpl() { }

bool HtcuniversalPluginImpl::supports( const QString& manufacturer )
{
  return manufacturer.contains("Qualcomm MSM6250");
}

QModemService *HtcuniversalPluginImpl::create
  (const QString& service, QSerialIODeviceMultiplexer *mux, QObject *parent)
{
  return new HtcuniversalModemService(service, mux, parent);
}
