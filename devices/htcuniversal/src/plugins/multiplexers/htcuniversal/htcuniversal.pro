qtopia_project(qtopia plugin)
TARGET=htcuniversalmultiplex

CONFIG+=no_tr

HEADERS		=  htcuniversalmultiplexer.h
SOURCES	        =  htcuniversalmultiplexer.cpp

requires(enable_modem)
depends(libraries/qtopiacomm/serial)
