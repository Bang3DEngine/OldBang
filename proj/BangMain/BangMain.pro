include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= console
INCLUDEPATH += $$PWD/../../include
QT = core gui opengl widgets

SOURCES += \
    $$PWD/../../src/MainBang.cpp

EDITOR {
    TARGET = $$BIN_DIR/Bang
    RESOURCES += $$PWD/../../res/EngineAssets/qdarkstyle/style.qrc
    RESOURCES += $$PWD/../../res/EngineAssets/QtResources.qrc
}

GAME {
    TARGET = $$BIN_DIR/Game.exe
}

CONFIG += no_lflags_merge
LIBS = -L$$BIN_DIR/lib/ \
    -lBangEditor \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    -lBangEditor \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    $$LIBS

PRE_TARGETDEPS += \
    $$BIN_DIR/lib/libBangEngine.so \
    $$BIN_DIR/lib/libBangEditor.so  \
    $$BIN_DIR/lib/libBangGraphics.so \
    $$BIN_DIR/lib/libBangDataStructures.so \
