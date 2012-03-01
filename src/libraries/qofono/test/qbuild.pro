TEMPLATE=app
TARGET=qofonotest

CONFIG+=qtopia
DEFINES+=QTOPIA
QT+=dbus
INCLUDEPATH+=$$shellQuote($$path(,project))/..

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qofonotest
    desc="oFono test program for QtMoko"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
SOURCES += main.cpp \
    mainwindow.cpp \
    ofonoutil.cpp \
    ../qofonodbusabstractinterface.cpp \
    ../ofonosimmanager.cpp \
    ../ofonoconnectionmanager.cpp \
    ../ofonosupplementaryservices.cpp \
    ../qofonodbuspendingcall.cpp \
    ../qofonoutil.cpp \
    ../ofonopushnotification.cpp \
    ../ofonocellbroadcast.cpp \
    ../ofononetworkregistration.cpp \
    ../ofonomessagemanager.cpp \
    ../ofonoradiosettings.cpp \
    ../ofonovoicecallmanager.cpp \
    ../ofonophonebook.cpp \
    ../ofonomanager.cpp \
    ../qofonoobjectlist.cpp

HEADERS += mainwindow.h \
    ofonoutil.h \
    qtelephonynamespace.h \
    ../ofonovoicecallmanager.h \
    ../ofonophonebook.h \
    ../ofonosupplementaryservices.h \
    ../ofonoradiosettings.h \
    ../qofonodbuspendingcall.h \
    ../qofonodbusabstractinterface.h \
    ../ofonoconnectionmanager.h \
    ../qofonoutil.h \
    ../qofonodbuspendingreply.h \
    ../ofonopushnotification.h \
    ../ofonocellbroadcast.h \
    ../qofonostringmap.h \
    ../ofononetworkregistration.h \
    ../ofonomessagemanager.h \
    ../ofonosimmanager.h \
    ../ofonomanager.h \
    ../qofonoobjectlist.h

FORMS += mainwindow.ui

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qofonotest.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qofonotest
]

