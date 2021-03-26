#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T22:43:53
#
#-------------------------------------------------
include($$PWD/../shared_app.pri)

QT += printsupport axcontainer xml

TEMPLATE = lib

DEFINES += EG_LIBRARY

CONFIG -= warn_off
CONFIG += warn_on
CONFIG -= debug_and_release ordered embed_manifest_dll embed_manifest_exe

win32-msvc2010 {
    CONFIG += win32-msvc
    DEFINES += _CRT_SECURE_NO_WARNINGS
}


CONFIG(debug, debug|release) {
TARGET = EGd
DESTDIR = $$PWD/../debug/

LIBS += $$PWD/../debug/CAd.lib
LIBS += $$PWD/../debug/DBd.lib
LIBS += $$PWD/../debug/QRd.lib
LIBS += $$PWD/../debug/qcaddxf_debug.lib
LIBS += $$PWD/../debug/qcadgui.lib
LIBS += $$PWD/../debug/qcadentity.lib
LIBS += $$PWD/../debug/qcadoperations.lib
LIBS += $$PWD/../debug/qcadspatialindex.lib
LIBS += $$PWD/../debug/spatialindexnavel.lib
LIBS += $$PWD/../debug/qcadgrid.lib
LIBS += $$PWD/../debug/qcadsnap.lib
LIBS += $$PWD/../debug/qcadcore.lib
LIBS += $$PWD/../debug/qcadstemmer.lib
LIBS += $$PWD/../debug/stemmer.lib
LIBS += $$PWD/../debug/qcadzip.lib
LIBS += $$PWD/../debug/quazip.lib

} else {
TARGET = EG
DESTDIR = $$PWD/../release

LIBS += $$PWD/../release/CA.lib
LIBS += $$PWD/../release/DB.lib
LIBS += $$PWD/../release/QR.lib
LIBS += $$PWD/../release/qcaddxf.lib
LIBS += $$PWD/../release/qcadgui.lib
LIBS += $$PWD/../release/qcadentity.lib
LIBS += $$PWD/../release/qcadoperations.lib
LIBS += $$PWD/../release/qcadspatialindex.lib
LIBS += $$PWD/../release/spatialindexnavel.lib
LIBS += $$PWD/../release/qcadgrid.lib
LIBS += $$PWD/../release/qcadsnap.lib
LIBS += $$PWD/../release/qcadcore.lib
LIBS += $$PWD/../release/qcadstemmer.lib
LIBS += $$PWD/../release/stemmer.lib
LIBS += $$PWD/../release/qcadzip.lib
LIBS += $$PWD/../release/quazip.lib

} # CONFIG(debug, debug|release)

!r_no_opennurbs {
LIBS += $$PWD/../debug/opennurbs.lib
LIBS += $$PWD/../debug/zlib.lib
}

win32 {
    LIBS += \
        -lRpcrt4 \
        "kernel32.lib" \
        "user32.lib" \
        "gdi32.lib" \
        "winspool.lib" \
        "comdlg32.lib" \
        "advapi32.lib" \
        "shell32.lib" \
        "ole32.lib" \
        "oleaut32.lib" \
        "uuid.lib" \
        "odbc32.lib" \
        "odbccp32.lib"
}

INCLUDEPATH += \
    $$PWD/../src/core \
    $$PWD/../src/core/math \
    $$PWD/../src/grid \
    $$PWD/../src/snap \
    $$PWD/../src/gui \
    $$PWD/../src/entity \
    $$PWD/../src/operations \
    $$PWD/../src/scripting \
    $$PWD/../src/stemmer \
    $$PWD/../src/io/dxf \
    $$PWD/../src/spatialindex \
    $$PWD/../src/3rdparty \
    $$PWD/../src/zip \
    $$PWD/../CA \
    $$PWD/../DB \
    $$PWD/../QR

SOURCES += \
    ExcelDataLoader.cpp \
    ExcelEngine.cpp \
    FormPrinter.cpp \
    MaterialsListLoader01.cpp \
    SystemConfigManager.cpp \
    ProjectConfigManager.cpp \
    QExcel.cpp \
    DissimilarLibraryOperator.cpp \
    DissimilarParametersReader.cpp \
    DissimilarParametersWriter.cpp \
    LabelDrawer.cpp \
    LabelPrinter.cpp \
    CNCGenerator.cpp \
    PolylineEquidistant.cpp \
    emu_c99.cpp \
    CNCBatchExporter.cpp \
    CuttingPointSearcher.cpp \
    LabelPrinterInstance.cpp \
    tinyxml2.cpp \
    EG.cpp \
    SegmentsCreator.cpp

HEADERS +=\
    DataSt.h \
    ExcelDataLoader.h \
    ExcelEngine.h \
    FormPrinter.h \
    MaterialsListLoader01.h \
    SystemConfigManager.h \
    ProjectConfigManager.h \
    QExcel.h \
    DissimilarLibraryOperator.h \
    DissimilarParametersReader.h \
    DissimilarParametersWriter.h \
    Typedef.h \
    LabelDrawer.h \
    LabelPrinter.h \
    CNCGenerator.h \
    PolylineEquidistant.h \
    emu_c99.h \
    CNCBatchExporter.h \
    CuttingPointSearcher.h \
    LabelPrinterInstance.h \
    tinyxml2.h \
    eg_global.h \
    EG.h \
    SegmentsCreator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
