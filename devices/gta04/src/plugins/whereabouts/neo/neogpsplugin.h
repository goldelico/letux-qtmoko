/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
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

#ifndef NEOGPSPLUGIN_H
#define NEOGPSPLUGIN_H

#include <QSocketNotifier>
#include <QWhereaboutsPlugin>

class QWhereabouts;

class QTOPIA_PLUGIN_EXPORT NeoGpsPlugin : public QWhereaboutsPlugin
{
    Q_OBJECT
public:
    explicit NeoGpsPlugin(QObject *parent = 0);
    ~NeoGpsPlugin();

    virtual QWhereabouts *create(const QString &source);

private:
    int hasData;                // 1, we have data from serial port, 0 serial does not produce any data, -1 means that 3s timeout not finished yet
    QSocketNotifier *notifier;

private slots:
    void newDataAvailable();
    void hasDataTimeout();
};

#endif
