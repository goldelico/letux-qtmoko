#ifndef QFSOSIMHOMEZONE_H
#define QFSOSIMHOMEZONE_H
#include <QtDBus>

class QFsoSIMHomeZone
{
public:
    explicit QFsoSIMHomeZone();
    QString name;
    int x;
    int y;
    int radius;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMHomeZone & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMHomeZone & value);
};

Q_DECLARE_METATYPE(QFsoSIMHomeZone)

typedef QList<QFsoSIMHomeZone> QFsoSIMHomeZoneList;
Q_DECLARE_METATYPE(QFsoSIMHomeZoneList)

#endif // QFSOSIMHOMEZONE_H

