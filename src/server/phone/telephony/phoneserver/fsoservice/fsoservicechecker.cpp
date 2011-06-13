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

#include "fsoservicechecker.h"
#include "fsotelephonyservice.h"

/*!
    Construct a server-side service checker for FSO services.
*/
FsoServiceChecker::FsoServiceChecker
        ( FsoTelephonyService *service )
    : QServiceChecker( service->service(), service, Server )
{
    setValid( true );
}

/*!
    Destroy this server-side FSO service checker.
*/
FsoServiceChecker::~FsoServiceChecker()
{
}
