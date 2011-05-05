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

HEADERS=\
    org.freesmartphone.Device.LED.h

SOURCES=\
    org.freesmartphone.Device.LED.cpp
