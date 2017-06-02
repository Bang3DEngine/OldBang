TEMPLATE = lib
QT_VERSION = 5

# PARAMETERS CATCHING
BUILD_MODE=$$BUILD_MODE # EDITOR or GAME
isEmpty(BUILD_MODE) {
  BUILD_MODE=EDITOR
}

RELEASE_OR_DEBUG=$$RELEASE_OR_DEBUG # RELEASE or DEBUG
isEmpty(RELEASE_OR_DEBUG) {
  RELEASE_OR_DEBUG=RELEASE_MODE
}
#####################


# QMake config #############
CONFIG += $$BUILD_MODE $$RELEASE_OR_DEBUG
CONFIG += qt
####################################

# LIBS ###########################
LIBS += \
    -lGLEW            \ # GLEW
    -lGLU             \
    -ldl              \ # To load libraries and stuff
    -lalut -lopenal   \ # Alut and OpenAL
    -lassimp          \
    -lfreetype
########################################

# INCLUDE PATH ###################
INCLUDEPATH +=                       \
    /usr/include                     \
    ../include                       \
    ../include/freetype
######################################

BIN_DIR=""
EDITOR {
    DEBUG_MODE   { BIN_DIR = ../bin/EditorDebug   }
    RELEASE_MODE { BIN_DIR = ../bin/EditorRelease }
}
GAME {
    DEBUG_MODE   { BIN_DIR = ../bin/GameDebug   }
    RELEASE_MODE { BIN_DIR = ../bin/GameRelease }
}
OBJECTS_DIR = $$BIN_DIR/obj


# G++ options (release, debug, etc.) ############
DEBUG_MODE {
    OPTIMIZATION_OPT = -O0 -Wl,-O0
    DEBUG_OPT = -g
}
RELEASE_MODE {
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
