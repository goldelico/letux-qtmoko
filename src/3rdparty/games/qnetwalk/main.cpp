//  qnetwalk/main.cpp
//  Copyright (C) 2004, Andi Peredri <andi@ukr.net>
//  Ported to Qt4 by xep

#include <QTextCodec>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include "mainwindow.h"

#ifdef QTOPIA_PHONE
#include <QtopiaApplication>
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,MainWindow)
QTOPIA_MAIN
#else
#include <QApplication>

int main(int argc, char** argv)
{
	Q_INIT_RESOURCE(pics);

	if(argc > 1 && QString(argv[1]) == "-help")
	{
		qWarning("Usage: qnetwalk [OPTIONS]\n"
				 "QNetWalk is a game for system administrators.\n"
				 "  -novice	    set the skill Novice\n"
				 "  -amateur	set the skill Amateur\n"
				 "  -expert	    set the skill Expert\n"
				 "  -master	    set the skill Master\n"
				 "  -nosound	disable all sound effects\n"
				 "  -help	    display this help and exit\n");
		return 1;
	}

	QApplication app(argc, argv);

	QString appdir = app.applicationDirPath();
	QString filename = QString("qnetwalk_") + QLocale::system().name();
	QTranslator translator(&app);
	if(!translator.load(filename, appdir + "/translations/"))
		translator.load(filename, appdir + "/../share/qnetwalk/translations/");
	app.installTranslator(&translator);

	MainWindow window;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	window.show();

	return app.exec();
}
#endif
