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

LIBS = -L$$BIN_DIR/lib/ -lBang -lBangDataStructures $$LIBS
PRE_TARGETDEPS += $$BIN_DIR/lib/libBang.a $$BIN_DIR/lib/libBangDataStructures.a
