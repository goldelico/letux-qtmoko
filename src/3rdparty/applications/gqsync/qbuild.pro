TEMPLATE=app
TARGET=gqsync
CONFIG+=qtopia no_quicklaunch no_singleexec

#include (zlib/zlib.pri)

# Input
HEADERS += \
  googlesession.h \
  googlesync.h \
  googlecontact.h \
  loginwindow.h \
  gzip.h \

SOURCES += \
  main.cpp \
  googlesession.cpp \
  googlesession_parser.cpp \
  googlesync.cpp \
  googlecontact.cpp \
  loginwindow.cpp \
  gzip.cpp \


QT += xml network gui
QTOPIA*=pim
MODULES*=zlib

pkg [
    name=gqsync
    desc="Google synchronization tool"
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=Ilya Petrov <ilya.muromec@gmail.com>
]

target [
    hint=sxe
    domain=trusted
]

desktop [
    hint=desktop
    files=gqsync.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/gqsync
]

