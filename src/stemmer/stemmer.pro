include( ../../shared.pri )
SOURCES = \
    RStemmer.cpp
HEADERS = \
    RStemmer.h

TEMPLATE = lib
CONFIG += plugin
TARGET = $${RLIBNAME}stemmer
LIBS += -lstemmer
OTHER_FILES += stemmer.dox
DEFINES += QCADSTEMMER_LIBRARY

# modify by oxh 2016/06/22
#POST_TARGETDEPS += ../../$$ROUTDIR/$${RLIBPRE}stemmer$${RLIBPOST}
