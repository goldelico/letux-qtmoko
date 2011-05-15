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
    ../org_freesmartphone_Device_LED.cpp \
    ../org_freesmartphone_GSM_Device.cpp \
    ../org_freesmartphone_GSM_NetworkProvider.cpp \
    ../org_freesmartphone_GSM_NetworkProviderList.cpp \
    ../org_freesmartphone_GSM_Network.cpp \
    ../org_freesmartphone_GSM_CallDetail.cpp \
    ../org_freesmartphone_GSM_CallDetailList.cpp \
    ../org_freesmartphone_GSM_Call.cpp \
    mainwindow.cpp \
    main.cpp

HEADERS += mainwindow.h \
    ../org_freesmartphone_Device_LED.h \
    ../org_freesmartphone_GSM_Device.h \
    ../org_freesmartphone_GSM_Call.h \
    ../org_freesmartphone_GSM_Network.h \
    ../org_freesmartphone_GSM_CallDetail.h \
    ../org_freesmartphone_GSM_CallDetailList.h \
    ../org_freesmartphone_GSM_NetworkProviderList.h \
    ../org_freesmartphone_GSM_NetworkProvider.h

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

