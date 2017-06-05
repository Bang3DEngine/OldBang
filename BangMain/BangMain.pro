include(../BangCommon.pri)

TEMPLATE = app
CONFIG -= console
INCLUDEPATH += ../include
QT = core gui opengl widgets

SOURCES += \
    ../src/MainBang.cpp

EDITOR {
    TARGET = $$BIN_DIR/Bang
    RESOURCES += ../EngineAssets/qdarkstyle/style.qrc
    RESOURCES += ../EngineAssets/QtResources.qrc
}

GAME {
    TARGET = $$BIN_DIR/Game.exe
}

LIBS = -L$$BIN_DIR/lib/ -lBang -lBangDataStructures $$LIBS
PRE_TARGETDEPS += $$BIN_DIR/lib/libBang.a $$BIN_DIR/lib/libBangDataStructures.a
