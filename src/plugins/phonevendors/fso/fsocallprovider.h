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
#include <qphonecallprovider.h>
#include <qfsogsmcall.h>
#include "fsomodemservice.h"
#include "fsoutil.h"
#include "fsophonecall.h"

class FsoCallProvider : public QPhoneCallProvider
{
    Q_OBJECT
public:
    FsoCallProvider( const QString & service, QObject * parent );
    ~FsoCallProvider();
    
    QFsoGSMCall gsmCall;

    void dial(FsoPhoneCall *call, const QDialOptions& options);
    void hangup(FsoPhoneCall *call, QPhoneCall::Scope scope);
    
protected:
    QPhoneCallImpl *create
        ( const QString& identifier, const QString& callType );

private slots:
    void callStatusChange(int id, const QString &status, const QVariantMap &properties);
        
};

#endif

