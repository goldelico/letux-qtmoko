#ifndef ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_H
#define ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_H

#include <QtDBus>
#include <QVariantMap>

class OrgFreesmartphoneGSMTextMessage
{
public:
    explicit OrgFreesmartphoneGSMTextMessage();

    int index;
    QString status;
    QString number;
    QString timestamp;
    QString content;
    QVariantMap properties;

    friend QDBusArgument &operator<<(QDBusArgument &argument, const OrgFreesmartphoneGSMTextMessage & sms);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, OrgFreesmartphoneGSMTextMessage & sms);

    static void registerMetaType();
};

Q_DECLARE_METATYPE(OrgFreesmartphoneGSMTextMessage)

#endif // ORG_FREESMARTPHONE_GSM_TEXT_MESSAGE_H
