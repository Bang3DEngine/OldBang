include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = app
CONFIG -= app app_bundle
CONFIG += console
OBJECTS -= moc_*

TARGET = $$BIN_DIR/BangPreprocessor

PreprocessTarget.target = PreprocessTargetName
PreprocessTarget.commands = (cd $$BANG_ROOT ; sh ../scripts/preprocessHeaders.sh)
QMAKE_EXTRA_TARGETS += $$PreprocessTarget.target
POST_TARGETDEPS += PreprocessTargetName

LIBS = \
     -L$$BIN_DIR/lib/ \
    -lBangCore \
    $$LIBS

PRE_TARGETDEPS += \
    $$BIN_DIR/lib/libBangCore.a

SOURCES += \
    ./MainBangPreprocessor.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BangPreprocessor.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.cpp

HEADERS += \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BangPreprocessor.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.h
