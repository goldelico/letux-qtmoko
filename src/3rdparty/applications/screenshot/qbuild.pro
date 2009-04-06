#qtopia_project(qtopia app) # see buildsystem.html for more project keywords
TEMPLATE=app
TARGET=screenshot
CONFIG+=qtopia
CONFIG+=no_singleexec
CONFIG+=no_quicklaunch
CONFIG+=no_tr


HEADERS             = screenshot.h
SOURCES             = main.cpp \
                      screenshot.cpp
desktop [
	files=screenshot.desktop
	path=/apps/Applications
	hint=desktop
]

pics [
	files=pics/*
	path=/pics/screenshot
	hint=pics
]

#INSTALLS+=desktop pics

pkg [
	name=screenshot
	desc=Screenshot Application
	version=1.0.0-1
	maintainer=Trolltech (www.trolltech.com)
	license=GPL
	domain=window
]
