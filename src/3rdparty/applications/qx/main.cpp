#ifdef QT_QWS_FICGTA01

#include "qx.h"
#include <qtopiaapplication.h>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,QX)
QTOPIA_MAIN

#else

#include <QtGui/QApplication>
#include "qx.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QX w;
    w.show();
    return a.exec();
}


#endif
