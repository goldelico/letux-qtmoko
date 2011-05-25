#ifndef QFSOSIMENTRY_H
#define QFSOSIMENTRY_H
#include <QtDBus>

class QFsoSIMEntry
{
public:
    explicit QFsoSIMEntry();
    int index;
    QString name;
    QString number;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMEntry & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMEntry & value);
    static void registerMetaType();
};

Q_DECLARE_METATYPE(QFsoSIMEntry)

typedef QList<QFsoSIMEntry> QFsoSIMEntryList;
Q_DECLARE_METATYPE(QFsoSIMEntryList)

#endif // QFSOSIMENTRY_H

