TEMPLATE=lib
TARGET=qfso
MODULE_NAME=qfso
VERSION=1.0.0

CONFIG+=qtopia hide_symbols singleexec

QT+=dbus

pkg [
    name=qfso
    desc="Freesmartphone.org bindings for QtMoko."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

SOURCES += \
    orgfreesmartphonedeviceled.cpp \
    orgfreesmartphonegsmdevice.cpp \
    orgfreesmartphonegsmcall.cpp \
    orgfreesmartphonegsmnetwork.cpp \
    orgfreesmartphonegsmcalldetail.cpp \
    orgfreesmartphonegsmcalldetaillist.cpp \
    orgfreesmartphonegsmnetworkproviderlist.cpp \
    orgfreesmartphonegsmnetworkprovider.cpp \
    orgfreesmartphonegsmsms.cpp \
    orgfreesmartphonegsmtextmessage.cpp \
    orgfreesmartphonegsmtextmessagelist.cpp
HEADERS += \
    orgfreesmartphonedeviceled.h \
    orgfreesmartphonegsmdevice.h \
    orgfreesmartphonegsmcall.h \
    orgfreesmartphonegsmnetwork.h \
    orgfreesmartphonegsmcalldetail.h \
    orgfreesmartphonegsmcalldetaillist.h \
    orgfreesmartphonegsmnetworkproviderlist.h \
    orgfreesmartphonegsmnetworkprovider.h \
    orgfreesmartphonegsmtextmessagelist.h \
    orgfreesmartphonegsmtextmessage.h \
    orgfreesmartphonegsmsms.h