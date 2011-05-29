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

#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>

template <class T, class T2, class T3>
        int checkReply(QDBusPendingReply<T, T2, T3> & reply,
                   const QString & fn,
                   bool waitForFinished)
{
    if(waitForFinished)
    {
        reply.waitForFinished();
    }
    if(!reply.isFinished())
    {
        return -1;
    }
    if(reply.isValid())
    {
        return 1;
    }
    QString err = reply.error().message();
    qWarning() << "Error in " << fn << err;
    return 0;
}

#endif

