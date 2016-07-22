TEMPLATE = app
QT_VERSION = 4
QT += core gui opengl

# Call this qmake using either:
#   qmake "BUILD_MODE=EDITOR"
#   qmake "BUILD_MODE=GAME"

# $$BUILD_MODE:
#    EDITOR
#    GAME

message(Building $$BUILD_MODE ...)
CONFIG += $$BUILD_MODE
CONFIG += EDITOR

include(deployment.pri)
qtcAddDeployment()

CONFIG += qt

QMAKE_CXXFLAGS += -g -Wl,--export-dynamic --std=c++11 -Wno-unused-parameter -Wunused-variable -Wno-sign-compare -fPIC

LIBS += -lGLEW -ldl

DEFINES += QT_PROJECT_PATH=\\\"$$PWD\\\"

EDITOR {
    DEFINES += BANG_EDITOR
    RESOURCES = Assets/Engine/qdarkstyle/style.qrc
    OBJECTS_DIR = objEditor/
    TARGET = BangEditor.exe
}

GAME {
    OBJECTS_DIR = objGame/
    TARGET = Game.exe
}


INCLUDEPATH += \
    $$PWD/src/ \
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
    $$PWD/src/View/ \
    $$PWD/BangPreprocessor/GeneratedFiles/

EDITOR {
    INCLUDEPATH += \
        $$PWD/src/View/ \
        $$PWD/src/View/Interfaces/ \
        $$PWD/src/View/Dialogs/ \
        $$PWD/src/View/Inspector/ \
        $$PWD/src/View/Hierarchy/ \
        $$PWD/src/View/Explorer/ \
        $$PWD/src/View/Logger/ \
        $$PWD/src/View/EditorCamera/ \
        $$PWD/src/View/EditorGameObject/ \
        $$PWD/src/View/EditorScene/ \
}


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
    $$PWD/Assets/Behaviours/TestBehaviour.cpp / \
    $$PWD/Assets/Behaviours/TestBehaviour.h / \
    Assets/Engine/Meshes/Pyramid.obj \
    Assets/Engine/Meshes/Cube.obj \
    Assets/Engine/Meshes/Cone.obj \
    Assets/Engine/Meshes/Sphere.obj \
    Assets/Engine/Meshes/Cube.bmesh \
    Assets/Engine/Meshes/Cone.bmesh \
    Assets/Engine/Meshes/Sphere.bmesh \
    Assets/Engine/Meshes/Pyramid.bmesh \
    Assets/Engine/Shaders/PR_DirectionalLight.frag \
    Assets/Engine/Materials/DirectionalLight.bmat \
    Assets/Engine/Materials/Default.bmat \
    Assets/Engine/Materials/Line.bmat \
    Assets/Engine/Materials/LineRotationAxis.bmat \
    Assets/Engine/Shaders/D2G_Line.frag \
    Assets/Engine/Shaders/D2G_Line.vert \
    Assets/Engine/Shaders/D2G_LineRotationAxis.frag \
    Assets/Engine/Shaders/D2G_LineRotationAxis.vert \
    Assets/Engine/Shaders/D2G_Default.frag \
    Assets/Engine/Shaders/D2G_Default.vert \
    Assets/Engine/Shaders/PR_Default.frag \
    Assets/Engine/Shaders/PR_Default.vert \
    Assets/Materials/luigiMaterial.bmat \
    Assets/Materials/testMaterial.bmat \
    Assets/Materials/testMaterial2.bmat \
    Assets/Engine/Shaders/SelectionBuffer.frag \
    Assets/Engine/Materials/RenderGBufferToScreen.bmat \
    Assets/Engine/Shaders/RenderGBufferToScreen.frag \
    Assets/Engine/Materials/PointLight.bmat \
    Assets/Engine/Shaders/PR_PointLight.frag

HEADERS += \
    src/Domain/Engine/Physics/Rect.h \
    src/View/WindowMain.h \
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
    src/View/Canvas.h \
    src/Domain/Engine/Others/StringUtils.h \
    src/View/IWindow.h \
    src/Domain/Engine/Components/DirectionalLight.h \
    src/Domain/Engine/Components/Light.h \
    src/Domain/Graphics/ShaderPreprocessor.h \
    src/View/ClipboardGameObject.h \
    src/Domain/Engine/Components/PointLight.h

