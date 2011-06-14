REQUIRES=enable_cell
QTOPIA*=phone
MODULES*=qfso

SERVER_DEPS*=\
    /src/server/core_server\
    /src/server/phone/telephony/phoneserver/base\

HEADERS+=\
        fsoutil.h\
        fsotelephonyservice.h\
        fsotelephonyservicefactory.h\
        fsonetworkregistration.h\
        fsosupplementaryservices.h\
        fsoservicechecker.h\
        fsorffunctionality.h\
        fsocallprovider.h\
        fsophonecall.h\
        fsosmssender.h

SOURCES+=\
        fsotelephonyservicefactory.cpp\
        fsotelephonyservice.cpp\
        fsonetworkregistration.cpp\
        fsosupplementaryservices.cpp\
        fsoservicechecker.cpp\
        fsorffunctionality.cpp\
        fsocallprovider.cpp\
        fsophonecall.cpp\
        fsosmssender.cpp

