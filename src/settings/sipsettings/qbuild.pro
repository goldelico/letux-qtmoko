TEMPLATE=app
TARGET=sipsettings

CONFIG+=qtopia singleexec quicklaunch
QTOPIA*=phone collective
MODULES*=dissipate2

STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES
UNIFIED_NCT_LUPDATE=1

pkg [
    name=sipsettings
    desc="SIP settings for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

FORMS=\
    sipsettingsbase.ui

HEADERS=\
    sipsettings.h

SOURCES=\
    sipsettings.cpp\
    main.cpp

# Install rules

target [
    hint=sxe
    domain=trusted
]

help [
    hint=help
    source=help
    files=*.html
]

desktop [
    hint=desktop
    files=sipsettings.desktop
    path=/apps/Settings
]

pics [
    hint=pics
    files=pics/*
    path=/pics/sipsettings
]

