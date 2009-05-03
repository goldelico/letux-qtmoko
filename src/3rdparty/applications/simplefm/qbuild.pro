#qtopia_project(qtopia app)
TEMPLATE=app
TARGET=simplefm
CONFIG+=qtopia no_quicklaunch no_singleexec no_tr
DEFINES+=QTOPIA_PHONE
HEADERS = simplefm.h \
		qurlview.h \
		qabstracturlmodel.h \
		qlocalurlmodel.h \
		qsfinputdialog.h
SOURCES = main.cpp \
		simplefm.cpp \
		qurlview.cpp \
		qabstracturlmodel.cpp \
		qlocalurlmodel.cpp \
		qsfinputdialog.cpp

desktop [
files=simplefm.desktop
path=/apps/Applications
hint=desktop
]
#INSTALLS+=desktop
pics [
files=pics/*
path=/pics/simplefm/
hint=pics
]
#INSTALLS+=pics
pkg [
name=qpe-simplefm
desc=File Manager for Qtopia
domain=trusted
]
