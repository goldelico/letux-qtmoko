qtopia_project(embedded qtopia core plugin)
TARGET = htckbdhandler

CONFIG+=no_tr

HEADERS = htckbdmap.h htckbddriverplugin.h htckbdhandler.h
SOURCES = htckbddriverplugin.cpp htckbdhandler.cpp

