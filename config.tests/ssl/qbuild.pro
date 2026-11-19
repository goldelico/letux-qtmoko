TEMPLATE=app
CONFIG+=embedded
TARGET=ssl
SOURCES+=sslcheck.cpp
LIBS+=-lssl -lcrypto
