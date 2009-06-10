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

#ifndef HTCUNIVERSALHARDWARE_H
#define HTCUNIVERSALHARDWARE_H


#include <QObject>
#include <QProcess>

#include <qvaluespace.h>

class QSocketNotifier;
class QtopiaIpcAdaptor;

class HtcuniversalHardware : public QObject
{
    Q_OBJECT

public:
    HtcuniversalHardware(QObject *parent = 0);
    ~HtcuniversalHardware();

public slots:
    void setBrightness(int bright = -3);

private:
    QSocketNotifier *hingeNotify;
    int hingeFd;
    

private slots:
    void readHingeData();
    void setRotate(int rot = 0);
    void shutdownRequested();
};


#endif // HTCUNIVERSALHARDWARE_H
