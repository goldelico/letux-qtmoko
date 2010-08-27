STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES
UNIFIED_NCT_LUPDATE=1

pkg [
    name=finxi-theme
    desc="Finximod theme for Qt Extended."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

conf [
    hint=themecfg
    files=finximod.conf
    path=/etc/themes
    trtarget=Theme-Finxi
]

data [
    hint=image
    files=*.xml *rc
    path=/etc/themes/finximod
]

pics [
    hint=pics
    files=pics/*
    path=/pics/themes/finximod
]

bgimage [
    hint=background
    files=background.png
    path=/pics/themes/finximod
]

