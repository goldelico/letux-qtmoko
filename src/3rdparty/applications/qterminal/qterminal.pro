#-------------------------------------------------
#
# Project created by QtCreator 2009-04-14T22:32:40
#
#-------------------------------------------------

TARGET = qterminal
TEMPLATE = app

QT          += core gui

DEFINES+=HAVE_OPENPTY

LIBS+=-lutil

HEADERS+=BlockArray.h\
    Character.h\
    CharacterColor.h\
    ColorScheme.h\
    DefaultTranslatorText.h\
    Emulation.h\
    Filter.h\
    History.h\
    io_layer.h\
    KeyboardTranslator.h\
    LineFont.h\
    MainWindow.h\
    MyPty.h\
    Preferences.h\
    procctl.h\
    profile.h\
    Screen.h\
    ScreenWindow.h\
    Session.h\
    ShellCommand.h\
    TerminalCharacterDecoder.h\
    TerminalDisplay.h\
    Vt102Emulation.h

SOURCES+=BlockArray.cpp\
    ColorScheme.cpp\
    Emulation.cpp\
    Filter.cpp\
    History.cpp\
    io_layer.cpp\
    KeyboardTranslator.cpp\
    main.cpp\
    MainWindow.cpp\
    MyPty.cpp\
    Preferences.cpp\
    procctl.cpp\
    profile.cpp\
    Screen.cpp\
    ScreenWindow.cpp\
    Session.cpp\
    ShellCommand.cpp\
    TerminalCharacterDecoder.cpp\
    TerminalDisplay.cpp\
    Vt102Emulation.cpp