EDITOR {
HEADERS += \
    src/View/EditorGameObject/EditorSelectionGameObject.h \
    src/View/EditorScene/EditorDebugGameObject.h \
    src/View/EditorGameObject/EditorTranslateAxis.h \
    src/View/EditorGameObject/EditorTranslateAxisGroup.h \
    src/View/Inspector/InspectorButtonSW.h \
    src/View/Toolbar.h \
    src/View/WindowEventManager.h \
    src/View/Interfaces/IWindowEventManagerListener.h \
    src/View/Inspector/Inspector.h \
    src/View/Hierarchy/Hierarchy.h \
    src/View/Inspector/InspectorWidgetInfo.h \
    src/View/Inspector/InspectorComponentWidget.h \
    src/View/Inspector/InspectorSW.h \
    src/View/Inspector/InspectorFloatSW.h \
    src/View/Inspector/InspectorVFloatSW.h \
    src/View/Logger/ListLogger.h \
    src/View/Logger/LoggerWidget.h \
    src/View/Inspector/InspectorEnumSW.h \
    src/View/Explorer/Explorer.h \
    src/View/Inspector/InspectorFileSW.h \
    src/View/Inspector/IInspectable.h \
    src/View/MenuBar.h \
    src/View/Explorer/ExplorerDirTree.h \
    src/View/Inspector/InspectorWidget.h \
    src/View/Inspector/InspectorImageFileWidget.h \
    src/View/Explorer/File.h \
    src/View/Explorer/FileImage.h \
    src/View/Inspector/InspectorStringSW.h \
    src/View/Inspector/InspectorMeshFileWidget.h \
    src/View/Explorer/FileMesh.h \
    src/View/EditorCamera/EditorCamera.h \
    src/View/EditorGameObject/EditorGameObject.h \
    src/View/EditorScene/EditorScene.h \
    src/View/Explorer/FileTexture2DAsset.h \
    src/Domain/Graphics/SelectionFramebuffer.h \
    src/View/Inspector/InspectorTexture2DAssetWidget.h \
    src/View/Dialogs/FileDialog.h \
    src/View/EditorGameObject/EditorBBox.h \
    src/View/EditorGameObject/EditorRotateAxisGroup.h \
    src/View/EditorGameObject/EditorRotateAxis.h \
    src/View/EditorGameObject/EditorAxisGroup.h \
    src/View/EditorGameObject/EditorAxis.h \
    src/View/EditorGameObject/EditorScaleAxisGroup.h \
    src/View/EditorGameObject/EditorScaleAxis.h \
    src/View/EditorScene/EditorFloor.h
}

GAME {
    HEADERS += \
        src/View/GameWindow.h
}


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
    src/View/Canvas.cpp \
    src/Domain/Engine/Others/StringUtils.cpp \
    src/View/IWindow.cpp \
    src/Domain/Engine/Components/DirectionalLight.cpp \
    src/Domain/Engine/Components/Light.cpp \
    src/Domain/Graphics/ShaderPreprocessor.cpp \
    src/View/ClipboardGameObject.cpp \
    src/Domain/Engine/Interfaces/IFileable.cpp \
    src/Domain/Engine/Components/PointLight.cpp

EDITOR {
    SOURCES += \
        src/View/EditorGameObject/EditorRotateAxisGroup.cpp \
        src/View/EditorGameObject/EditorRotateAxis.cpp \
        src/View/EditorGameObject/EditorAxisGroup.cpp \
        src/View/EditorGameObject/EditorAxis.cpp \
        src/View/EditorGameObject/EditorScaleAxisGroup.cpp \
        src/View/EditorGameObject/EditorScaleAxis.cpp \
        src/View/EditorScene/EditorFloor.cpp \
        src/View/WindowMain.cpp \
        src/View/WindowEventManager.cpp \
        src/View/Interfaces/IWindowEventManagerListener.cpp \
        src/View/Inspector/Inspector.cpp \
        src/View/Hierarchy/Hierarchy.cpp \
        src/View/Inspector/InspectorWidgetInfo.cpp \
        src/Domain/Graphics/SelectionFramebuffer.cpp \
        src/View/Inspector/InspectorComponentWidget.cpp \
        src/View/Inspector/InspectorSW.cpp \
        src/View/Inspector/InspectorFloatSW.cpp \
        src/View/Inspector/InspectorVFloatSW.cpp \
        src/View/Logger/ListLogger.cpp \
        src/View/Logger/LoggerWidget.cpp \
        src/View/Inspector/InspectorEnumSW.cpp \
        src/View/Explorer/Explorer.cpp \
        src/View/Inspector/InspectorFileSW.cpp \
        src/View/Inspector/IInspectable.cpp \
        src/View/MenuBar.cpp \
        src/View/Explorer/ExplorerDirTree.cpp \
        src/View/Inspector/InspectorWidget.cpp \
        src/View/Inspector/InspectorImageFileWidget.cpp \
        src/View/Explorer/File.cpp \
        src/View/Explorer/FileImage.cpp \
        src/View/Inspector/InspectorStringSW.cpp \
        src/View/Inspector/InspectorMeshFileWidget.cpp \
        src/View/Explorer/FileMesh.cpp \
        src/View/EditorCamera/EditorCamera.cpp \
        src/View/EditorGameObject/EditorGameObject.cpp \
        src/View/EditorScene/EditorScene.cpp \
        src/View/Explorer/FileTexture2DAsset.cpp \
        src/View/Inspector/InspectorTexture2DAssetWidget.cpp \
        src/View/Dialogs/FileDialog.cpp \
        src/View/EditorGameObject/EditorBBox.cpp \
        src/View/EditorGameObject/EditorSelectionGameObject.cpp \
        src/View/EditorScene/EditorDebugGameObject.cpp \
        src/View/EditorGameObject/EditorTranslateAxisGroup.cpp \
        src/View/EditorGameObject/EditorTranslateAxis.cpp \
        src/View/Toolbar.cpp \
        src/View/Inspector/InspectorButtonSW.cpp
}

GAME {
    SOURCES += \
        src/View/GameWindow.cpp
}

SUBDIRS += \
    BangPreprocessor/BangPreprocessor.pro
