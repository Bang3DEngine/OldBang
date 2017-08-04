include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
QT = core gui widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangDataStructures

SOURCES += \
    $$BANG_ROOT/src/Engine/DataStructures/String.cpp \
    $$BANG_ROOT/src/Engine/Math/Color.cpp \
    $$BANG_ROOT/src/Engine/Math/Math.cpp \
    $$BANG_ROOT/src/Engine/Math/Random.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/Path.cpp \
    $$BANG_ROOT/src/Engine/Debug/StreamOperators.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/File.cpp

HEADERS += \
    $$BANG_ROOT/src/Engine/Debug/StreamOperators.h \
    $$BANG_ROOT/src/Engine/Math/Math.h \
    $$BANG_ROOT/src/Engine/Math/Random.h \
    $$BANG_ROOT/src/Engine/IO/Files/Path.h \
    $$BANG_ROOT/src/Engine/DataStructures/Collection.h \
    $$BANG_ROOT/src/Engine/Math/Color.h \
    $$BANG_ROOT/src/Engine/Math/Rect.h \
    $$BANG_ROOT/src/Engine/Math/Vector3.h \
    $$BANG_ROOT/src/Engine/Math/Vector4.h \
    $$BANG_ROOT/src/Engine/Math/Vector2.h \
    $$BANG_ROOT/src/Engine/DataStructures/Set.h \
    $$BANG_ROOT/src/Engine/DataStructures/Array.h \
    $$BANG_ROOT/src/Engine/DataStructures/List.h \
    $$BANG_ROOT/src/Engine/DataStructures/Map.h \
    $$BANG_ROOT/src/Engine/DataStructures/String.h \
    $$BANG_ROOT/src/Engine/Math/Quaternion.h \
    $$BANG_ROOT/src/Engine/Math/Matrix3.h \
    $$BANG_ROOT/src/Engine/Math/Matrix4.h \
    $$BANG_ROOT/src/Engine/IO/Files/File.h
