#ifndef QFSOSIMMESSAGE_H
#define QFSOSIMMESSAGE_H
#include <QtDBus>

class QFsoSIMMessage
{
public:
    explicit QFsoSIMMessage();
    int index;
    QString status;
    QString number;
    QString timestamp;
    QString contents;
    QVariantMap properties;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMMessage & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMMessage & value);
};

Q_DECLARE_METATYPE(QFsoSIMMessage)

typedef QList<QFsoSIMMessage> QFsoSIMMessageList;
Q_DECLARE_METATYPE(QFsoSIMMessageList)

#endif // QFSOSIMMESSAGE_H

