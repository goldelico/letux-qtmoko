/*
 *   ksame 0.4 - simple Game
 *   Copyright (C) 1997,1998  Marcus Kreutzberger <kreutzbe@informatik.mu-luebeck.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#include "mainwindow.h"

#ifdef QTOPIA_PHONE
#include <QtopiaApplication>
using namespace QSame;
QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,MainWindow)
QTOPIA_MAIN
#else
#include <QApplication>


int main( int argc, char **argv )
{
	QApplication app(argc,argv);

	QSame::MainWindow *w = new QSame::MainWindow();
	w->show();

	return app.exec();
}
#endif
