/****************************************************************************
**
** Copyright (C) 2000-2006 TROLLTECH ASA. All rights reserved.
**
** This file is part of the Phone Edition of the Qtopia Toolkit.
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

#ifndef OPENEZXKBDHANDLER_H
#define OPENEZXKBDHANDLER_H

#include <QObject>
#include <QWSKeyboardHandler>

class QSocketNotifier;
class openezxKbdHandler : public QObject, public QWSKeyboardHandler
{
    Q_OBJECT
public:
    openezxKbdHandler(const QString &device);
    ~openezxKbdHandler();

private:
    QSocketNotifier *m_notify;
    QString devName;
    int  kbdFD;
    bool shift;
    bool alt;
    bool ctrl;

    void getEnvDevice();

private Q_SLOTS:
    void readKbdData();
};

#endif // OPENEZXKBDHANDLER_H
