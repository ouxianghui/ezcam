#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T20:47:52
#
#-------------------------------------------------

include($$PWD/../shared_app.pri)

QT += core gui sql script svg printsupport axcontainer xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TRANSLATIONS += App_en_US.ts

CONFIG -= warn_off
CONFIG += warn_on
CONFIG -= debug_and_release ordered embed_manifest_dll embed_manifest_exe

win32-msvc2010 {
    CONFIG += win32-msvc
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

CONFIG(debug, debug|release) {
TARGET = EZCamd
DESTDIR = $$PWD/../debug/

LIBS += $$PWD/../debug/CAd.lib
LIBS += $$PWD/../debug/EGd.lib
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
LIBS += $$PWD/../debug/DicPlus32.lib
LIBS += $$PWD/../debug/Dongle_d.lib

} else {
TARGET = EZCam
DESTDIR = $$PWD/../release/

LIBS += $$PWD/../release/CA.lib
LIBS += $$PWD/../release/EG.lib
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
LIBS += $$PWD/../release/DicPlus32.lib
LIBS += $$PWD/../release/Dongle_d.lib

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
    $$PWD/../EG \
    $$PWD/../DB \
    $$PWD/../QR

HEADERS += \
    MainWindow.h \
    ViewPort.h \
    WorkspaceTreeWidget.h \
    DialogNewProject.h \
    DialogSystemSettings.h \
    XbelReader.h \
    XbelWriter.h \
    Global.h \
    MaterialTableWidget.h \
    SourceBoardListWidget.h \
    CentralWidget.h \
    SolutionSummaryTableWidget.h \
    GraphicsViewer.h \
    NCExporter.h \
    DialogDissimilarShapes.h \
    DissimilarPreviewer.h \
    BitmapExportDialog.h \
    DialogPrintBoards.h \
    DialogPrintPreviewer.h \
    DialogPrintSettings.h \
    ImageDrawer.h \
    QProgressHud.h \
    DialogStatusIndicator.h \
    R6PlusOperator.h \
    DialogAbout.h \
    RockeyArmOperator.h \
    Dongle_API.h \
    DialogCustomDissimilarShape.h \
    ShapeViewer.h \
    ShapeEditorTableWidget.h \
    EZDefaultAction.h \
    EZSelectEntitiesAction.h \
    EZResetAction.h \
    EZReplaceDocumentAction.h \
    EZPrintLabelAction.h \
    EZNavigationAction.h \
    EZModifyCuttingPointAction.h \
    EZExportNCAction.h \
    EZExportBitmapAction.h \
    EZEventHandler.h \
    EZEditNCOrderAction.h \
    EZDocumentSaveAsAction.h \
    EZDocumentSaveAction.h \
    HoleEditorTableWidget.h \
    DrillEditorTableWidget.h \
    DrillEditorDiameterColumnDelegate.h \
    ShapeEditorFeatureColumnDelegate.h \
    ShapeEditorSolutionColumnDelegate.h

SOURCES += main.cpp \
    MainWindow.cpp \
    ViewPort.cpp \
    WorkspaceTreeWidget.cpp \
    DialogNewProject.cpp \
    DialogSystemSettings.cpp \
    XbelReader.cpp \
    XbelWriter.cpp \
    MaterialTableWidget.cpp \
    SourceBoardListWidget.cpp \
    CentralWidget.cpp \
    SolutionSummaryTableWidget.cpp \
    GraphicsViewer.cpp \
    NCExporter.cpp \
    DialogDissimilarShapes.cpp \
    DissimilarPreviewer.cpp \
    BitmapExportDialog.cpp \
    DialogPrintBoards.cpp \
    DialogPrintPreviewer.cpp \
    DialogPrintSettings.cpp \
    ImageDrawer.cpp \
    QProgressHud.cpp \
    DialogStatusIndicator.cpp \
    R6PlusOperator.cpp \
    DialogAbout.cpp \
    RockeyArmOperator.cpp \
    DialogCustomDissimilarShape.cpp \
    ShapeViewer.cpp \
    ShapeEditorTableWidget.cpp \
    EZDefaultAction.cpp \
    EZSelectEntitiesAction.cpp \
    EZResetAction.cpp \
    EZReplaceDocumentAction.cpp \
    EZPrintLabelAction.cpp \
    EZNavigationAction.cpp \
    EZModifyCuttingPointAction.cpp \
    EZExportNCAction.cpp \
    EZExportBitmapAction.cpp \
    EZEventHandler.cpp \
    EZEditNCOrderAction.cpp \
    EZDocumentSaveAsAction.cpp \
    EZDocumentSaveAction.cpp \
    HoleEditorTableWidget.cpp \
    DrillEditorTableWidget.cpp \
    DrillEditorDiameterColumnDelegate.cpp \
    ShapeEditorFeatureColumnDelegate.cpp \
    ShapeEditorSolutionColumnDelegate.cpp

FORMS += \
    MainWindow.ui \
    ViewPort.ui \
    DialogNewProject.ui \
    DialogSystemSettings.ui \
    DialogDissimilarShapes.ui \
    DissimilarPreviewer.ui \
    BitmapExportDialog.ui \
    DialogPrintBoards.ui \
    DialogPrintPreviewer.ui \
    DialogPrintSettings.ui \
    ImageDrawer.ui \
    QProgressHud.ui \
    DialogStatusIndicator.ui \
    DialogAbout.ui \
    DialogCustomDissimilarShape.ui \
    ShapeViewer.ui

RESOURCES += \
    images.qrc

RC_FILE = appicon.rc

DISTFILES +=
