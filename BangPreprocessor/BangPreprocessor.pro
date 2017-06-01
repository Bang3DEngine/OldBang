include(../BangCommon.pri)

TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

LIBS = -lBang $$LIBS
SOURCES += ../BangPreprocessor/main.cpp
TARGET = ../bin/BangPreprocessor
