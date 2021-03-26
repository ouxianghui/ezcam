#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T08:10:51
#
#-------------------------------------------------

QT -= gui

TEMPLATE = lib

DEFINES += CA_LIBRARY

CONFIG -= warn_off
CONFIG += warn_on
CONFIG -= debug_and_release ordered embed_manifest_dll embed_manifest_exe

win32-msvc2010 {
    CONFIG += win32-msvc
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

CONFIG(debug, debug|release) {
TARGET = CAd
DESTDIR = $$PWD/../debug
} else {
TARGET = CA
DESTDIR = $$PWD/../release
} # CONFIG(debug, debug|release)


SOURCES += \
    GuillotineBinPack.cpp \
    MaxRectsBinPack.cpp \
    Rect.cpp \
    ShelfBinPack.cpp \
    SkylineBinPack.cpp \
    ShelfNextFitBinPack.cpp \
    AlgoMaxRects.cpp \
    RectanglePacker.cpp \
    CA.cpp

HEADERS +=\
    GuillotineBinPack.h \
    MaxRectsBinPack.h \
    Rect.h \
    ShelfBinPack.h \
    SkylineBinPack.h \
    ShelfNextFitBinPack.h \
    AlgoMaxRects.h \
    AlgoTypedef.h \
    RectanglePacker.h \
    CA.h \
    ca_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
