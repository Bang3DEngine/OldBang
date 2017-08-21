include(./BangCommon.pri)
system(rm -f Makefile)
system(cd $$PWD ; ../scripts/qmakeAll.sh Bang.pro)
system(cd $$PWD ; mkdir -p ../bin/Debug/obj   ../bin/Debug/lib)
system(cd $$PWD ; mkdir -p ../bin/Release/obj ../bin/Release/lib)

TEMPLATE = subdirs

DISTFILES += \
$$PWD/../res/.astylerc

SUBDIRS = \
BangCore \
BangGraphics \
BangPreprocessor \
BangEngine \
BangMainGameBuilder \
BangMainGame \
BangMain

BangPreprocessor.depends    += BangCore
BangGraphics.depends        += BangCore
BangEngine.depends          += BangCore BangPreprocessor BangGraphics
BangMain.depends            += BangCore BangPreprocessor BangGraphics BangEngine
BangMainGameBuilder.depends += BangCore BangPreprocessor BangGraphics BangEngine
BangMainGame.depends        += BangCore BangPreprocessor BangGraphics BangEngine
