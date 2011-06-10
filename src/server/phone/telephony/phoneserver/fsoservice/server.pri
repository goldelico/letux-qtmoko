REQUIRES=enable_cell
QTOPIA*=phonemodem
MODULES*=qfso

SERVER_DEPS*=\
    /src/server/core_server\
    /src/server/phone/telephony/phoneserver/base\

HEADERS=\
    fsoplugin.h\
    fsomodemservice.h\
    fsonetworkregistration.h\
    fsosupplementaryservices.h\
    fsoservicechecker.h\
    fsorffunctionality.h\
    fsocallprovider.h\
    fsophonecall.h\
    fsoutil.h

SOURCES=\
    fsotelephonyservicefactory.cpp\
    fsoplugin.cpp\
    fsomodemservice.cpp\
    fsonetworkregistration.cpp\
    fsosupplementaryservices.cpp\
    fsoservicechecker.cpp\
    fsorffunctionality.cpp\
    fsocallprovider.cpp \
    fsophonecall.cpp

