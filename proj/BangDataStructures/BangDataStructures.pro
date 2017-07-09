include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
QT = core gui widgets

CONFIG += dll

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

HEADERS += \
    $$PWD/../../src/Engine/Debug/OStreamOperators.h \
    $$PWD/../../src/Engine/Math/Math.h \
    $$PWD/../../src/Engine/Math/Random.h \
    $$PWD/../../src/Engine/IO/Files/Path.h \
    $$PWD/../../src/Engine/DataStructures/Collection.h \
    $$PWD/../../src/Engine/Math/Color.h \
    $$PWD/../../src/Engine/DataStructures/UMap.h \
    $$PWD/../../src/Engine/Math/Rect.h \
    $$PWD/../../src/Engine/Math/Vector3.h \
    $$PWD/../../src/Engine/Math/Vector4.h \
    $$PWD/../../src/Engine/Math/Vector2.h \
    $$PWD/../../src/Engine/DataStructures/Set.h \
    $$PWD/../../src/Engine/DataStructures/TypeMap.h \
    $$PWD/../../src/Engine/DataStructures/Array.h \
    $$PWD/../../src/Engine/DataStructures/List.h \
    $$PWD/../../src/Engine/DataStructures/Timer.h \
    $$PWD/../../src/Engine/DataStructures/Map.h \
    $$PWD/../../src/Engine/DataStructures/Property.h \
    $$PWD/../../src/Engine/DataStructures/String.h \
    $$PWD/../../src/Engine/DataStructures/NamedEnum.h \
    $$PWD/../../src/Engine/Math/Quaternion.h \
    $$PWD/../../src/Engine/Math/Matrix4.h \
    $$PWD/../../src/Engine/IO/Files/File.h
