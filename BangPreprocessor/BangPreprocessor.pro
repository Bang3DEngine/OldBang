include(../BangCommon.pri)

TEMPLATE = app
QT = core gui widgets

CONFIG -= app
CONFIG += console
CONFIG -= app_bundle
OBJECTS -= moc_*

TARGET = $$BIN_DIR/BangPreprocessor

PreprocessTarget.target = PreprocessTarget
PreprocessTarget.commands = sh ../scripts/preprocessHeaders.sh
QMAKE_EXTRA_TARGETS += PreprocessTarget
POST_TARGETDEPS += PreprocessTarget

SOURCES += \
    ./MainBangPreprocessor.cpp \
    ../src/Domain/BangPreprocessor/BangPreprocessor.cpp \
    ../src/Domain/BangPreprocessor/BPReflectedStruct.cpp \
    ../src/Domain/BangPreprocessor/BPReflectedVariable.cpp

HEADERS += \
    ../src/Domain/BangPreprocessor/BangPreprocessor.h \
    ../src/Domain/BangPreprocessor/BPReflectedStruct.h \
    ../src/Domain/BangPreprocessor/BPReflectedVariable.h

LIBS = -L$$BIN_DIR/lib/ -lBangDataStructures $$LIBS
PRE_TARGETDEPS += $$BIN_DIR/lib/libBangDataStructures.a
