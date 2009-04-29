TEMPLATE=app
TARGET=sipagent

CONFIG+=qtopia singleexec
QTOPIA*=phone audio comm collective pim
MODULES*=dissipate2 gsm g711

pkg [
    name=sipagent
    desc="SIP agent for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

HEADERS=\
    siplayer.h\
    sdpparser.h\
    mediasessionservice.h\
    mediasessionclient.h\
    sipagentmedia.h\
    mediastream.h\
    rtpsinkstream.h\
    rtpsourcestream.h\
    commonencodestream.h\
    gsmencodestream.h\
    pcmuencodestream.h\
    pcmaencodestream.h\
    commondecodestream.h\
    gsmdecodestream.h\
    pcmudecodestream.h\
    pcmadecodestream.h\
    audioinputstream.h\
    audiooutputstream.h\
    dtmfstream.h\
    wavoutputstream.h\
#    sippresenceprovider.h\
    telephonyservice.h\
    siptelephonyservice.h

SOURCES=\
    main.cpp\
    siplayer.cpp\
    sdpparser.cpp\
    mediasessionservice.cpp\
    mediasessionclient.cpp\
    sipagentmedia.cpp\
    mediastream.cpp\
    rtpsinkstream.cpp\
    rtpsourcestream.cpp\
    commonencodestream.cpp\
    gsmencodestream.cpp\
    pcmuencodestream.cpp\
    pcmaencodestream.cpp\
    commondecodestream.cpp\
    gsmdecodestream.cpp\
    pcmudecodestream.cpp\
    pcmadecodestream.cpp\
    audioinputstream.cpp\
    audiooutputstream.cpp\
    dtmfstream.cpp\
    wavoutputstream.cpp\
 #   sippresenceprovider.cpp\
    telephonyservice.cpp\
    siptelephonyservice.cpp

# Install rules

target [
    hint=sxe
    domain=trusted
]

telephonyservice [
    files=services/Telephony/sipagent
    path=/services/Telephony
]

mediaservice [
    files=services/MediaSession/sipagent
    path=/services/MediaSession
]

