/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2011 qtmoko project
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/
#include "rotation.h"
#include <qtopiaapplication.h>

// QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,RotationSettings)
// QTOPIA_MAIN

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RotationSettings w;

	switch(atoi(argv[1])) {
		case 0:
			w.setnewRotation(0);
			break;
		case 90:
			w.setnewRotation(90);
			break;
		case 180:
			w.setnewRotation(180);
			break;
		case 270:
			w.setnewRotation(270);
			break;
		default:
			break;
		
	}

	return 0;
}



