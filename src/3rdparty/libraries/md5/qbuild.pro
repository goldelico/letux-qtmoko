TEMPLATE=lib
CONFIG+=qt embedded singleexec
TARGET=md5

MODULE_NAME=md5
LICENSE=BSD GPL_COMPATIBLE
QT=core

# The bundled Qt tree provides md5.cpp/md5.h. With -system-qt,
# use Qt's QCryptographicHash through the local md5hash wrapper instead.
!system_qt {
    SOURCEPATH+=/qtopiacore/qt/src/3rdparty/md5
    SOURCES+=md5.cpp
    HEADERS+=md5.h
}

SOURCES+=md5hash.cpp
HEADERS+=md5hash.h md5hashglobal.h

