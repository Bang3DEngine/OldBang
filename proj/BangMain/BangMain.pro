include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= console
QT = core gui opengl widgets

GAME {
SOURCES += \
    $$BANG_ROOT/src/GameMain.cpp
}

EDITOR {
SOURCES += \
    $$BANG_ROOT/src/EditorMain.cpp
}

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics \
    $$BANG_ROOT/include/Bang/BangEngine

EDITOR  {
    INCLUDEPATH += $$BANG_ROOT/include/Bang/BangEditor
}

EDITOR {
    TARGET = $$BIN_DIR/Bang
    RESOURCES += $$BANG_ROOT/res/EngineAssets/qdarkstyle/style.qrc
    RESOURCES += $$BANG_ROOT/res/EngineAssets/QtResources.qrc
}

GAME {
    TARGET = $$BIN_DIR/Game.exe
}

EDITOR {
    EDITOR_LIB = -lBangEditor
}

CONFIG += no_lflags_merge
LIBS = -L$$BIN_DIR/lib/ \
    $$EDITOR_LIB \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    $$EDITOR_LIB \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    $$LIBS


PRE_TARGETDEPS += \
    $$BIN_DIR/lib/libBangEngine.a \
    $$BIN_DIR/lib/libBangGraphics.a \
    $$BIN_DIR/lib/libBangDataStructures.a

EDITOR {
    PRE_TARGETDEPS += $$BIN_DIR/lib/libBangEditor.a
}
