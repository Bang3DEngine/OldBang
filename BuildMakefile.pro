TEMPLATE = app

QT_VERSION = 4

TARGET = Game.exe
OBJECTS_DIR = obj-buildGame/

include(deployment.pri)
qtcAddDeployment()

QT += core gui opengl

CONFIG += qt

QMAKE_CXXFLAGS += -g --std=c++11 -Wno-unused-parameter -Wunused-variable -Wno-sign-compare

LIBS += -lGLEW -ldl

DEFINES += QT_PROJECT_PATH=\\\"$$PWD\\\"

RESOURCES = Assets/Engine/qdarkstyle/style.qrc

INCLUDEPATH += \
    $$PWD/src/ \
    $$PWD/src/View/ \
    $$PWD/src/Domain/Interfaces/ \
    $$PWD/src/Domain/Graphics/ \
    $$PWD/src/Domain/Graphics/Interfaces/ \
    $$PWD/src/Domain/Engine/Interfaces/ \
    $$PWD/src/Domain/Engine/GameObjects/ \
    $$PWD/src/Domain/Engine/Others/ \
    $$PWD/src/Domain/Engine/Components/ \
    $$PWD/src/Domain/Engine/Readers/ \
    $$PWD/src/Domain/Engine/Assets/ \
    $$PWD/src/Domain/Engine/Assets/Meshes/ \
    $$PWD/src/Domain/Engine/Assets/Behaviours/ \
    $$PWD/src/Domain/Engine/Physics/ \
    $$PWD/src/Persistence/ \
    $$PWD/src/Domain/Graphics/ \
    $$PWD/BangPreprocessor/GeneratedFiles/

OTHER_FILES += \
    .gitignore \
    Assets/Engine/qdarkstyle/style.qss \
    run.sh \
    updateUI.sh \
    src/Domain \
    Assets/Scenes/sceneTest1.scene \
    Assets/Scenes/sceneFileFormat.txt \
    Assets/Scenes/assetsTest.assets \
    Assets/Assets/Meshes/Pyramid.obj \
    Assets/Engine/Materials \
    Assets/Engine/Shaders/pass_pos.frag \
    Assets/Engine/Shaders/pass_pos_normal.frag \
    Assets/Engine/Shaders/pass_pos_normal_uv.frag \
    Assets/Engine/Shaders/render_to_screen.frag \
    Assets/Engine/Shaders/model_pos.vert \
    Assets/Engine/Shaders/model_pos_normal.vert \
    Assets/Engine/Shaders/pass_pos.vert \
    Assets/Engine/Shaders/pass_pos_normal.vert \
    Assets/Engine/Shaders/pvm_pos_normal.vert \
    Assets/Engine/Shaders/pvm_pos_normal_uv.vert \
    Assets/Engine/Shaders/render_to_screen.vert \
    Assets/Engine/Shaders/lines.frag \
    Assets/Engine/Shaders/lines.vert \
    Assets/Engine/Shaders/pvm_pos.vert \
    Assets/Engine/Shaders/selection.frag

HEADERS += \
    src/Domain/Engine/Physics/Rect.h \
    src/Domain/Interfaces/IToString.h \
    src/Domain/Engine/Interfaces/ISceneEventListener.h \
    src/Domain/Engine/Others/Logger.h \
    src/Domain/Engine/Others/Timer.h \
    src/Domain/Engine/Others/Time.h \
    src/Domain/Engine/GameObjects/GameObject.h \
    src/Domain/Engine/GameObjects/Scene.h \
    src/Domain/Graphics/ShaderContract.h \
    src/Domain/Graphics/ShaderProgram.h \
    src/Domain/Graphics/Texture.h \
    src/Domain/Graphics/VAO.h \
    src/Domain/Graphics/Framebuffer.h \
    src/Domain/Graphics/Shader.h \
    src/Domain/Graphics/GBuffer.h \
    src/Domain/Graphics/TextureRender.h \
    src/Domain/Graphics/VBO.h \
    src/Domain/Graphics/TextureDepth.h \
    src/Domain/Graphics/Interfaces/IGLBindable.h \
    src/Domain/Graphics/Interfaces/IToString.h \
    src/Domain/Graphics/Interfaces/IGLIdable.h \
    src/Domain/Engine/Components/Transform.h \
    src/Domain/Engine/Components/Component.h \
    src/Domain/Engine/Components/MeshRenderer.h \
    src/Domain/Engine/Components/Camera.h \
    src/Domain/Engine/Assets/Asset.h \
    src/Domain/Engine/Assets/Material.h \
    src/Domain/Engine/Assets/Meshes/Mesh.h \
    src/Domain/Engine/Assets/Meshes/MeshPyramid.h \
    src/Domain/Engine/Components/Behaviour.h \
    src/Domain/Engine/Interfaces/IFileable.h \
    src/Domain/Engine/Assets/Texture2D.h \
    src/Domain/Engine/Assets/Prefab.h \
    src/Bang.h \
    src/Persistence/AssetsManager.h \
    src/Persistence/FileReader.h \
    src/Persistence/stb_image.h \
    src/Domain/Engine/Others/Input.h \
    src/Persistence/FileWriter.h \
    src/Persistence/Persistence.h \
    src/Domain/Engine/Components/LineRenderer.h \
    src/Domain/Engine/Components/Renderer.h \
    src/Domain/Engine/Physics/Box.h \
    src/Domain/Engine/Physics/Vector3.h \
    src/Domain/Engine/Physics/Quaternion.h \
    src/Domain/Engine/Physics/Matrix4.h \
    src/Domain/Engine/Physics/Sphere.h \
    src/Domain/Engine/Others/SystemUtils.h \
    src/Domain/Engine/Components/BehaviourHolder.h \
    src/Domain/Engine/Others/SingletonManager.h \
    src/Domain/Engine/Components/CircleRenderer.h \
    src/Domain/Engine/Components/SingleLineRenderer.h \
    src/Domain/Engine/Components/LineStripRenderer.h \
    src/Domain/Engine/Assets/Meshes/MeshFactory.h \
    src/Domain/Engine/Interfaces/ICloneable.h \
    src/Domain/Engine/Others/GameBuilder.h \
    src/Domain/Engine/Others/StringUtils.h
 
