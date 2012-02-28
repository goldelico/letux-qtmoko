SOURCES = qfsodbusxml2cpp.cpp
DESTDIR = .
TARGET = qfsodbusxml2cpp
QT = core xml
CONFIG += qdbus
CONFIG -= app_bundle
win32:CONFIG += console

target.path=$$[QT_INSTALL_BINS]
INSTALLS += target
