#ifndef QFSOSIMHOMEZONE_H
#define QFSOSIMHOMEZONE_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoSIMHomeZone
{
public:
    explicit QFsoSIMHomeZone();
    QString name;
    int x;
    int y;
    int radius;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoSIMHomeZone & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoSIMHomeZone & value);
};

Q_DECLARE_METATYPE(QFsoSIMHomeZone)

typedef QList<QFsoSIMHomeZone> QFsoSIMHomeZoneList;
Q_DECLARE_METATYPE(QFsoSIMHomeZoneList)

#endif // QFSOSIMHOMEZONE_H

