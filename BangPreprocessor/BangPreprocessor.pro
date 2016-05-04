TEMPLATE = app

QT_VERSION = 4

OBJECTS_DIR = obj/

QT += core gui opengl

CONFIG += qt

QMAKE_CXXFLAGS += -g --std=c++11


include(deployment.pri)
qtcAddDeployment()

SOURCES += \
    BangPreprocessorMain.cpp

INCLUDEPATH += \
    $$PWD/../src/ \
    $$PWD/../src/View/ \
    $$PWD/../src/Domain/Interfaces/ \
    $$PWD/../src/Domain/Graphics/ \
    $$PWD/../src/Domain/Graphics/Interfaces/ \
    $$PWD/../src/Domain/Engine/Interfaces/ \
    $$PWD/../src/Domain/Engine/Entities/ \
    $$PWD/../src/Domain/Engine/Others/ \
    $$PWD/../src/Domain/Engine/Components/ \
    $$PWD/../src/Domain/Engine/Readers/ \
    $$PWD/../src/Domain/Engine/Assets/ \
    $$PWD/../src/Domain/Engine/Assets/Meshes/ \
    $$PWD/../src/Domain/Engine/Assets/Behaviours/ \
    $$PWD/../src/Domain/Engine/Physics/ \
    $$PWD/../src/Domain/Graphics/ \

OTHER_FILES += \
    GeneratedFiles/BP_SceneReader_cpp_UserBehaviours_elseifs.bp
