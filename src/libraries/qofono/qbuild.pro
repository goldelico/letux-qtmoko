TEMPLATE=lib
TARGET=qofono
MODULE_NAME=qofono
VERSION=1.0.0

CONFIG+=qtopia hide_symbols singleexec

QT+=dbus

DEFINES+=QOFONO_LIBRARY
DEFINES+=QTMOKO

pkg [
    name=qofono
    desc="ofono.org bindings for QtMoko."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

SOURCES += qofonodbusabstractinterface.cpp \
    ofonosimmanager.cpp \
    ofonoconnectionmanager.cpp \
    ofonosupplementaryservices.cpp \
    qofonodbuspendingcall.cpp \
    qofonoutil.cpp \
    ofonopushnotification.cpp \
    ofonocellbroadcast.cpp \
    ofononetworkregistration.cpp \
    ofonomessagemanager.cpp \
    ofonoradiosettings.cpp \
    ofonovoicecallmanager.cpp \
    ofonophonebook.cpp \
    qofonoobjectlist.cpp

HEADERS += ofonovoicecallmanager.h \
    ofonophonebook.h \
    ofonosupplementaryservices.h \
    ofonoradiosettings.h \
    qofonodbuspendingcall.h \
    qofonodbusabstractinterface.h \
    ofonoconnectionmanager.h \
    qofonoutil.h \
    qofonodbuspendingreply.h \
    ofonopushnotification.h \
    ofonocellbroadcast.h \
    qofonostringmap.h \
    ofononetworkregistration.h \
    ofonomessagemanager.h \
    ofonosimmanager.h \
    qofonoobjectlist.h