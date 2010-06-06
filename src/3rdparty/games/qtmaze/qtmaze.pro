TEMPLATE = app
TARGET = qtmaze

QT += core gui

FORMS=\
    formbase.ui

HEADERS=\
    form.h\
    renderarea.h\
    accelerometers.h\
    types.h\
    json.h\
    paramsloader.h\
    vibro.h

SOURCES=\
    main.cpp\
    form.cpp\
    renderarea.cpp\
    accelerometers.cpp\
    json.c\
    paramsloader.cpp\
    vibro.cpp

