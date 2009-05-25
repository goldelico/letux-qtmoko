TARGET=qnetwalk
CONFIG+=qtopia quicklaunch 

# Input
HEADERS=cell.h mainwindow.h
SOURCES=cell.cpp main.cpp mainwindow.cpp
RESOURCES=pics/pics.qrc
TRANSLATIONS+=translations/qnetwalk_de.ts \
                translations/qnetwalk_es.ts \
                translations/qnetwalk_fr.ts \
                translations/qnetwalk_it.ts \
                translations/qnetwalk_nl.ts \
                translations/qnetwalk_pl.ts \
                translations/qnetwalk_pt_BR.ts \
                translations/qnetwalk_ru.ts
DEFINES+=QTOPIA_PHONE

pkg [
    name=qnetwalk
    desc="QNetwalk"
    version=$$QTOPIA_VERSION
    license=$$QTOPIA_LICENSE
    maintainer=$$QTOPIA_MAINTAINER
]

target [
    hint=sxe
    domain=trusted
]

desktop [
    hint=desktop
    files=qnetwalk.desktop
    path=/apps/Games
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qnetwalk
]

sounds [
    hint=image
    files=sounds/*
    path=/sounds/qnetwalk
]


