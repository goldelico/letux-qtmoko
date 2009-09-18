TEMPLATE=app
TARGET=qmplayer

CONFIG+=qtopia quicklaunch
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qmplayer
    desc="MPlayer frontend"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
HEADERS=\
    qmplayer.h

SOURCES=\
    main.cpp\
    qmplayer.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qmplayer.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qmplayer
]

