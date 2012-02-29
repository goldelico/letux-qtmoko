TEMPLATE=app
TARGET=qofonotest

CONFIG+=qtopia
DEFINES+=QTOPIA
QT+=dbus
INCLUDEPATH+=$$shellQuote($$path(,project))/..

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

# Package info
pkg [
    name=qofonotest
    desc="oFono test program for QtMoko"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
SOURCES += main.cpp \
    mainwindow.cpp \
    ofonoutil.cpp \
    ../qofonoutil.cpp \
    ../qofonodbuspendingcall.cpp \
    ../qofonopreferencesservice.cpp \
    ../qofonopimnote.cpp \
    ../qofonomusicplayer.cpp \
    ../qofonopimcalls.cpp \
    ../qofonoaudiomanager.cpp \
    ../qofonodevicepowercontrol.cpp \
    ../qofonosimhomezone.cpp \
    ../qofonotimealarm.cpp \
    ../qofonodeviceproximity.cpp \
    ../qofonophonecall.cpp \
    ../qofonopimdates.cpp \
    ../qofonogsmsim.cpp \
    ../qofonodeviceled.cpp \
    ../qofonodeviceidlenotifier.cpp \
    ../qofonogsmdebug.cpp \
    ../qofonogsmnetwork.cpp \
    ../qofonocalldetail.cpp \
    ../qofonosimentry.cpp \
    ../qofonogsmmonitor.cpp \
    ../qofonopimtaskquery.cpp \
    ../qofonodeviceorientation.cpp \
    ../qofonousage.cpp \
    ../qofonodeviceinput.cpp \
    ../qofonopimcallquery.cpp \
    ../qofonogsmpdp.cpp \
    ../qofonogsmmux.cpp \
    ../qofonoerror.cpp \
    ../qofonodevicedisplay.cpp \
    ../qofonogsmcall.cpp \
    ../qofonodataworld.cpp \
    ../qofonopimmessagequery.cpp \
    ../qofonolocation.cpp \
    ../qofonopimnotequery.cpp \
    ../qofonopimnotes.cpp \
    ../qofonopimtask.cpp \
    ../qofonodevicerealtimeclock.cpp \
    ../qofonogsmsms.cpp \
    ../qofonopimmessages.cpp \
    ../qofonocontextclient.cpp \
    ../qofonoworldcountry.cpp \
    ../qofonoinfo.cpp \
    ../qofonoresource.cpp \
    ../qofonogsmcb.cpp \
    ../qofononetwork.cpp \
    ../qofonomusicplayerplaylist.cpp \
    ../qofonopimfields.cpp \
    ../qofonodeviceambientlight.cpp \
    ../qofonopimdate.cpp \
    ../qofonopimtasks.cpp \
    ../qofonogsmdevice.cpp \
    ../qofononetworkprovider.cpp \
    ../qofonophone.cpp \
    ../qofonopimmessage.cpp \
    ../qofononotification.cpp \
    ../qofonoevents.cpp \
    ../qofonogsmhz.cpp \
    ../qofonopimtypes.cpp \
    ../qofonogsmerror.cpp \
    ../qofonoworldconnectivityaccesspoint.cpp \
    ../qofonopimcontactquery.cpp \
    ../qofonodeviceinfo.cpp \
    ../qofonopimcontact.cpp \
    ../qofonoapplicationsession.cpp \
    ../qofonopimdatequery.cpp \
    ../qofonogsmvoicemail.cpp \
    ../qofonodeviceaudio.cpp \
    ../qofonowakeupalarm.cpp \
    ../qofonocontextmanager.cpp \
    ../qofonopimcontacts.cpp \
    ../qofonopreferences.cpp \
    ../qofonoapplicationmanager.cpp \
    ../qofonodevicepowersupply.cpp \
    ../qofonosimmessage.cpp \
    ../qofonopimmessagefolder.cpp \
    ../qofonodevicevibrator.cpp \
    ../qofonopimcall.cpp \
    ../qofonodbusabstractinterface.cpp

HEADERS += mainwindow.h \
    ofonoutil.h \
    ../qofonoutil.h \
    ../qofonodbuspendingcall.h \
    ../qofonodbuspendingreply.h \
    ../qofonodeviceidlenotifier.h \
    ../qofonodeviceled.h \
    ../qofonosimhomezone.h \
    ../qofonogsmnetwork.h \
    ../qofonopimtasks.h \
    ../qofonopimcontactquery.h \
    ../qofonowakeupalarm.h \
    ../qofonopreferences.h \
    ../qofonodevicedisplay.h \
    ../qofonodataworld.h \
    ../qofonolocation.h \
    ../qofonosimentry.h \
    ../qofonogsmhz.h \
    ../qofonopimcalls.h \
    ../qofonocontextmanager.h \
    ../qofonogsmsms.h \
    ../qofonocontextclient.h \
    ../qofonomusicplayerplaylist.h \
    ../qofonopimcontact.h \
    ../qofonopimfields.h \
    ../qofonopimcall.h \
    ../qofonogsmvoicemail.h \
    ../qofonopimtaskquery.h \
    ../qofonogsmmonitor.h \
    ../qofonodevicepowersupply.h \
    ../qofonogsmdevice.h \
    ../qofonoevents.h \
    ../qofonogsmcb.h \
    ../qofonogsmpdp.h \
    ../qofonoresource.h \
    ../qofonogsmdebug.h \
    ../qofonoworldcountry.h \
    ../qofonoapplicationmanager.h \
    ../qofononetwork.h \
    ../qofonodevicevibrator.h \
    ../qofonopimdatequery.h \
    ../qofonodevicerealtimeclock.h \
    ../qofonotimealarm.h \
    ../qofononetworkprovider.h \
    ../qofonopimmessage.h \
    ../qofonopimtypes.h \
    ../qofonodeviceproximity.h \
    ../qofonopimnote.h \
    ../qofonopimcontacts.h \
    ../qofonopreferencesservice.h \
    ../qofonopimmessagequery.h \
    ../qofonoaudiomanager.h \
    ../qofonogsmsim.h \
    ../qofonoerror.h \
    ../qofonomusicplayer.h \
    ../qofonopimdates.h \
    ../qofonodevicepowercontrol.h \
    ../qofonousage.h \
    ../qofonodeviceambientlight.h \
    ../qofonodeviceaudio.h \
    ../qofononotification.h \
    ../qofonopimnotequery.h \
    ../qofonocalldetail.h \
    ../qofonoworldconnectivityaccesspoint.h \
    ../qofonophone.h \
    ../qofonopimdate.h \
    ../qofonopimnotes.h \
    ../qofonopimtask.h \
    ../qofonosimmessage.h \
    ../qofonopimcallquery.h \
    ../qofonogsmcall.h \
    ../qofonopimmessages.h \
    ../qofonodeviceorientation.h \
    ../qofonopimmessagefolder.h \
    ../qofonogsmerror.h \
    ../qofonoinfo.h \
    ../qofonophonecall.h \
    ../qofonodeviceinfo.h \
    ../qofonoapplicationsession.h \
    ../qofonodeviceinput.h \
    ../qofonogsmmux.h \
    ../qofonointmap.h \
    ../qofonostringmap.h \
    ../qofonovariantmaplist.h \
    ../qofonodbusabstractinterface.h

FORMS += mainwindow.ui

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qofonotest.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qofonotest
]

