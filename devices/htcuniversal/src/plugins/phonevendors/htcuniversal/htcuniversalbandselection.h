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

#ifndef HTCUNIVERSALBANDSELECTION_H
#define HTCUNIVERSALBANDSELECTION_H

#include <qbandselection.h>
#include <qmodemservice.h>

class HtcuniversalBandSelection : public QBandSelection
{
  Q_OBJECT
    public:
  HtcuniversalBandSelection(QModemService *service);
  ~HtcuniversalBandSelection();

  public slots:
    void requestBand();
  void requestBands();
  void setBand(QBandSelection::BandMode mode, const QString& value);

  private slots:
    void bandQuery(bool ok, const QAtResult& result);
  void bandList(bool ok, const QAtResult& result);
  void bandSet(bool ok, const QAtResult& result);

 private:
  QModemService *service;
};

#endif /* HTCUNIVERSALBANDSELECTION_H */
