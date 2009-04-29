TEMPLATE=lib
CONFIG+=embedded
TARGET=mad
VERSION=0.15

MODULE_NAME=libmad
MODULES*=pthread
LICENSE=GPL

HEADERS		= libmad_version.h \
                  fixed.h \
                  bit.h \
                  timer.h \
                  stream.h \
                  frame.h \
                  synth.h \
                  decoder.h \
                  layer12.h \
                  layer3.h \
                  huffman.h \
                  global.h \
                  version.h \
                  libmad_global.h \
                  mad.h
SOURCES	        = version.c \
                  fixed.c \
                  bit.c \
                  timer.c \
                  stream.c \
                  frame.c \
                  synth.c \
                  decoder.c \
                  layer12.c \
                  layer3.c \
                  huffman.c

DEFINES += OPT_SPEED OPT_SSO PIC

equals(arch,i386):DEFINES += FPM_INTEL
else:equals(arch,arm):DEFINES += FPM_ARM
else:equals(arch,mips):DEFINES += FPM_MIPS
else:equals(arch,powerpc):DEFINES += FPM_PPC
else:DEFINES += FPM_64BIT

