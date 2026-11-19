MODULE_NAME=sqlite
LICENSE=FREEWARE

system_qt {
    # Use Debian's libsqlite3 instead of the copy in Qt's source tree.
    DEP.libs [
        TYPE=DEPENDS PERSISTED SDK
        EVAL="LIBS+=-lsqlite3"
    ]
} else {
    TEMPLATE=lib
    CONFIG+=embedded singleexec
    TARGET=qtopia-sqlite

    # Technically, this depends on Qt but that would create a circular dependency
    # All this needs is to include qconfig.h so just set INCLUDEPATH
    # FIXME this should open the Qt project (but that could lead to a race condition)
    INCLUDEPATH+=$$QTEDIR/include/QtCore

    release:DEFINES*=NDEBUG

    SOURCEPATH=/qtopiacore/qt/src/3rdparty/sqlite

    build_qtopia_sqlite {
        DEFINES+=SQLITE_OMIT_LOAD_EXTENSION SQLITE_CORE
    #    QMAKE_CFLAGS+=-O3
    #    QMAKE_CXXFLAGS+=-O3

        HEADERS=sqlite3.h
        SOURCES=sqlite3.c

        MODULES*=pthread
    }
}
