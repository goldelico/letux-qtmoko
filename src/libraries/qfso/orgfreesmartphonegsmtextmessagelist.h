#ifndef ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_LIST_H
#define ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_LIST_H

#include <QtDBus>
#include <QList>
#include "orgfreesmartphonegsmtextmessage.h"

class OrgFreesmartphoneGSMTextMessageList : public QList<OrgFreesmartphoneGSMTextMessage>
{
public:
    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMTextMessageList)

#endif // ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_LIST_H
