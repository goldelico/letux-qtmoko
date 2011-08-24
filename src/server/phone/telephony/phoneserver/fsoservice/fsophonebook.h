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

#ifndef FSOPHONEBOOK_H
#define FSOPHONEBOOK_H

#include <QtDBus>
#include <qphonebook.h>
#include "fsoutil.h"
#include <qfsopimmessage.h>
#include <qfsopimmessages.h>
#include <qfsopimmessagequery.h>

class FsoTelephonyService;

class FsoPhoneBook : public QPhoneBook
{
    Q_OBJECT
public:
    FsoPhoneBook(FsoTelephonyService * service);
    ~FsoPhoneBook();

    FsoTelephonyService *service;

public slots:

private slots:
};

#endif
