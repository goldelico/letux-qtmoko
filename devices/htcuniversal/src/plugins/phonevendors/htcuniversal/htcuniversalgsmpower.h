/*
 *  Jeremy Compostella <jeremy.compostella@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef HTCUNIVERSALGSMPOWER_H
#define HTCUNIVERSALGSMPOWER_H

#include <qmodemservice.h>
#include <QFile>

class HtcuniversalGsmPower : public QObject
{
 Q_OBJECT
   public:
 static HtcuniversalGsmPower *getInstance(QModemService *service = 0);

 public slots:
   void init();
 void turnOff();
 void turnOn();
 void enableNotifications();
 void disableNotifications();

 signals:
    void gsmReset();

 private:
 HtcuniversalGsmPower(QModemService *service);
 void chat(const char *str);

 static HtcuniversalGsmPower *instance;
 QModemService *service;
 QFile *powerFile;
 bool on;
};

#endif /* HTCUNIVERSALGSMPOWER_H */
