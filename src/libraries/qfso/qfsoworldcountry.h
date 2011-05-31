#ifndef QFSOWORLDCOUNTRY_H
#define QFSOWORLDCOUNTRY_H
#include <QtDBus>

#if defined(QFSO_LIBRARY)
    #define QFSO_EXPORT Q_DECL_EXPORT
#else
    #define QFSO_EXPORT Q_DECL_IMPORT
#endif

class QFSO_EXPORT QFsoWorldCountry
{
public:
    explicit QFsoWorldCountry();
    QString code;
    QString name;
    QFSO_EXPORT friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWorldCountry & value);
    QFSO_EXPORT friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWorldCountry & value);
};

Q_DECLARE_METATYPE(QFsoWorldCountry)

typedef QList<QFsoWorldCountry> QFsoWorldCountryList;
Q_DECLARE_METATYPE(QFsoWorldCountryList)

#endif // QFSOWORLDCOUNTRY_H

