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

SOURCES +=\
    org.freesmartphone.Device.LED.cpp \
    org.freesmartphone.GSM.Device.cpp \
    org.freesmartphone.GSM.Call.cpp \
    org.freesmartphone.GSM.Network.cpp \
    org.freesmartphone.GSM.CallDetail.cpp \
    org.freesmartphone.GSM.CallDetailList.cpp \
    org.freesmartphone.GSM.NetworkProviderList.cpp \
    org.freesmartphone.GSM.NetworkProvider.cpp

HEADERS +=\
    org.freesmartphone.Device.LED.h \
    org.freesmartphone.GSM.Device.h \
    org.freesmartphone.GSM.Call.h \
    org.freesmartphone.GSM.Network.h \
    org.freesmartphone.GSM.CallDetail.h \
    org.freesmartphone.GSM.CallDetailList.h \
    org.freesmartphone.GSM.NetworkProviderList.h \
    org.freesmartphone.GSM.NetworkProvider.h