#ifndef QFSOWORLDCOUNTRY_H
#define QFSOWORLDCOUNTRY_H
#include <QtDBus>

class QFsoWorldCountry
{
public:
    explicit QFsoWorldCountry();
    QString code;
    QString name;
    friend QDBusArgument &operator<<(QDBusArgument &argument, const QFsoWorldCountry & value);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, QFsoWorldCountry & value);
};

Q_DECLARE_METATYPE(QFsoWorldCountry)

typedef QList<QFsoWorldCountry> QFsoWorldCountryList;
Q_DECLARE_METATYPE(QFsoWorldCountryList)

#endif // QFSOWORLDCOUNTRY_H

