/*
 *  Alessandro Briosi <alex@briosix.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include "simplefm.h"

#ifdef QTOPIA_PHONE

#include <qtopiaapplication.h>

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET,SimpleFM)
QTOPIA_MAIN

#else

#include <QApplication>

int main(int argc, char *argv[]) {

   QApplication app(argc,argv); 
   SimpleFM fm;
   fm.show();
   return app.exec();
}

#endif
