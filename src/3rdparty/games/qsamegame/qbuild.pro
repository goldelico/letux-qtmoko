TEMPLATE=app
CONFIG+=qtopia singleexec quicklaunch
TARGET=qsamegame
QT+=svg

# Input
HEADERS=board.h mainwindow.h renderer.h view.h
SOURCES=board.cpp main.cpp mainwindow.cpp renderer.cpp view.cpp
DEFINES+=QTOPIA_PHONE
pkg [
    name=qsamegame
    desc="QSamegame"
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
    files=qsamegame.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/qsamegame.png
    path=/pics/qsamegame
]

