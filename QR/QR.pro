#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T13:13:03
#
#-------------------------------------------------

QT       -= gui
QT       += core widgets

TEMPLATE = lib

DEFINES += GQR_LIBRARY

DEFINES += HAVE_CONFIG_H

CONFIG(debug, debug|release) {
TARGET = QRd
DESTDIR = $$PWD/../debug
} else {
TARGET = QR
DESTDIR = $$PWD/../release
} # CONFIG(debug, debug|release)

SOURCES += \
    bitstream.c \
    mask.c \
    mmask.c \
    mqrspec.c \
    qrencode.c \
    qrinput.c \
    qrspec.c \
    rscode.c \
    split.c \
    QR.cpp

HEADERS +=\
    bitstream.h \
    mask.h \
    mmask.h \
    mqrspec.h \
    qrencode.h \
    qrencode_inner.h \
    qrinput.h \
    qrspec.h \
    rscode.h \
    split.h \
    config.h \
    common.h \
    qr_global.h \
    QR.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
