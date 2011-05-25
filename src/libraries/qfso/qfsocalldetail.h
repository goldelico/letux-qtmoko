#ifndef QFSOCALLDETAIL_H
#define QFSOCALLDETAIL_H
#include <QtDBus>

class QFsoCallDetail
{
public:
    explicit QFsoCallDetail();
    int id;
    QString status;
    QVariantMap properties;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoCallDetail & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoCallDetail & value);
    static void registerMetaType();
};

Q_DECLARE_METATYPE(QFsoCallDetail)

typedef QList<QFsoCallDetail> QFsoCallDetailList;
Q_DECLARE_METATYPE(QFsoCallDetailList)

#endif // QFSOCALLDETAIL_H

