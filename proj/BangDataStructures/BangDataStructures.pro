include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
QT = core gui widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangDataStructures
INCLUDEPATH += $$PWD/../../include
OBJECTS -= $$system("ls -1 . | grep moc_ | xargs")

SOURCES += \
    $$PWD/../../src/Engine/DataStructures/Timer.cpp \
    $$PWD/../../src/Engine/DataStructures/List.cpp \
    $$PWD/../../src/Engine/DataStructures/Map.cpp \
    $$PWD/../../src/Engine/DataStructures/Array.cpp \
    $$PWD/../../src/Engine/DataStructures/String.cpp \
    $$PWD/../../src/Engine/DataStructures/Property.cpp \
    $$PWD/../../src/Engine/DataStructures/TypeMap.cpp \
    $$PWD/../../src/Engine/DataStructures/Set.cpp \
    $$PWD/../../src/Engine/DataStructures/UMap.cpp \
    $$PWD/../../src/Engine/DataStructures/Collection.cpp \
    $$PWD/../../src/Engine/DataStructures/Regex.cpp \
    $$PWD/../../src/Engine/Math/Rect.cpp \
    $$PWD/../../src/Engine/Math/Vector3.cpp \
    $$PWD/../../src/Engine/Math/Quaternion.cpp \
    $$PWD/../../src/Engine/Math/Matrix4.cpp \
    $$PWD/../../src/Engine/Math/Vector4.cpp \
    $$PWD/../../src/Engine/Math/Vector2.cpp \
    $$PWD/../../src/Engine/Math/Color.cpp \
    $$PWD/../../src/Engine/Math/Math.cpp \
    $$PWD/../../src/Engine/Math/Random.cpp \
    $$PWD/../../src/Engine/IO/Files/Path.cpp \
    $$PWD/../../src/Engine/Debug/OStreamOperators.cpp \
    $$PWD/../../src/Engine/IO/Files/File.cpp
