system(rm -f Makefile)
system(rm -f .donePreprocess)
system(cd $$PWD ; ../scripts/qmakeAll.sh Bang.pro)
system(cd $$PWD ; mkdir -p ../bin/GameDebug/obj      ../bin/GameDebug/lib)
system(cd $$PWD ; mkdir -p ../bin/GameRelease/obj    ../bin/GameRelease/lib)
system(cd $$PWD ; mkdir -p ../bin/EditorDebug/obj    ../bin/EditorDebug/lib)
system(cd $$PWD ; mkdir -p ../bin/EditorRelease/obj  ../bin/EditorRelease/lib)
system(cd $$PWD ; sh ../scripts/createIncludeHeaders.sh)

TEMPLATE = subdirs

SUBDIRS = \
BangDataStructures \
BangGraphics \
BangPreprocessor \
BangEngine \
BangEditor \
BangMain

BangPreprocessor.depends += BangDataStructures
BangGraphics.depends     += BangDataStructures
BangEditor.depends       += BangDataStructures BangPreprocessor
BangEngine.depends       += BangDataStructures BangPreprocessor BangGraphics
BangMain.depends         += BangDataStructures BangPreprocessor BangEditor BangEngine
