TEMPLATE=lib
CONFIG+=qt embedded
TARGET=dissipate2
VERSION=1.0.0

MODULE_NAME=dissipate2
QT=core
MODULES*=resolv
LICENSE=LGPL

HEADERS=\
    base64.h\
    extern_md5.h\
    headerparser.h\
    messagesocket.h\
    mimecontenttype.h\
    parameterlist.h\
    sdp.h\
    sipcall.h\
    sipclient.h\
    sipdebug.h\
    sipheader.h\
    sipmessage.h\
    sipprotocol.h\
    sipregister.h\
    sipstatus.h\
    siptransaction.h\
    sipuri.h\
    sipurilist.h\
    sipuser.h\
    siputil.h\
    sipvia.h\
    sipvialist.h\
    tcpmessagesocket.h\
    udpmessagesocket.h\

SOURCES=\
    base64.c\
    extern_md5.c\
    headerparser.cpp\
    messagesocket.cpp\
    mimecontenttype.cpp\
    parameterlist.cpp\
    sdp.cpp\
    sipcall.cpp\
    sipclient.cpp\
    sipdebug.cpp\
    sipheader.cpp\
    sipmessage.cpp\
    sipprotocol.cpp\
    sipregister.cpp\
    sipstatus.cpp\
    siptransaction.cpp\
    sipuri.cpp\
    sipurilist.cpp\
    sipuser.cpp\
    siputil.cpp\
    sipvia.cpp\
    sipvialist.cpp\
    tcpmessagesocket.cpp\
    udpmessagesocket.cpp\

pkg.desc=SIP library
pkg.domain=trusted

# This project has tr()s but we don't call that code!!

