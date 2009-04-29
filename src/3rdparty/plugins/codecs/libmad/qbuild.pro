TEMPLATE=plugin
CONFIG+=qtopia
TARGET=madplugin

PLUGIN_FOR=qtopia
PLUGIN_TYPE=codecs

QTOPIA*=media
MODULES*=libmad

HEADERS = \
        mp3plugin.h \
        mp3decoder.h

SOURCES = \
        mp3plugin.cpp \
        mp3decoder.cpp

DEFINES += OPT_SPEED OPT_SSO PIC

equals(arch,i386):DEFINES += FPM_INTEL
else:equals(arch,arm):DEFINES += FPM_ARM
else:equals(arch,mips):DEFINES += FPM_MIPS
else:equals(arch,powerpc):DEFINES += FPM_PPC
else:DEFINES += FPM_64BIT

