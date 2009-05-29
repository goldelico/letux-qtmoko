TEMPLATE=app
TARGET=screenshot
CONFIG+=qtopia no_singleexec no_quicklaunch no_tr


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

pkg [
	name=screenshot
	desc="Screenshot Application"
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

target [
    hint=sxe
    domain=trusted
]
