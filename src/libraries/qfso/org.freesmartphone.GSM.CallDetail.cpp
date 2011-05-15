#include "org.freesmartphone.GSM.CallDetail.h"

OrgFreesmartphoneGSMCallDetail::OrgFreesmartphoneGSMCallDetail()
{
}

void OrgFreesmartphoneGSMCallDetail::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMCallDetail>("OrgFreesmartphoneGSMCallDetail");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMCallDetail>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMCallDetail & detail)
{
    argument.beginStructure();
    argument << detail.id;
    argument << detail.status;
    argument << detail.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMCallDetail & detail)
{
    argument.beginStructure();
    argument >> detail.id;
    argument >> detail.status;
    argument >> detail.properties;
    argument.endStructure();
    return argument;
}
