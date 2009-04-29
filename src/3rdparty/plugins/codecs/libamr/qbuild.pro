TEMPLATE=plugin
TARGET=amrrecordplugin

PLUGIN_FOR=qtopia
PLUGIN_TYPE=codecs

CONFIG+=qtopia singleexec
MODULES*=amr

HEADERS		=  amrrecord.h amrrecordimpl.h
SOURCES	        =  amrrecord.cpp amrrecordimpl.cpp

