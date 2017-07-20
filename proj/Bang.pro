include(./BangCommon.pri)
system(rm -f Makefile)
system(cd $$PWD ; ../scripts/qmakeAll.sh Bang.pro)
system(cd $$PWD ; mkdir -p ../bin/Debug/obj   ../bin/Debug/lib)
system(cd $$PWD ; mkdir -p ../bin/Release/obj ../bin/Release/lib)

TEMPLATE = subdirs

DISTFILES += \
$$PWD/../res/.astylerc

SUBDIRS = \
BangDataStructures \
BangGraphics \
BangPreprocessor \
BangEngine \
BangMainGameBuilder \
BangMainGame \
BangMain

BangPreprocessor.depends    += BangDataStructures
BangGraphics.depends        += BangDataStructures
BangEngine.depends          += BangDataStructures BangPreprocessor BangGraphics
BangMain.depends            += BangDataStructures BangPreprocessor BangGraphics BangEngine
BangMainGameBuilder.depends += BangDataStructures BangPreprocessor BangGraphics BangEngine
BangMainGame.depends        += BangDataStructures BangPreprocessor BangGraphics BangEngine
