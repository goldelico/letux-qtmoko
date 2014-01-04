TEMPLATE=app
TARGET=childmath

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=childmath=
    desc="Math training for children"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
HEADERS=\
    src/childmath.h

SOURCES=\
    src/main.cpp \
    src/childmath.cpp \

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=childmath.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/childmath
]

help [
    hint=help
    source=help
    files=*.html
]