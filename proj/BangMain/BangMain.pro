include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= console
QT = core gui opengl widgets

SOURCES += \
    $$BANG_ROOT/src/Main.cpp

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics \
    $$BANG_ROOT/include/Bang/BangEngine

TARGET = $$BIN_DIR/Bang

CONFIG += no_lflags_merge
LIBS = -L$$BIN_DIR/lib/ \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    -lBangEngine \
    -lBangGraphics \
    -lBangDataStructures \
    -lSDL2 \
    $$LIBS


PRE_TARGETDEPS += \
    $$BIN_DIR/lib/libBangEngine.a \
    $$BIN_DIR/lib/libBangGraphics.a \
    $$BIN_DIR/lib/libBangDataStructures.a
