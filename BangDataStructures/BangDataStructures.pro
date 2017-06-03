include(../BangCommon.pri)
system(rm -f Makefile) # Force always running of qmake

TEMPLATE = lib
QT = core gui widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangDataStructures
INCLUDEPATH += ../include
OBJECTS -= $$system("ls -1 . | grep moc_ | xargs")

SOURCES += \
    ../src/Domain/Engine/DataStructures/Timer.cpp \
    ../src/Domain/Engine/DataStructures/List.cpp \
    ../src/Domain/Engine/DataStructures/Map.cpp \
    ../src/Domain/Engine/DataStructures/Array.cpp \
    ../src/Domain/Engine/DataStructures/String.cpp \
    ../src/Domain/Engine/DataStructures/Property.cpp \
    ../src/Domain/Engine/DataStructures/TypeMap.cpp \
    ../src/Domain/Engine/DataStructures/Set.cpp \
    ../src/Domain/Engine/DataStructures/UMap.cpp \
    ../src/Domain/Engine/DataStructures/Collection.cpp \
    ../src/Domain/Engine/DataStructures/Regex.cpp \
    ../src/Domain/Engine/Physics/Rect.cpp \
    ../src/Domain/Engine/Physics/Vector3.cpp \
    ../src/Domain/Engine/Physics/Quaternion.cpp \
    ../src/Domain/Engine/Physics/Matrix4.cpp \
    ../src/Domain/Engine/Physics/Vector4.cpp \
    ../src/Domain/Engine/Physics/Vector2.cpp \
    ../src/Domain/Engine/Physics/Color.cpp \
    ../src/Domain/Engine/Physics/Math.cpp \
    ../src/Domain/Engine/Physics/Random.cpp \
    ../src/Persistence/Files/Path.cpp \
    ../src/Domain/Engine/Debug/OStreamOperators.cpp \
    ../src/Persistence/Files/File.cpp
