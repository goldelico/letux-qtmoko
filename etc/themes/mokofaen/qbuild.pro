STRING_LANGUAGE=en_US
AVAILABLE_LANGUAGES=$$QTOPIA_AVAILABLE_LANGUAGES
LANGUAGES=$$QTOPIA_LANGUAGES
UNIFIED_NCT_LUPDATE=1

pkg [
    name=mokofaen-theme
    desc="Mokofaen theme for QtMoko."
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

conf [
    hint=themecfg
    files=mokofaen.conf
    path=/etc/themes
    trtarget=Theme-Mokofaen
]

data [
    hint=image
    files=*.xml *rc
    path=/etc/themes/mokofaen
]

pics [
    hint=pics
    files=pics/*
    path=/pics/themes/mokofaen
]

bgimage [
    hint=background
    files=pics/Abstract_r.png
    path=/pics/themes/mokofaen
]

