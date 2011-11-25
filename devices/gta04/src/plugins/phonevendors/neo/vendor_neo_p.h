/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak <psonek2@seznam.cz>
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#ifndef VENDOR_NEO_P_H
#define VENDOR_NEO_P_H

#include <qmodemservice.h>
#include <qmodemcall.h>
#include <qmodemcallprovider.h>
#include <qmodemsimtoolkit.h>
#include <qmodemphonebook.h>
#include <qmodempinmanager.h>
#include <qmodempreferrednetworkoperators.h>
#include <qbandselection.h>
#include <qvibrateaccessory.h>
#include <qmodemnetworkregistration.h>
#include <qmodemsiminfo.h>
#include <qmodemcallvolume.h>
#include <qmodemsimfiles.h>
#include <qmodemservicenumbers.h>

#include <alsa/asoundlib.h>

class NeoModemService : public QModemService
{
    Q_OBJECT
public:
    NeoModemService
        (const QString & service, QSerialIODeviceMultiplexer * mux,
         QObject * parent = 0);
    ~NeoModemService();

    void initialize();
    bool supportsAtCced();
    
private slots:
    void sigq(const QString & msg);
    void reset();
    void suspend();
    void wake();
};

#endif
