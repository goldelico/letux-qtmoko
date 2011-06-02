/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2011 Radek Polak.
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

#ifndef FSOCALLPROVIDER_H
#define FSOCALLPROVIDER_H

#include <QTimer>
#include <QDebug>
#include <qtopialog.h>
#include <qmodemcallprovider.h>
#include <qfsogsmcall.h>
#include "fsomodemservice.h"
#include "fsoutil.h"
#include "fsophonecall.h"

class FsoCallProvider : public QModemCallProvider
{
    Q_OBJECT
public:
    FsoCallProvider( FsoModemService * service );
    ~FsoCallProvider();
    
    QFsoGSMCall gsmCall;

protected:
    QModemCallProvider::AtdBehavior atdBehavior() const;
    void abortDial( uint modemIdentifier, QPhoneCall::Scope scope );
    QString releaseCallCommand( uint modemIdentifier ) const;
    QString releaseActiveCallsCommand() const;
    QString releaseHeldCallsCommand() const;
    QString putOnHoldCommand() const;
    QString setBusyCommand() const;
    QString activateCallCommand
	( uint modemIdentifier, bool otherActiveCalls ) const;
    QString activateHeldCallsCommand() const;
    QString joinCallsCommand( bool detachSubscriber ) const;
    QString deflectCallCommand( const QString& number ) const;
    QPhoneCallImpl *create
        ( const QString& identifier, const QString& callType );

protected slots:
    void resetModem();

private:
    FsoModemService *service;
};

#endif

