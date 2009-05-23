TEMPLATE=app
CONFIG+=qtopia quicklaunch
TARGET = gottet


HEADERS=src/board.h \
	src/piece.h \
	src/score_board.h \
	src/window.h

SOURCES=src/board.cpp \
	src/main.cpp \
	src/piece.cpp \
	src/score_board.cpp \
	src/window.cpp

RESOURCES=pics/icon.qrc
DEFINES+=QTOPIA_PHONE

pkg [
    name=gottet
    desc="Gottet"
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

target [
    hint=sxe
    domain=trusted
]

desktop [
    hint=desktop
    files=gottet.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/gottet.png
    path=/pics/gottet
]


