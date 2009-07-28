#ifndef QXSERVICE_H
#define QXSERVICE_H

#include <QtopiaAbstractService>

class QX;

class QxService : public QtopiaAbstractService
{
    Q_OBJECT
public:
    QxService(QObject *parent);
    ~QxService() {}

public slots:
    void startApp(QString name);
    void appSwitch();

private:
    QX *qx;
};

#endif
