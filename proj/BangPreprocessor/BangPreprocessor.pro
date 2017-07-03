include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
QT = core gui widgets

CONFIG -= app
CONFIG += console
CONFIG -= app_bundle
OBJECTS -= moc_*

TARGET = $$BIN_DIR/BangPreprocessor

PreprocessTarget.target = PreprocessTargetName
PreprocessTarget.commands = (cd $$PWD/../.. ; sh ../scripts/preprocessHeaders.sh)
QMAKE_EXTRA_TARGETS += $$PreprocessTarget.target
POST_TARGETDEPS += PreprocessTargetName

SOURCES += \
    ./MainBangPreprocessor.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BangPreprocessor.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.cpp

HEADERS += \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BangPreprocessor.h \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.h \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.h

LIBS = -L$$BIN_DIR/lib/ -lBangDataStructures $$LIBS
PRE_TARGETDEPS += $$BIN_DIR/lib/libBangDataStructures.a
