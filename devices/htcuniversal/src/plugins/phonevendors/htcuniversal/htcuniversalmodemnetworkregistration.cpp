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

#include "htcuniversalmodemnetworkregistration.h"
#include <qpinmanager.h>

HtcuniversalModemNetworkRegistration::HtcuniversalModemNetworkRegistration
(QModemService *service)
  : QModemNetworkRegistration(service), currentName("")
{
  connect(this, SIGNAL(currentOperatorChanged()), this, SLOT(translateIdToName()));
}

HtcuniversalModemNetworkRegistration::~HtcuniversalModemNetworkRegistration() {}

static QString translation[][2] =
  { { "20201", "Cosmote"}, 
    { "20205", "PANAFON"},
    { "20210", "TELESTET"},
    { "20404", "LIBERTEL"},
    { "20408", "KPN Telecom"},
    { "20412", "O2"},
    { "20416", "BEN"},
    { "20420", "Dutchtone NV"},
    { "20601", "PROXIMUS"},
    { "20610", "Mobistar"},
    { "20620", "Base"},
    { "20801", "Orange"},
    { "20810", "SFR"},
    { "20820", "Bouygues Telecom"},
    { "21303", "MOBILAND"},
    { "21401", "Airtel GSM 900-Spain"},
    { "21403", "Retevision Movil"},
    { "21407", "MOVISTAR"},
    { "21601", "Pannon GSM"},
    { "21670", "Vodafone"},
    { "21630", "Westel 900"},
    { "21890", "GSMBIH"},
    { "21901", "CRONET"},
    { "21910", "VIP"},
    { "22001", "MOBTEL"},
    { "22002", "ProMonte GSM"},
    { "22003", "Telekom Srbije"},
    { "22201", "Telecom Italia Mobile"},
    { "22210", "OMNITEL"},
    { "22288", "Wind Telecomunicazioni SpA"},
    { "22601", "CONNEX GSM"},
    { "22610", "DIALOG"},
    { "22801", "NATEL International"},
    { "22802", "diAx Mobile AG"},
    { "23001", "T-Mobile CZ"},
    { "23002", "EuroTel"},
    { "23003", "Oskar"},
    { "23101", "Orange"},
    { "23102", "EuroTel GSM"},
    { "23201", "A1"},
    { "23203", "T-Mobile AT"},
    { "23205", "ONE"},
    { "23207", "tele.ring"},
    { "23410", "Cellnet"},
    { "23415", "Vodafone"},
    { "23430", "T-Mobile UK"},
    { "23433", "ORANGE"},
    { "23450", "Jersey Telecoms GSM"},
    { "23455", "Guernsey Telecoms GSM"},
    { "23458", "PRONTO GSM"},
    { "23801", "TDK-MOBIL"},
    { "23802", "SONOFON"},
    { "23820", "TELIA DK"},
    { "23830", "Mobilix"},
    { "24001", "Telia AB"},
    { "24007", "COMVIQ"},
    { "24008", "EUROPOLITAN"},
    { "24201", "Telenor Mobil"},
    { "24202", "NetCom GSM"},
    { "24403", "Telia City (Finland)"},
    { "24405", "Radiolinja"},
    { "24409", "Finnet"},
    { "24491", "Sonera"},
    { "24601", "OMNITEL"},
    { "24602", "Bite GSM"},
    { "24701", "LMT LV"},
    { "24702", "BALTCOM GSM"},
    { "24801", "EMT GSM"},
    { "24802", "Radiolinja Eesti AS"},
    { "24803", "Q GSM"},
    { "25001", "Mobile Telesystems"},
    { "25002", "North-West GSM"},
    { "25005", "Siberian Cellular Systems 900"},
    { "25007", "BM Telecom"},
    { "25010", "Don Telecom"},
    { "25012", "FECS-900"},
    { "25013", "Kuban GSM"},
    { "25039", "Uraltel"},
    { "25044", "North Caucasian GSM"},
    { "25099", "BeeLine"},
    { "25501", "UMC"},
    { "25502", "WellCOM"},
    { "25503", "Kyivstar"},
    { "25505", "Golden Telecom"},
    { "25901", "VOXTEL"},
    { "26001", "PLUS GSM"},
    { "26002", "ERA GSM"},
    { "26003", "IDEA Centertel"},
    { "26201", "T-Mobile D"},
    { "26202", "D2 PRIVAT"},
    { "26203", "E-Plus"},
    { "26207", "Interkom"},
    { "26601", "Gibtel GSM"},
    { "26801", "TELECEL"},
    { "26803", "OPTIMUS"},
    { "26806", "TMN"},
    { "27001", "LUXGSM"},
    { "27077", "TANGO"},
    { "27201", "EIRCELL-GSM"},
    { "27202", "Digifone"},
    { "27401", "Landssiminn GSM 900"},
    { "27402", "TAL hf"},
    { "27601", "AMC"},
    { "27801", "Vodafone Malta Limited"},
    { "28001", "CYTAGSM"},
    { "28201", "Geocell Limited"},
    { "28202", "Magti GSM"},
    { "28301", "ArmGSM"},
    { "28401", "M-TEL GSM BG"},
    { "28601", "Turkcell"},
    { "28602", "TELSIM GSM"},
    { "28801", "Faroese Telecom"},
    { "29001", "Tele Greenland"},
    { "29340", "SI.MOBIL d. d."},
    { "29341", "MOBITEL"},
    { "29370", "SI VEGA 070"},
    { "29401", "MobiMak"},
    { "30237", "Microcell Connexions Inc"},
    { "30272", "Rogers AT&T"},
    { "31001", "Cellnet"},
    { "31002", "Sprint Spectrum"},
    { "31011", "Wireless 2000 Telephone Co."},
    { "31015", "BellSouth Mobility DCS"},
    { "31016", "T-Mobile"},
    { "31017", "Pac Bell"},
    { "31020", "T-Mobile"},
    { "31021", "T-Mobile"},
    { "31022", "T-Mobile"},
    { "31023", "T-Mobile"},
    { "31024", "T-Mobile"},
    { "31025", "T-Mobile"},
    { "31026", "T-Mobile"},
    { "31027", "T-Mobile"},
    { "31031", "T-Mobile"},
    { "31038", "AT&T Wireless"},
    { "31058", "T-Mobile"},
    { "31066", "T-Mobile"},
    { "31077", "Iowa Wireless Services LP"},
    { "31080", "T-Mobile"},
    { "34001", "AMERIS"},
    { "40001", "AZERCELL GSM"},
    { "40002", "Bakcell GSM 2000"},
    { "40407", "TATA Cellular"},
    { "40410", "AirTel"},
    { "40411", "Essar Cellphone"},
    { "40412", "Escotel"},
    { "40414", "Modicom"},
    { "40415", "Essar Cellphone"},
    { "40420", "Max Touch"},
    { "40421", "BPL - Mobile"},
    { "40427", "BPL USWEST Cellular"},
    { "40430", "Command"},
    { "40440", "SkyCell"},
    { "40441", "RPG Cellular"},
    { "40442", "AIRCEL"},
    { "41001", "Mobilink"},
    { "41302", "DIALOG GSM"},
    { "41501", "CELLIS"},
    { "41503", "LIBANCELL"},
    { "41601", "Fastlink"},
    { "41709", "MOBILE SYRIA"},
    { "41902", "MTCNet"},
    { "42001", "Al Jawwal"},
    { "42007", "E.A.E"},
    { "42202", "GTO"},
    { "42402", "UAE-ETISALAT"},
    { "42501", "Partner Communications Company Ltd"},
    { "42601", "BHR MOBILE PLUS"},
    { "42701", "QATARNET"},
    { "43211", "TCI"},
    { "43404", "Daewoo Unitel"},
    { "43405", "Coscom"},
    { "43701", "Bitel"},
    { "45400", "TCSL GSM"},
    { "45404", "HKGHT"},
    { "45406", "SMARTONE GSM"},
    { "45410", "New World PCS"},
    { "45412", "PEOPLES"},
    { "45416", "SUNDAY"},
    { "45501", "TELEMOVEL+ GSM900-Macau"},
    { "45601", "MobiTel"},
    { "45602", "SAMART-GSM"},
    { "45701", "Lao Shinawatra Telecom"},
    { "46000", "China Telecom GSM"},
    { "46001", "CU-GSM"},
    { "46601", "Far EasTone Telecoms 900"},
    { "46606", "TUNTEX GSM 1800"},
    { "46688", "KG Telecom"},
    { "46692", "Chunghwa GSM"},
    { "46693", "MobiTai"},
    { "46697", "TWNGSM"},
    { "46699", "TransAsia"},
    { "47001", "GrameenPhone Ltd"},
    { "47019", "Mobile 2000"},
    { "50212", "Maxis Mobile"},
    { "50213", "TM Touch"},
    { "50216", "DiGi 1800"},
    { "50217", "ADAM"},
    { "50219", "CELCOM"},
    { "50501", "MobileNet"},
    { "50502", "OPTUS"},
    { "50503", "VODAFONE"},
    { "50508", "One.Tel"},
    { "51001", "SATELINDO"},
    { "51008", "LIPPO TELECOM"},
    { "51010", "TELKOMSEL"},
    { "51011", "Excelcom"},
    { "51021", "INDOSAT"},
    { "51501", "ISLACOM"},
    { "51502", "Globe Telecom"},
    { "52001", "AIS GSM"},
    { "52010", "WCS"},
    { "52018", "Worldphone 1800"},
    { "52023", "HELLO"},
    { "52501", "SingTel Mobile"},
    { "52502", "ST-PCN"},
    { "52503", "MOBILEONE"},
    { "52811", "DSTCom"},
    { "53001", "Vodafone New Zealand Limited"},
    { "54201", "Vodafone"},
    { "54601", "Mobilis"},
    { "54720", "VINI"},
    { "60201", "MobiNil"},
    { "60202", "Tunicell"},
    { "60301", "ALGERIAN MOBILE NETWORK"},
    { "60401", "I A M"},
    { "60801", "ALIZE"},
    { "61102", "Lagui"},
    { "61203", "IVOIRIS"},
    { "61205", "Telecel"},
    { "61501", "TOGO CELL"},
    { "61701", "Cellplus Mobile Comms"},
    { "61801", "Omega"},
    { "62001", "SPACEFON"},
    { "62501", "CVMOVEL"},
    { "63301", "Seychelles Cellular Services"},
    { "63310", "AIRTEL"},
    { "63401", "MobiTel"},
    { "63510", "Rwandacell"},
    { "63601", "ETMTN"},
    { "64001", "TRITEL"},
    { "64110", "MTN-Uganda"},
    { "64202", "ANTARIS"},
    { "64301", "T.D.M GSM 900"},
    { "64501", "ZAMCELL"},
    { "64601", "Madacom"},
    { "64603", "Sacel Madagascar S.A."},
    { "64710", "SRR"},
    { "64801", "NET*ONE"},
    { "64803", "Telecel"},
    { "64901", "MTC"},
    { "65001", "Callpoint 900"},
    { "65101", "Vodacom Lesotho (Pty) Ltd"},
    { "65202", "Vista" },
    { "65310", "Swazi MTN" },
    { "65501", "Vodacom"},
    { "65507", "Cell C" },
    { "65510", "MTN"},
    { "68038", "NPI Wireless"},
    { "71201", "ICE" }, 
    { "72235", "Port Hable" },
    { "72402", "TIM" },
    { "72403", "TIM" },
    { "72404", "TIM" },
    { "72431", "Oi" },
    { "73001", "Entel Telefonia Movi"},
    { "73010", "Entel PCS"},
    { "73401", "Infonet"},
    { "73402", "Digitel" },
    { "74601", "ICMS" },
    { "74602", "TeleG" },
    { "732101", "Comcel" },
    { "End", "Unknown" } };

void HtcuniversalModemNetworkRegistration::translateIdToName()
{
  if (currentOperatorName() != currentName) {
    currentName = "Unknown : " + currentOperatorName();
    for (unsigned int i = 0 ; translation[i][0] != "End" ; ++i)
      if (currentOperatorName() == translation[i][0]) {
	currentName = translation[i][1];
	break;
      }
    updateCurrentOperator(currentOperatorMode(), currentOperatorId(),
			  currentName, currentOperatorTechnology());
  }    
}
