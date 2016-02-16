TEMPLATE = app

QT_VERSION = 4

OBJECTS_DIR = obj/

include(deployment.pri)
qtcAddDeployment()

QT += core gui opengl

CONFIG += qt

QMAKE_CXXFLAGS += -g --std=c++11

LIBS += -lGLEW

INCLUDEPATH += \
    $$PWD/src/ \
    $$PWD/src/View/ \
    $$PWD/src/Domain/Interfaces/ \
    $$PWD/src/Domain/Graphics/ \
    $$PWD/src/Domain/Graphics/Interfaces/ \
    $$PWD/src/Domain/Engine/Interfaces/ \
    $$PWD/src/Domain/Engine/Entities/ \
    $$PWD/src/Domain/Engine/Others/ \
    $$PWD/src/Domain/Engine/Parts/ \
    $$PWD/src/Domain/Engine/Parts/Meshes/ \
    $$PWD/src/Domain/Engine/Parts/Behaviours/ \
    $$PWD/src/Domain/Engine/Physics/ \
    $$PWD/src/Domain/Graphics/ \


OTHER_FILES += \
    .gitignore \
    run.sh \
    updateUI.sh \
    res/Shaders/pass.vert \
    res/Shaders/model.vert \
    res/Shaders/pass_pos_normal.frag \
    res/Shaders/pass_pos.frag \
    res/Shaders/pass_pos_normal.vert \
    res/Shaders/pass_pos.vert \
    res/Shaders/model_pos_normal.vert \
    res/Shaders/model_pos.vert \
    src/Domain \
    res/Shaders/pvm_pos_normal.vert \
    res/Shaders/pass_pos_normal_uv.frag \
    res/Shaders/pvm_pos_normal_uv.vert \
    res/Shaders/render_to_screen.frag \
    res/Shaders/render_to_screen.vert \
    res/Stages/stageTest1.stage \
    res/Stages/stageFileFormat.txt

HEADERS += \
    src/Domain/Engine/Physics/Rect.h \
    src/View/Canvas.h \
    src/View/WindowMain.h \
    src/Domain/Interfaces/IToString.h \
    src/Domain/Engine/Interfaces/IStageEventListener.h \
    src/Domain/Engine/Others/stb_image.h \
    src/Domain/Engine/Others/Logger.h \
    src/Domain/Engine/Others/FileLoader.h \
    src/Domain/Engine/Others/Timer.h \
    src/Domain/Engine/Others/Time.h \
    src/Domain/Engine/Entities/Entity.h \
    src/Domain/Engine/Entities/Stage.h \
    src/Domain/Graphics/ShaderContract.h \
    src/Domain/Graphics/ShaderProgram.h \
    src/Domain/Graphics/Texture.h \
    src/Domain/Graphics/VAO.h \
    src/Domain/Graphics/Framebuffer.h \
    src/Domain/Graphics/Shader.h \
    src/Domain/Graphics/Texture2D.h \
    src/Domain/Graphics/GBuffer.h \
    src/Domain/Graphics/TextureRender.h \
    src/Domain/Graphics/VBO.h \
    src/Domain/Graphics/TextureDepth.h \
    src/Domain/Graphics/Interfaces/IGLBindable.h \
    src/Domain/Graphics/Interfaces/IToString.h \
    src/Domain/Graphics/Interfaces/IGLIdable.h \
    src/Domain/Engine/Parts/Meshes/MeshScreenPlane.h \
    src/Domain/Engine/Parts/Meshes/MeshPyramid.h \
    src/Domain/Engine/Parts/Meshes/MeshPlane.h \
    src/Domain/Engine/Parts/Meshes/Mesh.h \
    src/Domain/Engine/Parts/Behaviours/TestCameraBehaviour.h \
    src/Domain/Engine/Parts/Behaviours/TestBehaviour.h \
    src/Domain/Engine/Parts/Behaviours/Behaviour.h \
    src/Domain/Engine/Parts/Transform.h \
    src/Domain/Engine/Parts/Part.h \
    src/Domain/Engine/Parts/MeshRenderer.h \
    src/Domain/Engine/Parts/Material.h \
    src/Domain/Engine/Parts/Camera.h \
    src/Domain/Engine/Others/StageLoader.h

SOURCES += \
    src/Domain/Engine/Physics/Rect.cpp \
    src/View/Canvas.cpp \
    src/Domain/Engine/Others/stb_image.cpp \
    src/Domain/Engine/Others/FileLoader.cpp \
    src/Domain/Engine/Others/Logger.cpp \
    src/Domain/Engine/Others/Timer.cpp \
    src/Domain/Engine/Others/Time.cpp \
    src/Domain/Engine/Entities/Entity.cpp \
    src/Domain/Engine/Entities/Stage.cpp \
    src/Domain/Graphics/ShaderProgram.cpp \
    src/Domain/Graphics/ShaderContract.cpp \
    src/Domain/Graphics/GBuffer.cpp \
    src/Domain/Graphics/Texture.cpp \
    src/Domain/Graphics/Framebuffer.cpp \
    src/Domain/Graphics/VAO.cpp \
    src/Domain/Graphics/Shader.cpp \
    src/Domain/Graphics/Texture2D.cpp \
    src/Domain/Graphics/TextureRender.cpp \
    src/Domain/Graphics/TextureDepth.cpp \
    src/Domain/Graphics/VBO.cpp \
    src/Domain/Graphics/Interfaces/IGLBindable.cpp \
    src/Domain/Engine/Parts/Meshes/MeshScreenPlane.cpp \
    src/Domain/Engine/Parts/Meshes/MeshPyramid.cpp \
    src/Domain/Engine/Parts/Meshes/MeshPlane.cpp \
    src/Domain/Engine/Parts/Meshes/Mesh.cpp \
    src/Domain/Engine/Parts/Behaviours/TestCameraBehaviour.cpp \
    src/Domain/Engine/Parts/Behaviours/TestBehaviour.cpp \
    src/Domain/Engine/Parts/Behaviours/Behaviour.cpp \
    src/Domain/Engine/Parts/Transform.cpp \
    src/Domain/Engine/Parts/Part.cpp \
    src/Domain/Engine/Parts/MeshRenderer.cpp \
    src/Domain/Engine/Parts/Material.cpp \
    src/Domain/Engine/Parts/Camera.cpp \
    src/main.cpp \
    src/Domain/Engine/Others/StageLoader.cpp
