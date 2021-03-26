include (../shared.pri)
SUBDIRS = \
    3rdparty \
    core \
    entity \
    grid \
    snap \
    operations \
    stemmer \
    spatialindex \
    gui \
    io \
    zip

!r_mobile {
    SUBDIRS += \
        customwidgets
# modify by oxh 2016/06/22
#        scripting \
#        run
}

TEMPLATE = subdirs
OTHER_FILES += api_overview.dox
