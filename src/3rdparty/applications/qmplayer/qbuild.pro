TEMPLATE=app
TARGET=qmplayer

CONFIG+=qtopia

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qmplayer
    desc="No Description"
    license=Unknown
    version=1.0
    maintainer="Unknown <unknown@example.com>"
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

