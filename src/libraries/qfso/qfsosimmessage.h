#ifndef QFSOSIMMESSAGE_H
#define QFSOSIMMESSAGE_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoSIMMessage
{
public:
    explicit QFsoSIMMessage();
    int index;
    QString status;
    QString number;
    QString timestamp;
    QString contents;
    QVariantMap properties;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMMessage & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMMessage & value);
};

Q_DECLARE_METATYPE(QFsoSIMMessage)

typedef QList<QFsoSIMMessage> QFsoSIMMessageList;
Q_DECLARE_METATYPE(QFsoSIMMessageList)

#endif // QFSOSIMMESSAGE_H

