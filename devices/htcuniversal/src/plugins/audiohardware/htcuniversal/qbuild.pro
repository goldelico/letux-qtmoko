TEMPLATE=plugin
TARGET=htcuniversalaudiohardware
 
PLUGIN_FOR=qtopia
PLUGIN_TYPE=audiohardware
 
CONFIG+=qtopia
QTOPIA+=audio
enable_bluetooth:QTOPIA+=comm


HEADERS		=  htcuniversalaudioplugin.h
SOURCES	        =  htcuniversalaudioplugin.cpp
