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

#include "htcuniversalbandselection.h"
#include <qatutils.h>
#include <qatresultparser.h>
#include <qpinmanager.h>

// Known bands, by mask.
typedef struct
{
  const char *name;
  int         value;

} BandInfo;
static BandInfo const bandInfo[] = {
  {"GSM 900",             1},
  {"DCS 1800",            2},
  {"PCS 1900",            4},
  {"E-GSM",               8},
  /*   {"GSM 850",             16}, */
  {"Tripleband 900/1800/1900", 15},
};
#define numBands    ((int)(sizeof(bandInfo) / sizeof(BandInfo)))

HtcuniversalBandSelection::HtcuniversalBandSelection(QModemService *service)
  : QBandSelection(service->service(), service, Server)
{
  this->service = service;
}

HtcuniversalBandSelection::~HtcuniversalBandSelection()
{
}

void HtcuniversalBandSelection::requestBand()
{
  service->primaryAtChat()->chat
    ("AT@BANDSEL?", this, SLOT(bandQuery(bool,QAtResult)));
}

void HtcuniversalBandSelection::requestBands()
{
  QStringList list;
  for (int index = 0; index < numBands; ++index) {
    list += QString(bandInfo[index].name);
  }
  emit bands(list);

  //     service->primaryAtChat()->chat
  //         ("AT%BAND=?", this, SLOT(bandList(bool,QAtResult)));
}

void HtcuniversalBandSelection::setBand(QBandSelection::BandMode mode, const QString& value)
{
  if (mode == Automatic) {
    service->primaryAtChat()->chat
      ("AT@BANDSEL=0", this, SLOT(bandSet(bool,QAtResult)));
  } else {
    int bandValue = 0;
    QStringList names = value.split(", ");
    foreach (QString name, names) {
      bool seen = false;
      for (int index = 0; index < numBands; ++index) {
	if (name == bandInfo[index].name) {
	  bandValue |= bandInfo[index].value;
	  seen = true;
	  break;
	}
      }
      if (!seen) {
	// The band name is not valid.
	emit setBandResult(QTelephony::OperationNotSupported);
	return;
      }
    }
    if (!bandValue) {
      // No band names supplied.
      emit setBandResult(QTelephony::OperationNotSupported);
      return;
    }
    service->primaryAtChat()->chat
      ("AT@BANDSEL=1," + QString::number(bandValue),
	this, SLOT(bandSet(bool,QAtResult)));
  }
}

// Convert a band value into a name.  Returns an empty list if unknown.
static QStringList bandValueToName(int bandValue)
{
  QStringList bands;
  for (int index = 0; index < numBands; ++index) {
    if ((bandValue & bandInfo[index].value) == bandInfo[index].value) {
      bandValue &= ~bandInfo[index].value;
      bands += QString(bandInfo[index].name);
    }
  }
  return bands;
}

void HtcuniversalBandSelection::bandQuery(bool, const QAtResult& result)
{

  QAtResultParser parser(result);
  int bandValue;
  qLog(AtChat)<<"bandQuery";
  if (parser.next("@BANDSEL:")) {
    bandValue = (int)parser.readNumeric();
  } else {
    // Command did not work, so assume "Auto".
    bandValue = 4;
  }
  for (int index = 0; index < numBands; ++index) {
    if (bandValue == bandInfo[index].value) {
      emit band(Manual, bandInfo[index].name);
      return;
    }
  }
  emit band(Automatic, QString());



  //     QAtResultParser parser(result);
  //     int bandValue;
  //     qLog(AtChat)<<"bandQuery";
  //     if (parser.next("%BAND:")) {
  //         if (parser.readNumeric() != 0) {
  //             bandValue = (int)parser.readNumeric();
  //             QStringList bands = bandValueToName(bandValue);
  //             if (bands.size() > 0) {
  //                 emit band(Manual, bands.join(", "));
  //                 return;
  //             }

  //         }
  //     }

  //     emit band(Automatic, QString());

}

void HtcuniversalBandSelection::bandList(bool, const QAtResult& result)
{
  QAtResultParser parser(result);
  QStringList bandNames;
  if (parser.next("@BANDSEL:")) {

    parser.readList();  // Skip list of supported modes.
    QList<QAtResultParser::Node> list = parser.readList();
    foreach (QAtResultParser::Node node, list) {

      if (node.isNumber()) {
	bandNames += bandValueToName((int)node.asNumber());
	qLog(AtChat)<<  (int)node.asNumber();
      } else if (node.isRange()) {
	int first = (int)node.asFirst();
	int last = (int)node.asLast();
	qLog(AtChat)<<"isRange"<<first<<last;
	while (first <= last) {
	  qLog(AtChat)<< bandValueToName(first) << first;
	  bandNames += bandValueToName(first).join(" | ");
	  ++first;
	}
      }
    }
  }
  emit bands(bandNames);
}

void HtcuniversalBandSelection::bandSet(bool, const QAtResult& result)
{
  emit setBandResult((QTelephony::Result)result.resultCode());
}
