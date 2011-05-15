#ifndef ORG_FREESMARTPHONE_GSM_CALLDETAIL_H
#define ORG_FREESMARTPHONE_GSM_CALLDETAIL_H

#include <QtDBus>

class OrgFreesmartphoneGSMCallDetail
{
public:
    explicit OrgFreesmartphoneGSMCallDetail();

    int id;
    QString status;
    QVariantMap properties;

    friend QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMCallDetail & detail);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMCallDetail & detail);

    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMCallDetail)

#endif // ORG_FREESMARTPHONE_GSM_CALLDETAIL_H
