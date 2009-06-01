TEMPLATE=app
CONFIG+=qtopia no_singleexec no_quicklaunch no_tr
TARGET = shopper

QT += sql

FORMS += shopperbase.ui newitembase.ui

HEADERS += shopper.h \
           shopperdatabase.h \
           newitem.h
SOURCES += main.cpp \
           shopper.cpp \
           newitem.cpp \
           shopperdatabase.cpp 


pkg [
    name=shopper
    desc="Shopping list."
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
    files=shopper.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/shopper
]
