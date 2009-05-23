TEMPLATE=app
TARGET=keypebble
CONFIG+=qtopia no_singleexec no_quicklaunch

FORMS=vncoptionsbase.ui

HEADERS=d3des.h \
		  krfbbuffer.h \
		  krfbcanvas.h \
		  krfbconnection.h \
		  krfbdecoder.h \
		  krfblogin.h \
		  krfboptions.h \
		  krfbserverinfo.h \
		  kvnc.h \
		  kvncconnectdlg.h \
		  kvncoptionsdlg.h \
		  vncauth.h

SOURCES=d3des.c \
		  vncauth.c \
		  krfbbuffer.cpp \
		  krfbcanvas.cpp \
		  krfbconnection.cpp \
		  krfbdecoder.cpp \
		  krfblogin.cpp \
		  krfboptions.cpp \
		  kvnc.cpp \
		  kvncconnectdlg.cpp \
		  kvncoptionsdlg.cpp \
		  main.cpp

pkg [
    name=keypebble
    desc="Keypebble is a VNC client for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

target [
    hint=sxe
    domain=trusted
]

desktop [
    hint=desktop
    files=keypebble.desktop
    path=/apps/Applications
]

help [
    hint=help
    source=help
    files=*.html
]

pics [
    hint=pics
    files=pics/*
    path=/pics/keypebble
]
