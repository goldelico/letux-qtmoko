TEMPLATE=plugin
TARGET=svgkeyboard

PLUGIN_FOR=qtopia
PLUGIN_TYPE=inputmethods

CONFIG+=qtopia singleexec

QT += svg

STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES

pkg [
    name=svgkeyboard-inputmethod
    desc="Keyboard inputmethod plugin for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
    multi=plugins/inputmethods/svgkeyboard
]

HEADERS=\
    keyboard.h\
    keyboardimpl.h\
    keyboardframe.h

SOURCES=\
    keyboard.cpp\
    keyboardimpl.cpp\
    keyboardframe.cpp

config [
    hint=image
    files=layouts/*
    path=/etc/im/svgkbd
]


