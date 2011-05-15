#ifndef ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H
#define ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H

#include <QObject>
#include <QList>
#include <QtDBus>
#include <QMap>

#include <org.freesmartphone.GSM.CallDetail.h>

class OrgFreesmartphoneGSMCallDetailList
{
public:
    explicit OrgFreesmartphoneGSMCallDetailList();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMCallDetailList &list);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMCallDetailList &list);

    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMCallDetailList)

#endif // ORG_FREESMARTPHONE_GSM_CALL_DETAIL_LIST_H
