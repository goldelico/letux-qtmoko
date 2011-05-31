#ifndef QFSOCALLDETAIL_H
#define QFSOCALLDETAIL_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoCallDetail
{
public:
    explicit QFsoCallDetail();
    int id;
    QString status;
    QVariantMap properties;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoCallDetail & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoCallDetail & value);
};

Q_DECLARE_METATYPE(QFsoCallDetail)

typedef QList<QFsoCallDetail> QFsoCallDetailList;
Q_DECLARE_METATYPE(QFsoCallDetailList)

#endif // QFSOCALLDETAIL_H

