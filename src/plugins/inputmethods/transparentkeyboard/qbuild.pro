TEMPLATE=plugin
TARGET=transparentkeyboard

PLUGIN_FOR=qtopia
PLUGIN_TYPE=inputmethods

CONFIG+=qtopia singleexec

STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES

pkg [
    name=transparentkeyboard-inputmethod
    desc="Keyboard inputmethod plugin for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
    multi=plugins/inputmethods/transparentkeyboard
]

HEADERS=\
    keyboard.h\
    pickboardcfg.h\
    pickboardpicks.h\
    keyboardimpl.h\
    keyboardframe.h

SOURCES=\
    keyboard.cpp\
    pickboardcfg.cpp\
    pickboardpicks.cpp\
    keyboardimpl.cpp\
    keyboardframe.cpp

pics [
    hint=pics
    files=pics/*
    path=/pics/keyboard
]

