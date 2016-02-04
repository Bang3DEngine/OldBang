TEMPLATE = app

QT_VERSION = 4

SOURCES += \
    View/Canvas.cpp \
    Domain/main.cpp \
    Domain/Shader.cpp \
    Domain/Logger.cpp \
    Domain/VBO.cpp \
    Domain/VAO.cpp \
    Domain/ShaderProgram.cpp

OBJECTS_DIR = obj/

include(deployment.pri)
qtcAddDeployment()

QT += core gui opengl

OTHER_FILES += \
    .gitignore \
    run.sh \
    updateUI.sh

CONFIG += qt

QMAKE_CXXFLAGS += --std=c++11

LIBS += -lGLEW

HEADERS += \
    View/Canvas.h \
    Domain/Shader.h \
    Domain/IToString.h \
    Domain/Logger.h \
    Domain/VBO.h \
    Domain/VAO.h \
    Domain/Idable.h \
    Domain/ShaderProgram.h