HEADERS += src/View/Canvas.h src/View/WindowMain.h

SOURCES += \
    src/Domain/Engine/Physics/Rect.cpp \
    src/Domain/Engine/Others/Logger.cpp \
    src/Domain/Engine/Others/Timer.cpp \
    src/Domain/Engine/Others/Time.cpp \
    src/Domain/Engine/GameObjects/GameObject.cpp \
    src/Domain/Engine/GameObjects/Scene.cpp \
    src/Domain/Graphics/ShaderProgram.cpp \
    src/Domain/Graphics/ShaderContract.cpp \
    src/Domain/Graphics/GBuffer.cpp \
    src/Domain/Graphics/Texture.cpp \
    src/Domain/Graphics/Framebuffer.cpp \
    src/Domain/Graphics/VAO.cpp \
    src/Domain/Graphics/Shader.cpp \
    src/Domain/Graphics/TextureRender.cpp \
    src/Domain/Graphics/TextureDepth.cpp \
    src/Domain/Graphics/VBO.cpp \
    src/Domain/Graphics/Interfaces/IGLBindable.cpp \
    src/Domain/Engine/Components/Transform.cpp \
    src/Domain/Engine/Components/Component.cpp \
    src/Domain/Engine/Components/MeshRenderer.cpp \
    src/Domain/Engine/Components/Camera.cpp \
    src/main.cpp \
    src/Domain/Engine/Assets/Asset.cpp \
    src/Domain/Engine/Assets/Meshes/Mesh.cpp \
    src/Domain/Engine/Assets/Meshes/MeshPyramid.cpp \
    src/Domain/Engine/Assets/Material.cpp \
    src/Domain/Engine/Components/Behaviour.cpp \
    src/Domain/Engine/Assets/Texture2D.cpp \
    src/Domain/Engine/Assets/Prefab.cpp \
    src/Persistence/stb_image.cpp \
    src/Persistence/AssetsManager.cpp \
    src/Persistence/FileReader.cpp \
    src/Domain/Engine/Others/Input.cpp \
    src/Persistence/FileWriter.cpp \
    src/Persistence/Persistence.cpp \
    src/Domain/Engine/Components/LineRenderer.cpp \
    src/Domain/Engine/Components/Renderer.cpp \
    src/Domain/Engine/Physics/Box.cpp \
    src/Domain/Engine/Physics/Vector3.cpp \
    src/Domain/Engine/Physics/Quaternion.cpp \
    src/Domain/Engine/Physics/Matrix4.cpp \
    src/Domain/Engine/Physics/Sphere.cpp \
    src/Domain/Engine/Others/SystemUtils.cpp \
    src/Domain/Engine/Components/BehaviourHolder.cpp \
    src/Domain/Engine/Others/SingletonManager.cpp \
    src/Domain/Engine/Components/CircleRenderer.cpp \
    src/Domain/Engine/Components/SingleLineRenderer.cpp \
    src/Domain/Engine/Components/LineStripRenderer.cpp \
    src/Domain/Engine/Assets/Meshes/MeshFactory.cpp \
    src/Domain/Engine/Interfaces/ICloneable.cpp \
    src/Domain/Engine/Others/GameBuilder.cpp \
    src/Domain/Engine/Others/StringUtils.cpp

SOURCES += src/View/Canvas.cpp src/View/WindowMain.cpp

SUBDIRS += \
    BangPreprocessor/BangPreprocessor.pro
