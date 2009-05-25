qtopia_project(qtopia plugin)
TARGET=htcuniversalaudiohardware

HEADERS		=  htcuniversalaudioplugin.h
SOURCES	        =  htcuniversalaudioplugin.cpp

depends(libraries/qtopiaaudio)
enable_bluetooth {
    depends(libraries/qtopiacomm)
}
