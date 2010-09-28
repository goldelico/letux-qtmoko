# -------------------------------------------------
# Project created by QtCreator 2009-04-14T22:32:40
# -------------------------------------------------
TARGET = qx
TEMPLATE = app
LIBS += -lXtst
QT += core \
    gui \
    network
SOURCES += main.cpp \
    qx.cpp \
    apprunningscreen.cpp \
    AppInfo.cpp \
    AppSettings.cpp \
    DesktopScanner.cpp \
    rotate.cpp \
    libfakekey.c
HEADERS += qx.h \
    apprunningscreen.h \
    AppInfo.h \
    AppSettings.h \
    DesktopScanner.h \
    rotate.h \
    fakekey.h
