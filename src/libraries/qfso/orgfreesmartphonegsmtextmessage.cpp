#include "orgfreesmartphonegsmtextmessage.h"

OrgFreesmartphoneGSMTextMessage::OrgFreesmartphoneGSMTextMessage()
{
}

void OrgFreesmartphoneGSMTextMessage::registerMetaType()
{
    qRegisterMetaType<OrgFreesmartphoneGSMTextMessage>("OrgFreesmartphoneGSMTextMessage");
    qDBusRegisterMetaType<OrgFreesmartphoneGSMTextMessage>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMTextMessage & sms)
{
    argument.beginStructure();
    argument << sms.index;
    argument << sms.status;
    argument << sms.number;
    argument << sms.timestamp;
    argument << sms.content;
    argument << sms.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMTextMessage & sms)
{
    argument.beginStructure();
    argument >> sms.index;
    argument >> sms.status;
    argument >> sms.number;
    argument >> sms.timestamp;
    argument >> sms.content;
    argument >> sms.properties;
    argument.endStructure();
    return argument;
}
