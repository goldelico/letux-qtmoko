#ifndef TEST_H
#define TEST_H

#include <QList>

class test : public QList
{
    Q_OBJECT
public:
    explicit test(QObject *parent = 0);

signals:

public slots:

};

#endif // TEST_H
