#include "org.freesmartphone.GSM.CallDetailList.h"

void OrgFreesmartphoneGSMCallDetailList::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMCallDetailList>("OrgFreesmartphoneGSMCallDetailList");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMCallDetailList>();
}
