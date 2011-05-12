# -------------------------------------------------
# Project created by QtCreator 2011-05-05T11:33:54
# -------------------------------------------------
QT += dbus
TARGET = qfsotest
TEMPLATE = app
INCLUDEPATH += ..
SOURCES += main.cpp \
    mainwindow.cpp \
    ../org.freesmartphone.Device.LED.cpp \
    ../org.freesmartphone.GSM.Device.cpp \
    ../org.freesmartphone.GSM.Call.cpp \
    ../org.freesmartphone.GSM.Network.cpp
HEADERS += mainwindow.h \
    ../org.freesmartphone.Device.LED.h \
    ../org.freesmartphone.GSM.Device.h \
    ../org.freesmartphone.GSM.Call.h \
    ../org.freesmartphone.GSM.Network.h
FORMS += mainwindow.ui
