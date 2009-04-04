
#include "MainWindow.h"

#ifdef QTOPIA_PHONE

#include <qtopiaapplication.h>

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,MainWindow)
QTOPIA_MAIN

#else

#include <QApplication>

int main(int argc, char **argv)
{
  QApplication app(argc,argv);
  MainWindow mw;
  mw.show();
  return app.exec();
}

#endif
