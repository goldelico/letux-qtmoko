#ifdef QTOPIA

#include <qtopiaapplication.h>
#include "form.h"

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,Form)
QTOPIA_MAIN

#else

#include <QtGui/QApplication>
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form w;
    w.show();
    return a.exec();
}

#endif
