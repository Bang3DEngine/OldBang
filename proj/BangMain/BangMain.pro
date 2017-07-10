include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= console
QT = core gui opengl widgets

SOURCES += \
    $$BANG_ROOT/src/MainBang.cpp

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics

EDITOR {
    TARGET = $$BIN_DIR/Bang
    RESOURCES += $$BANG_ROOT/res/EngineAssets/qdarkstyle/style.qrc
    RESOURCES += $$BANG_ROOT/res/EngineAssets/QtResources.qrc
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
    $$BIN_DIR/lib/libBangEngine.a \
    $$BIN_DIR/lib/libBangEditor.a  \
    $$BIN_DIR/lib/libBangGraphics.a \
    $$BIN_DIR/lib/libBangDataStructures.a \
