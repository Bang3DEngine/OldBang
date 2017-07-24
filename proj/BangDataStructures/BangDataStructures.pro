include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
QT = core gui widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangDataStructures

SOURCES += \
    $$BANG_ROOT/src/Engine/DataStructures/List.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/Map.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/Array.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/String.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/Property.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/TypeMap.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/Set.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/UMap.cpp \
    $$BANG_ROOT/src/Engine/DataStructures/Collection.cpp \
    $$BANG_ROOT/src/Engine/Math/Rect.cpp \
    $$BANG_ROOT/src/Engine/Math/Quaternion.cpp \
    $$BANG_ROOT/src/Engine/Math/Matrix4.cpp \
    $$BANG_ROOT/src/Engine/Math/Vector4.cpp \
    $$BANG_ROOT/src/Engine/Math/Color.cpp \
    $$BANG_ROOT/src/Engine/Math/Math.cpp \
    $$BANG_ROOT/src/Engine/Math/Random.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/Path.cpp \
    $$BANG_ROOT/src/Engine/Debug/OStreamOperators.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/File.cpp

HEADERS += \
    $$BANG_ROOT/src/Engine/Debug/OStreamOperators.h \
    $$BANG_ROOT/src/Engine/Math/Math.h \
    $$BANG_ROOT/src/Engine/Math/Random.h \
    $$BANG_ROOT/src/Engine/IO/Files/Path.h \
    $$BANG_ROOT/src/Engine/DataStructures/Collection.h \
    $$BANG_ROOT/src/Engine/Math/Color.h \
    $$BANG_ROOT/src/Engine/DataStructures/UMap.h \
    $$BANG_ROOT/src/Engine/Math/Rect.h \
    $$BANG_ROOT/src/Engine/Math/Vector3.h \
    $$BANG_ROOT/src/Engine/Math/Vector4.h \
    $$BANG_ROOT/src/Engine/Math/Vector2.h \
    $$BANG_ROOT/src/Engine/DataStructures/Set.h \
    $$BANG_ROOT/src/Engine/DataStructures/TypeMap.h \
    $$BANG_ROOT/src/Engine/DataStructures/Array.h \
    $$BANG_ROOT/src/Engine/DataStructures/List.h \
    $$BANG_ROOT/src/Engine/DataStructures/Map.h \
    $$BANG_ROOT/src/Engine/DataStructures/Property.h \
    $$BANG_ROOT/src/Engine/DataStructures/String.h \
    $$BANG_ROOT/src/Engine/DataStructures/NamedEnum.h \
    $$BANG_ROOT/src/Engine/Math/Quaternion.h \
    $$BANG_ROOT/src/Engine/Math/Matrix4.h \
    $$BANG_ROOT/src/Engine/IO/Files/File.h
