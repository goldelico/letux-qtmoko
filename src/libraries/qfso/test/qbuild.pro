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
SOURCES += main.cpp \
    mainwindow.cpp \
    ../orgfreesmartphonedeviceled.cpp \
    ../orgfreesmartphonegsmdevice.cpp \
    ../orgfreesmartphonegsmcall.cpp \
    ../orgfreesmartphonegsmnetwork.cpp \
    ../orgfreesmartphonegsmcalldetail.cpp \
    ../orgfreesmartphonegsmcalldetaillist.cpp \
    ../orgfreesmartphonegsmnetworkproviderlist.cpp \
    ../orgfreesmartphonegsmnetworkprovider.cpp

HEADERS += mainwindow.h \
    ../orgfreesmartphonedeviceled.h \
    ../orgfreesmartphonegsmdevice.h \
    ../orgfreesmartphonegsmcall.h \
    ../orgfreesmartphonegsmnetwork.h \
    ../orgfreesmartphonegsmcalldetail.h \
    ../orgfreesmartphonegsmcalldetaillist.h \
    ../orgfreesmartphonegsmnetworkproviderlist.h \
    ../orgfreesmartphonegsmnetworkprovider.h

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

