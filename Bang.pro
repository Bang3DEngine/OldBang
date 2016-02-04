TEMPLATE = app

QT_VERSION = 4

OBJECTS_DIR = obj/

include(deployment.pri)
qtcAddDeployment()

QT += core gui opengl

CONFIG += qt

QMAKE_CXXFLAGS += --std=c++11

LIBS += -lGLEW

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/include/Domain \
    $$PWD/include/View \


OTHER_FILES += \
    .gitignore \
    run.sh \
    updateUI.sh



HEADERS += \
    include/Domain/GLBindable.h \
    include/Domain/GLIdable.h \
    include/Domain/IToString.h \
    include/Domain/Logger.h \
    include/Domain/Material.h \
    include/Domain/Mesh.h \
    include/Domain/MeshRenderer.h \
    include/Domain/Shader.h \
    include/Domain/ShaderProgram.h \
    include/Domain/VAO.h \
    include/Domain/VBO.h \
    include/View/Canvas.h \
    include/View/WindowMain.h

SOURCES += \
    src/Domain/Logger.cpp \
    src/Domain/main.cpp \
    src/Domain/Material.cpp \
    src/Domain/Mesh.cpp \
    src/Domain/MeshRenderer.cpp \
    src/Domain/Shader.cpp \
    src/Domain/ShaderProgram.cpp \
    src/Domain/VAO.cpp \
    src/Domain/VBO.cpp \
    src/View/Canvas.cpp
