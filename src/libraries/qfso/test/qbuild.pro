TEMPLATE=app
TARGET=qfsotest

CONFIG+=qtopia
DEFINES+=QTOPIA
QT+=dbus
INCLUDEPATH+=$$shellQuote($$path(,project))/..

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qfsotest
    desc="FSO test program for QtMoko"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
HEADERS=\
    mainwindow.h \
    ../org.freesmartphone.Device.LED.h \
	../org.freesmartphone.GSM.Device.h

SOURCES=\
    main.cpp \
    mainwindow.cpp \
    ../org.freesmartphone.Device.LED.cpp \
	../org.freesmartphone.GSM.Device.cpp

FORMS += mainwindow.ui

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qfsotest.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qfsotest
]

