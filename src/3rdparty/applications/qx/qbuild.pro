TEMPLATE=app
TARGET=qx

CONFIG+=qtopia
LIBS+=-lX11
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qx
    desc="X launcher"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
HEADERS=\
    qx.h \
    qxservice.h \
    apprunningscreen.h

SOURCES=\
    main.cpp\
    qx.cpp \
    qxservice.cpp \
    apprunningscreen.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qx.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qx
]

service [
    hint=image
    files=services/QX/qx
    path=/services/QX
]

servicedef [
    hint=image
    files=services/QX.service
    path=/services
]

