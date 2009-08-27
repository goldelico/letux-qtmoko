TEMPLATE=plugin
TARGET = htckbdhandler

PLUGIN_FOR=qt
PLUGIN_TYPE=kbddrivers

CONFIG+=qtopia
QTOPIA=base

HEADERS = htckbdmap.h htckbddriverplugin.h htckbdhandler.h
SOURCES = htckbddriverplugin.cpp htckbdhandler.cpp

