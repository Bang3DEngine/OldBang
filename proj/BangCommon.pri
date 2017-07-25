TEMPLATE = lib
QT_VERSION = 5

# PARAMETERS CATCHING
COMPILER=$$(BANG_BUILD_COMPILER)
isEmpty(COMPILER) {
  COMPILER = g++
}

RELEASE_OR_DEBUG=$$(BANG_BUILD_RELEASE_OR_DEBUG) # RELEASE or DEBUG
isEmpty(RELEASE_OR_DEBUG) {
  RELEASE_OR_DEBUG = RELEASE
}
#####################

QMAKE_CXX  = $$COMPILER
QMAKE_LINK = $$COMPILER
CONFIG += $$RELEASE_OR_DEBUG
CONFIG += c++11
CONFIG += qt

LIBS += \
    -lGLEW            \ # GLEW
    -lGLU             \
    -ldl              \ # To load libraries and stuff
    -lalut -lopenal   \ # Alut and OpenAL
    -lassimp          \
    -lfreetype

BANG_ROOT=$$PWD/..

INCLUDEPATH +=                               \
    /usr/include                             \
    $$BANG_ROOT/include                         \
    $$BANG_ROOT/include/Bang                    \
    $$BANG_ROOT/include/Bang/BangDataStructures \
    $$BANG_ROOT/include/Bang/BangPreprocessor   \
    $$BANG_ROOT/include/Bang/BangEngine         \
    $$BANG_ROOT/include/freetype

BIN_DIR=""
DEBUG   { BIN_DIR = $$BANG_ROOT/bin/Debug   }
RELEASE { BIN_DIR = $$BANG_ROOT/bin/Release }

OBJECTS_DIR = $$BIN_DIR/obj

# G++ options (release, debug, etc.) ############
DEBUG {
    OPTIMIZATION_OPT = -O0 -Wl,-O0
    DEBUG_OPT = -g
}
RELEASE {
    OPTIMIZATION_OPT = -O3 -Wl,-O3
}

#OTHER_OPTS = -Wall --std=c++14 -Wl,--export-dynamic -Wno-unused-parameter \
             #-Wunused-variable -Wswitch -Wno-sign-compare -fPIC
OTHER_OPTS = --std=c++14 \
             -Wall \
             -Wint-to-pointer-cast \
             -Wno-unused-parameter \
             -Wno-sign-compare \
             -Wswitch \
             -Wl,--export-dynamic  \
             -fPIC
ALL_OPTS = -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CFLAGS           -= $$ALL_OPTS
QMAKE_CFLAGS_DEBUG     -= $$ALL_OPTS
QMAKE_CFLAGS_RELEASE   -= $$ALL_OPTS
QMAKE_CXXFLAGS         -= $$ALL_OPTS
QMAKE_CXXFLAGS_DEBUG   -= $$ALL_OPTS
QMAKE_CXXFLAGS_RELEASE -= $$ALL_OPTS
QMAKE_LFLAGS           -= $$ALL_OPTS
QMAKE_LFLAGS_DEBUG     -= $$ALL_OPTS
QMAKE_LFLAGS_RELEASE   -= $$ALL_OPTS

# Fix for "fatal error: stdlib.h: No such file or directory"
QMAKE_CFLAGS_ISYSTEM    =

QMAKE_LFLAGS           += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT
QMAKE_CXXFLAGS         += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT
###################################################
