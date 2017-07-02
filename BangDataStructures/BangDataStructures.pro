include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
QT = core gui widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangDataStructures
INCLUDEPATH += ../include
OBJECTS -= $$system("ls -1 . | grep moc_ | xargs")

SOURCES += \
    ../src/Engine/DataStructures/Timer.cpp \
    ../src/Engine/DataStructures/List.cpp \
    ../src/Engine/DataStructures/Map.cpp \
    ../src/Engine/DataStructures/Array.cpp \
    ../src/Engine/DataStructures/String.cpp \
    ../src/Engine/DataStructures/Property.cpp \
    ../src/Engine/DataStructures/TypeMap.cpp \
    ../src/Engine/DataStructures/Set.cpp \
    ../src/Engine/DataStructures/UMap.cpp \
    ../src/Engine/DataStructures/Collection.cpp \
    ../src/Engine/DataStructures/Regex.cpp \
    ../src/Engine/Math/Rect.cpp \
    ../src/Engine/Math/Vector3.cpp \
    ../src/Engine/Math/Quaternion.cpp \
    ../src/Engine/Math/Matrix4.cpp \
    ../src/Engine/Math/Vector4.cpp \
    ../src/Engine/Math/Vector2.cpp \
    ../src/Engine/Math/Color.cpp \
    ../src/Engine/Math/Math.cpp \
    ../src/Engine/Math/Random.cpp \
    ../src/Engine/IO/Files/Path.cpp \
    ../src/Engine/Debug/OStreamOperators.cpp \
    ../src/Engine/IO/Files/File.cpp
