#ifdef QTOPIA

#include "qx.h"
#include <qtopiaapplication.h>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,QxMainWindow)
QTOPIA_MAIN

#else

#include <QtGui/QApplication>
#include "qx.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QxMainWindow w;
    w.show();
    return a.exec();
}


#endif
