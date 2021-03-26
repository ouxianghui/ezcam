#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T21:53:57
#
#-------------------------------------------------

QT += sql
QT -= gui

TEMPLATE = lib

DEFINES += DB_LIBRARY

CONFIG -= warn_off
CONFIG += warn_on
CONFIG -= debug_and_release ordered embed_manifest_dll embed_manifest_exe

win32-msvc2010 {
    CONFIG += win32-msvc
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

CONFIG(debug, debug|release) {
TARGET = DBd
DESTDIR = $$PWD/../debug
} else {
TARGET = DB
DESTDIR = $$PWD/../release
} # CONFIG(debug, debug|release)

INCLUDEPATH += \
    $$PWD/../EG \
    $$PWD/../CA

SOURCES += \
    DatabaseManager.cpp \
    MaterialListTable.cpp \
    CuttingSolutionTable.cpp \
    SourceBoardTable.cpp \
    ProductBoardTable.cpp \
    MaterialGroupTable.cpp \
    BoardRectTable.cpp \
    DB.cpp

HEADERS +=\
    DatabaseManager.h \
    MaterialListTable.h \
    CuttingSolutionTable.h \
    SourceBoardTable.h \
    ProductBoardTable.h \
    MaterialGroupTable.h \
    BoardRectTable.h \
    DB.h \
    db_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
