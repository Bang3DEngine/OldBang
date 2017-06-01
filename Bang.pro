TEMPLATE = subdirs

SUBDIRS = \
BangLib \
BangMain \
BangPreprocessor

BangMain.depends         += BangLib
BangPreprocessor.depends += BangLib

DISTFILES += \
    BangCommon.pri
