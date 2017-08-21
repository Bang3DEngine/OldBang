include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= console

SOURCES += \
    $$BANG_ROOT/src/Engine/GameBuilder/MainGameBuilder.cpp

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics \
    $$BANG_ROOT/include/Bang/BangEngine

TARGET = $$BIN_DIR/BangGameBuilder

CONFIG += no_lflags_merge
LIBS = -L$$BIN_DIR/lib/ \
    -lBangEngine \
    -lBangGraphics \
    -lBangCore \
    -lBangEngine \
    -lBangGraphics \
    -lBangCore \
    -lSDL2 \
    $$LIBS


PRE_TARGETDEPS += \
    $$BIN_DIR/lib/libBangEngine.a \
    $$BIN_DIR/lib/libBangGraphics.a \
    $$BIN_DIR/lib/libBangCore.a
