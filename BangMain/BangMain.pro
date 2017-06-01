include(../BangCommon.pri)

TEMPLATE = app
CONFIG -= console

LIBS = -lBang $$LIBS
SOURCES += ../src/main.cpp
TARGET = ../bin/Bang
