#ifdef QT_QWS_FICGTA01

#include "qmplayer.h"
#include <qtopiaapplication.h>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,QMplayer)
QTOPIA_MAIN

#else

#include <QtGui/QApplication>
#include "qmplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMplayer w;
    w.show();
    return a.exec();
}


#endif
