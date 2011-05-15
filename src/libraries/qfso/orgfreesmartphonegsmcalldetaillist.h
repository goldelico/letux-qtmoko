#ifndef ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H
#define ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H

#include <QObject>
#include <QList>
#include <QtDBus>

#include "orgfreesmartphonegsmcalldetail.h"

class OrgFreesmartphoneGSMCallDetailList : public QList<OrgFreesmartphoneGSMCallDetail>
{
public:
    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMCallDetailList)

#endif // ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H
