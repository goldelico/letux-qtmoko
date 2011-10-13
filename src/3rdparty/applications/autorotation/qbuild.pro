TEMPLATE=app
TARGET=autorotation

CONFIG+=qtopia

STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES
UNIFIED_NCT_LUPDATE=1

pkg [
    name=autorotation
    desc="Rotation settings for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

HEADERS=\
    rotation.h

SOURCES=\
    rotation.cpp\
    main.cpp

# Install rules

target [
    hint=sxe
    domain=trusted
]

