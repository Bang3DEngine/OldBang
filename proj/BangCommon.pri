TEMPLATE = lib
QT_VERSION = 5

# PARAMETERS CATCHING
COMPILER=$$(BANG_BUILD_COMPILER) # EDITOR or GAME
isEmpty(COMPILER) {
  COMPILER=g++
}

EDITOR_OR_GAME=$$(BANG_BUILD_EDITOR_OR_GAME) # EDITOR or GAME
isEmpty(EDITOR_OR_GAME) {
  EDITOR_OR_GAME=EDITOR
}

RELEASE_OR_DEBUG=$$(BANG_BUILD_RELEASE_OR_DEBUG) # RELEASE or DEBUG
isEmpty(RELEASE_OR_DEBUG) {
  RELEASE_OR_DEBUG=RELEASE
}
#####################

QMAKE_CXX  = $$COMPILER
QMAKE_LINK = $$COMPILER
CONFIG += $$EDITOR_OR_GAME $$RELEASE_OR_DEBUG
CONFIG += qt

LIBS += \
    -lGLEW            \ # GLEW
    -lGLU             \
    -ldl              \ # To load libraries and stuff
    -lalut -lopenal   \ # Alut and OpenAL
    -lassimp          \
    -lfreetype

INCLUDEPATH +=                       \
    /usr/include                     \
    $$PWD/../include              \
    $$PWD/../include/freetype

BIN_DIR=""
EDITOR {
    DEBUG   { BIN_DIR = $$PWD/../bin/EditorDebug   }
    RELEASE { BIN_DIR = $$PWD/../bin/EditorRelease }
}
GAME {
    DEBUG   { BIN_DIR = $$PWD/../bin/GameDebug   }
    RELEASE { BIN_DIR = $$PWD/../bin/GameRelease }
}
OBJECTS_DIR = $$BIN_DIR/obj

# G++ options (release, debug, etc.) ############
DEBUG {
    OPTIMIZATION_OPT = -O0 -Wl,-O0
    DEBUG_OPT = -g
}
RELEASE {
    OPTIMIZATION_OPT = -O3 -Wl,-O3
}

EDITOR {
    DEFINES += BANG_EDITOR
}

#OTHER_OPTS = -Wall --std=c++14 -Wl,--export-dynamic -Wno-unused-parameter \
             #-Wunused-variable -Wswitch -Wno-sign-compare -fPIC
OTHER_OPTS = -Wall --std=c++14 -Wl,--export-dynamic -Wno-unused-parameter \
             -Wno-sign-compare -Wswitch -fPIC
QMAKE_CFLAGS           -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CFLAGS_DEBUG     -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CFLAGS_RELEASE   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS         -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS_DEBUG   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS_RELEASE -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS           -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS_DEBUG     -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS_RELEASE   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS           += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT
QMAKE_CXXFLAGS         += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT
###################################################
