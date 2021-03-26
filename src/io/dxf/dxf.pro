CONFIG += plugin
TARGET = dxf
include( ../../../shared.pri )

HEADERS = \
    RDxfExporter.h \
    RDxfExporterFactory.h \
    RDxfImporter.h \
    RDxfImporterFactory.h \
    RDxfPlugin.h
SOURCES = \
    RDxfExporter.cpp \
    RDxfExporterFactory.cpp \
    RDxfImporter.cpp \
    RDxfImporterFactory.cpp \
    RDxfPlugin.cpp
TEMPLATE = lib
DEFINES += QCADDXF_LIBRARY
LIBS += -l$${RLIBNAME}core -l$${RLIBNAME}entity -ldxflib -l$${RLIBNAME}operations
r_static_libs {
    CONFIG -= plugin
    CONFIG += staticlib
}
else {
# modify by oxh 2016/06/22
#    DESTDIR = ../../../plugins
    CONFIG += plugin
}

# modify by oxh 2016/06/22
#POST_TARGETDEPS += ../../../$$ROUTDIR/$${RLIBPRE}dxflib$${RLIBPOST}
