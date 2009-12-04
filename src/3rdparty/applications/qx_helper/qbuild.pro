TEMPLATE=app
TARGET=qx_helper

CONFIG+=qtopia
LIBS+=-lX11
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qx_helper
    desc="X launcher helper"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

SOURCES=qx_helper.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]