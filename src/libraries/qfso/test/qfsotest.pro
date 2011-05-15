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
    ../org.freesmartphone.GSM.Network.cpp \
    ../org.freesmartphone.GSM.CallDetail.cpp \
    ../org.freesmartphone.GSM.CallDetailList.cpp \
    ../org.freesmartphone.GSM.NetworkProviderList.cpp \
    ../org.freesmartphone.GSM.NetworkProvider.cpp
HEADERS += mainwindow.h \
    ../org.freesmartphone.Device.LED.h \
    ../org.freesmartphone.GSM.Device.h \
    ../org.freesmartphone.GSM.Call.h \
    ../org.freesmartphone.GSM.Network.h \
    ../org.freesmartphone.GSM.CallDetail.h \
    ../org.freesmartphone.GSM.CallDetailList.h \
    ../org.freesmartphone.GSM.NetworkProviderList.h \
    ../org.freesmartphone.GSM.NetworkProvider.h
FORMS += mainwindow.ui
