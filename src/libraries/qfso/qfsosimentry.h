#ifndef QFSOSIMENTRY_H
#define QFSOSIMENTRY_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoSIMEntry
{
public:
    explicit QFsoSIMEntry();
    int index;
    QString name;
    QString number;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMEntry & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMEntry & value);
};

Q_DECLARE_METATYPE(QFsoSIMEntry)

typedef QList<QFsoSIMEntry> QFsoSIMEntryList;
Q_DECLARE_METATYPE(QFsoSIMEntryList)

#endif // QFSOSIMENTRY_H

