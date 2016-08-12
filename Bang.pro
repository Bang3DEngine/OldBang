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
# CONFIG += GAME
CONFIG += qt

include(deployment.pri)
qtcAddDeployment()


QMAKE_CXXFLAGS += -g --std=c++11 -Wl,--export-dynamic -Wno-unused-parameter -Wunused-variable -Wno-sign-compare -fPIC -O2

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
    $$PWD/BangPreprocessor/GeneratedFiles/ \
    $$PWD/src/View/Inspector/ \

EDITOR {
    INCLUDEPATH += \
        $$PWD/src/View/ \
        $$PWD/src/View/Interfaces/ \
        $$PWD/src/View/Dialogs/ \
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
    $$PWD/Assets/Behaviours/LookAtLuigi.h \
    $$PWD/Assets/Behaviours/LookAtLuigi.cpp \
    Assets/Engine/Meshes/Pyramid.obj \
    Assets/Engine/Meshes/Cube.obj \
    Assets/Engine/Meshes/Cone.obj \
    Assets/Engine/Meshes/Sphere.obj \
    Assets/Engine/Meshes/Cube.bmesh \
    Assets/Engine/Meshes/Cone.bmesh \
    Assets/Engine/Meshes/Sphere.bmesh \
    Assets/Engine/Meshes/Pyramid.bmesh \
    Assets/Engine/Shaders/PR_DirectionalLight.frag \
    Assets/Engine/Shaders/D2G_Line.frag \
    Assets/Engine/Shaders/D2G_Line.vert \
    Assets/Engine/Shaders/D2G_LineRotationAxis.frag \
    Assets/Engine/Shaders/D2G_LineRotationAxis.vert \
    Assets/Engine/Shaders/D2G_Default.frag \
    Assets/Engine/Shaders/D2G_Default.vert \
    Assets/Engine/Shaders/PR_DrawScreenPlane.vert \
    Assets/Materials/luigiMaterial.bmat \
    Assets/Materials/testMaterial.bmat \
    Assets/Materials/testMaterial2.bmat \
    Assets/Engine/Shaders/SelectionBuffer.frag \
    Assets/Engine/Materials/RenderGBufferToScreen.bmat \
    Assets/Engine/Shaders/RenderGBufferToScreen.frag \
    Assets/Engine/Shaders/PR_PointLight.frag \
    Assets/Scenes/tmp.bscene \
    Assets/Scenes/test2.bscene \
    Assets/Engine/Materials/D2G_Default.bmat \
    Assets/Engine/Materials/D2G_Line.bmat \
    Assets/Engine/Materials/D2G_LineRotationAxis.bmat \
    Assets/Engine/Materials/PR_AfterLighting.bmat \
    Assets/Engine/Materials/PR_DirectionalLight.bmat \
    Assets/Engine/Materials/PR_PointLight.bmat \
    Assets/Engine/Shaders/PR_AfterLighting.frag \
    Assets/Behaviours/RandomScale.h \
    Assets/Behaviours/RandomScale.cpp \
    Assets/Engine/Shaders/PR_DrawScreenPlane.vert \
    Assets/Materials/victor.bmat \
    Assets/Engine/Meshes/Plane.bmesh \
    Assets/Meshes/luigiMesh.bmesh \
    Assets/Meshes/boyMesh.bmesh \
    Assets/Textures/victor.btex2d \
    Assets/Textures/woodTexture.btex2d \
    Assets/Textures/luigiTexture.btex2d \
    Assets/Textures/carpetTexture.btex2d \
    Assets/Scenes/test.bscene \
    Assets/Engine/Textures/DirectionalLightIcon.btex2d \
    Assets/Engine/Textures/PointLightIcon .btex2d \
    Assets/Engine/Textures/PointLightIcon .btex2d \
    Assets/Engine/Textures/PointLightIcon.btex2d

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
    src/Domain/Engine/Components/PointLight.h \
    src/Domain/Engine/Others/Property.h \
    src/Persistence/File.h \
    src/Persistence/XMLParser.h \
    src/Persistence/XMLNode.h \
    src/Persistence/XMLAttribute.h \
    src/Persistence/XMLProperty.h \
    src/Domain/Engine/Others/NamedEnum.h \
    src/Domain/Engine/Physics/Vector4.h \
    src/Domain/Engine/Physics/Vector2.h \
    src/Domain/Engine/Physics/Color.h \
    src/View/Inspector/IAttrWidgetButtonListener.h \
    src/View/Application.h

EDITOR {
HEADERS += \
    src/View/EditorGameObject/EditorSelectionGameObject.h \
    src/View/EditorScene/EditorDebugGameObject.h \
    src/View/EditorGameObject/EditorTranslateAxis.h \
    src/View/EditorGameObject/EditorTranslateAxisGroup.h \
    src/View/Inspector/AttrWidgetButton.h \
    src/View/Toolbar.h \
    src/View/WindowEventManager.h \
    src/View/Interfaces/IWindowEventManagerListener.h \
    src/View/Inspector/Inspector.h \
    src/View/Hierarchy/Hierarchy.h \
    src/View/Inspector/ComponentWidget.h \
    src/View/Inspector/AttributeWidget.h \
    src/View/Inspector/AttrWidgetFloat.h \
    src/View/Inspector/AttrWidgetVectorFloat.h \
    src/View/Logger/ListLogger.h \
    src/View/Logger/LoggerWidget.h \
    src/View/Inspector/AttrWidgetEnum.h \
    src/View/Explorer/Explorer.h \
    src/View/Inspector/AttrWidgetFile.h \
    src/View/Inspector/IInspectable.h \
    src/View/MenuBar.h \
    src/Persistence/ImageFile.h \
    src/Persistence/MeshFile.h \
    src/Persistence/Texture2DAssetFile.h \
    src/Persistence/MeshAssetFile.h \
    src/View/Inspector/MeshAssetFileInspectable.h \
    src/Persistence/MaterialAssetFile.h \
    src/View/Inspector/MaterialAssetFileInspectable.h \
    src/View/Explorer/ExplorerDirTree.h \
    src/View/Inspector/InspectorWidget.h \
    src/View/Inspector/ImageFileInspectable.h \
    src/View/Inspector/AttrWidgetString.h \
    src/View/Inspector/MeshFileInspectable.h \
    src/View/EditorCamera/EditorCamera.h \
    src/View/EditorGameObject/EditorGameObject.h \
    src/View/EditorScene/EditorScene.h \
    src/Domain/Graphics/SelectionFramebuffer.h \
    src/View/Inspector/Texture2DAssetFileInspectable.h \
    src/View/Dialogs/FileDialog.h \
    src/View/EditorGameObject/EditorBBox.h \
    src/View/EditorGameObject/EditorRotateAxisGroup.h \
    src/View/EditorGameObject/EditorRotateAxis.h \
    src/View/EditorGameObject/EditorAxisGroup.h \
    src/View/EditorGameObject/EditorAxis.h \
    src/View/EditorGameObject/EditorScaleAxisGroup.h \
    src/View/EditorGameObject/EditorScaleAxis.h \
    src/View/EditorScene/EditorFloor.h \
    src/Domain/Engine/Others/Gizmos.h \
    src/View/Inspector/AttrWidgetBool.h \
    src/View/Inspector/AttrWidgetColor.h \
    src/View/Inspector/TextFileInspectable.h \
    src/View/Inspector/TextFile.h \
    src/View/Inspector/PrefabAssetFileInspectable.h \
    src/View/Interfaces/IDroppableWidget.h
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
    src/Domain/Engine/Components/PointLight.cpp \
    src/Domain/Engine/Others/Property.cpp \
    src/Persistence/File.cpp \
    src/Persistence/XMLParser.cpp \
    src/Persistence/XMLNode.cpp \
    src/Persistence/XMLAttribute.cpp \
    src/Persistence/XMLProperty.cpp \
    src/Domain/Engine/Physics/Vector4.cpp \
    src/Domain/Engine/Physics/Vector2.cpp \
    src/Domain/Engine/Physics/Color.cpp \
    src/View/Inspector/IAttrWidgetButtonListener.cpp \
    src/View/Application.cpp

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
        src/Domain/Graphics/SelectionFramebuffer.cpp \
        src/View/Inspector/ComponentWidget.cpp \
        src/View/Inspector/AttributeWidget.cpp \
        src/View/Inspector/AttrWidgetFloat.cpp \
        src/View/Inspector/AttrWidgetVectorFloat.cpp \
        src/View/Logger/ListLogger.cpp \
        src/View/Logger/LoggerWidget.cpp \
        src/View/Inspector/AttrWidgetEnum.cpp \
        src/View/Explorer/Explorer.cpp \
        src/View/Inspector/AttrWidgetFile.cpp \
        src/View/Inspector/IInspectable.cpp \
        src/View/MenuBar.cpp \
        src/View/Explorer/ExplorerDirTree.cpp \
        src/View/Inspector/InspectorWidget.cpp \
        src/View/Inspector/ImageFileInspectable.cpp \
        src/View/Inspector/AttrWidgetString.cpp \
        src/View/Inspector/MeshFileInspectable.cpp \
        src/View/EditorCamera/EditorCamera.cpp \
        src/View/EditorGameObject/EditorGameObject.cpp \
        src/View/EditorScene/EditorScene.cpp \
        src/View/Inspector/Texture2DAssetFileInspectable.cpp \
        src/View/Dialogs/FileDialog.cpp \
        src/Persistence/ImageFile.cpp \
        src/Persistence/MeshFile.cpp \
        src/Persistence/Texture2DAssetFile.cpp \
        src/Persistence/MeshAssetFile.cpp \
        src/View/Inspector/MeshAssetFileInspectable.cpp \
        src/Persistence/MaterialAssetFile.cpp \
        src/View/Inspector/MaterialAssetFileInspectable.cpp \
        src/View/EditorGameObject/EditorBBox.cpp \
        src/View/EditorGameObject/EditorSelectionGameObject.cpp \
        src/View/EditorScene/EditorDebugGameObject.cpp \
        src/View/EditorGameObject/EditorTranslateAxisGroup.cpp \
        src/View/EditorGameObject/EditorTranslateAxis.cpp \
        src/View/Toolbar.cpp \
        src/View/Inspector/AttrWidgetButton.cpp \
        src/Domain/Engine/Others/Gizmos.cpp \
        src/View/Inspector/AttrWidgetBool.cpp \
        src/View/Inspector/AttrWidgetColor.cpp \
        src/View/Inspector/TextFileInspectable.cpp \
        src/View/Inspector/TextFile.cpp \
        src/View/Inspector/PrefabAssetFileInspectable.cpp \
        src/View/Interfaces/IDroppableWidget.cpp
}

GAME {
    SOURCES += \
        src/View/GameWindow.cpp
}

SUBDIRS += \
    BangPreprocessor/BangPreprocessor.pro
