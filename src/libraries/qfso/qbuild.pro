TEMPLATE=lib
TARGET=qfso
MODULE_NAME=qfso
VERSION=1.0.0

CONFIG+=qtopia hide_symbols singleexec

QT+=dbus

DEFINES+=QFSO_LIBRARY
DEFINES+=QTMOKO

pkg [
    name=qfso
    desc="Freesmartphone.org bindings for QtMoko."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

SOURCES += \
    qfsodbuspendingcall.cpp \
    qfsopreferencesservice.cpp \
    qfsopimnote.cpp \
    qfsomusicplayer.cpp \
    qfsopimcalls.cpp \
    qfsoaudiomanager.cpp \
    qfsodevicepowercontrol.cpp \
    qfsosimhomezone.cpp \
    qfsotimealarm.cpp \
    qfsodeviceproximity.cpp \
    qfsophonecall.cpp \
    qfsopimdates.cpp \
    qfsogsmsim.cpp \
    qfsodeviceled.cpp \
    qfsodeviceidlenotifier.cpp \
    qfsogsmdebug.cpp \
    qfsogsmnetwork.cpp \
    qfsocalldetail.cpp \
    qfsosimentry.cpp \
    qfsogsmmonitor.cpp \
    qfsopimtaskquery.cpp \
    qfsodeviceorientation.cpp \
    qfsousage.cpp \
    qfsodeviceinput.cpp \
    qfsopimcallquery.cpp \
    qfsogsmpdp.cpp \
    qfsogsmmux.cpp \
    qfsoerror.cpp \
    qfsodevicedisplay.cpp \
    qfsogsmcall.cpp \
    qfsodataworld.cpp \
    qfsopimmessagequery.cpp \
    qfsolocation.cpp \
    qfsopimnotequery.cpp \
    qfsopimnotes.cpp \
    qfsopimtask.cpp \
    qfsodevicerealtimeclock.cpp \
    qfsogsmsms.cpp \
    qfsopimmessages.cpp \
    qfsocontextclient.cpp \
    qfsoworldcountry.cpp \
    qfsoinfo.cpp \
    qfsoresource.cpp \
    qfsogsmcb.cpp \
    qfsonetwork.cpp \
    qfsomusicplayerplaylist.cpp \
    qfsopimfields.cpp \
    qfsodeviceambientlight.cpp \
    qfsopimdate.cpp \
    qfsopimtasks.cpp \
    qfsogsmdevice.cpp \
    qfsonetworkprovider.cpp \
    qfsophone.cpp \
    qfsopimmessage.cpp \
    qfsonotification.cpp \
    qfsoevents.cpp \
    qfsogsmhz.cpp \
    qfsopimtypes.cpp \
    qfsogsmerror.cpp \
    qfsoworldconnectivityaccesspoint.cpp \
    qfsopimcontactquery.cpp \
    qfsodeviceinfo.cpp \
    qfsopimcontact.cpp \
    qfsoapplicationsession.cpp \
    qfsopimdatequery.cpp \
    qfsogsmvoicemail.cpp \
    qfsodeviceaudio.cpp \
    qfsowakeupalarm.cpp \
    qfsocontextmanager.cpp \
    qfsopimcontacts.cpp \
    qfsopreferences.cpp \
    qfsoapplicationmanager.cpp \
    qfsodevicepowersupply.cpp \
    qfsosimmessage.cpp \
    qfsopimmessagefolder.cpp \
    qfsodevicevibrator.cpp \
    qfsopimcall.cpp \
    qfsodbusabstractinterface.cpp

HEADERS += \
    qfsodbuspendingcall.h \
    qfsodbuspendingreply.h \
    qfsodeviceidlenotifier.h \
    qfsodeviceled.h \
    qfsosimhomezone.h \
    qfsogsmnetwork.h \
    qfsopimtasks.h \
    qfsopimcontactquery.h \
    qfsowakeupalarm.h \
    qfsopreferences.h \
    qfsodevicedisplay.h \
    qfsodataworld.h \
    qfsolocation.h \
    qfsosimentry.h \
    qfsogsmhz.h \
    qfsopimcalls.h \
    qfsocontextmanager.h \
    qfsogsmsms.h \
    qfsocontextclient.h \
    qfsomusicplayerplaylist.h \
    qfsopimcontact.h \
    qfsopimfields.h \
    qfsopimcall.h \
    qfsogsmvoicemail.h \
    qfsopimtaskquery.h \
    qfsogsmmonitor.h \
    qfsodevicepowersupply.h \
    qfsogsmdevice.h \
    qfsoevents.h \
    qfsogsmcb.h \
    qfsogsmpdp.h \
    qfsoresource.h \
    qfsogsmdebug.h \
    qfsoworldcountry.h \
    qfsoapplicationmanager.h \
    qfsonetwork.h \
    qfsodevicevibrator.h \
    qfsopimdatequery.h \
    qfsodevicerealtimeclock.h \
    qfsotimealarm.h \
    qfsonetworkprovider.h \
    qfsopimmessage.h \
    qfsopimtypes.h \
    qfsodeviceproximity.h \
    qfsopimnote.h \
    qfsopimcontacts.h \
    qfsopreferencesservice.h \
    qfsopimmessagequery.h \
    qfsoaudiomanager.h \
    qfsogsmsim.h \
    qfsoerror.h \
    qfsomusicplayer.h \
    qfsopimdates.h \
    qfsodevicepowercontrol.h \
    qfsousage.h \
    qfsodeviceambientlight.h \
    qfsodeviceaudio.h \
    qfsonotification.h \
    qfsopimnotequery.h \
    qfsocalldetail.h \
    qfsoworldconnectivityaccesspoint.h \
    qfsophone.h \
    qfsopimdate.h \
    qfsopimnotes.h \
    qfsopimtask.h \
    qfsosimmessage.h \
    qfsopimcallquery.h \
    qfsogsmcall.h \
    qfsopimmessages.h \
    qfsodeviceorientation.h \
    qfsopimmessagefolder.h \
    qfsogsmerror.h \
    qfsoinfo.h \
    qfsophonecall.h \
    qfsodeviceinfo.h \
    qfsoapplicationsession.h \
    qfsodeviceinput.h \
    qfsogsmmux.h \
    qfsointmap.h \
    qfsostringmap.h \
    qfsovariantmaplist.h \
    qfsodbusabstractinterface.h
