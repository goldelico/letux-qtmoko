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
SOURCES +=\
    ../org.freesmartphone.Device.LED.cpp \
    ../org.freesmartphone.GSM.Device.cpp \
    ../org.freesmartphone.GSM.NetworkProvider.cpp \
    ../org.freesmartphone.GSM.NetworkProviderList.cpp \
    ../org.freesmartphone.GSM.Network.cpp \
    ../org.freesmartphone.GSM.CallDetail.cpp \
    ../org.freesmartphone.GSM.CallDetailList.cpp \
    ../org.freesmartphone.GSM.Call.cpp \
    mainwindow.cpp \
    main.cpp

HEADERS += mainwindow.h \
    ../org.freesmartphone.Device.LED.h \
    ../org.freesmartphone.GSM.Device.h \
    ../org.freesmartphone.GSM.Call.h \
    ../org.freesmartphone.GSM.Network.h \
    ../org.freesmartphone.GSM.CallDetail.h \
    ../org.freesmartphone.GSM.CallDetailList.h \
    ../org.freesmartphone.GSM.NetworkProviderList.h \
    ../org.freesmartphone.GSM.NetworkProvider.h

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

