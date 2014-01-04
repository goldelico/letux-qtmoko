#ifdef QTOPIA

#include <qtopiaapplication.h>
#include "mainwindow.h"

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, MainWindow)
QTOPIA_MAIN

#else // QTOPIA

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#endif // QTOPIA
