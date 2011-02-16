TEMPLATE=app
QT+=svg
CONFIG+=qtopia
TARGET=cutemaze

pkg [
    name=cutemaze
    desc="Cutemaze."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]
DEFINES+=QTOPIA_PHONE

HEADERS=src/board.h \
	src/cell.h \
	src/maze.h \
	src/scores.h \
	src/settings.h \
	src/theme.h \
	src/window.h

SOURCES=src/board.cpp \
	src/cell.cpp \
	src/main.cpp \
	src/maze.cpp \
	src/scores.cpp \
	src/settings.cpp \
	src/theme.cpp \
	src/window.cpp

RESOURCES=themes/theme.qrc preview/preview.qrc

target [
    hint=sxe
    domain=trusted
]

desktop [
    hint=desktop
    files=cutemaze.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=icons/cutemaze.png
    path=/pics/cutemaze
]


