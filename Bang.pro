TEMPLATE = app
QT_VERSION = 4
QT += core gui opengl

# Call this qmake using either:
#   qmake "BUILD_MODE=EDITOR"
#   qmake "BUILD_MODE=GAME"

# $$BUILD_MODE:
#    EDITOR
#    GAME

# message(Building $$BUILD_MODE ...)
CONFIG += $$BUILD_MODE
# CONFIG += EDITOR
# CONFIG += GAME
CONFIG += qt

include(deployment.pri)
qtcAddDeployment()

DEFINES += QT_PROJECT_PATH=\\\"$$PWD\\\"

QMAKE_CXXFLAGS += -g --std=c++11 -Wl,--export-dynamic -Wno-unused-parameter -Wunused-variable -Wno-sign-compare -fPIC
QMAKE_CXXFLAGS += -O3

FreeTypeTarget.commands = cd src/Domain/Graphics/FreeType
!exists (src/Domain/Graphics/FreeType/build) {
    message("Compiling FreeType")
    CONFIG += COMPILE_FREETYPE
    FreeTypeTarget.commands += && sh autogen.sh \
                               && mkdir build ; cd build \
                               && cmake .. && ../configure
}

FreeTypeTarget.commands += && cd build ; make
QMAKE_EXTRA_TARGETS += FreeTypeTarget
PRE_TARGETDEPS = FreeTypeTarget

LIBS += src/Domain/Graphics/FreeType/build/*.o
#LIBS += FreeTypeLib

LIBS += \
    -lGLEW  \ # GLEW, just GLEW
    -ldl    \ # To load libraries and stuff
    -lpng   \ # To read & write pngs, used by FreeType
    -lz -lbz2 # Used by FreeType too

EDITOR {
    DEFINES += BANG_EDITOR
    RESOURCES = Assets/Engine/qdarkstyle/style.qrc
    OBJECTS_DIR += objEditor/
    TARGET = BangEditor.exe
}

GAME {
    OBJECTS_DIR += objGame/
    TARGET = Game.exe
}


INCLUDEPATH += \
    $$PWD/src/ \
    $$PWD/src/Domain/Interfaces/ \
    $$PWD/src/Domain/Graphics/ \
    $$PWD/src/Domain/Graphics/FreeType/include \
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
    $$PWD/src/View/Inspector/

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
    Assets/Engine/Shaders/D2G_Default.frag \
    Assets/Engine/Shaders/D2G_Default.vert \
    Assets/Materials/luigiMaterial.bmat \
    Assets/Materials/testMaterial.bmat \
    Assets/Materials/testMaterial2.bmat \
    Assets/Engine/Shaders/SelectionBuffer.frag \
    Assets/Engine/Shaders/RenderGBufferToScreen.frag \
    Assets/Engine/Shaders/PR_PointLight.frag \
    Assets/Scenes/tmp.bscene \
    Assets/Scenes/test2.bscene \
    Assets/Engine/Materials/D2G_Default.bmat \
    Assets/Engine/Materials/PR_EditorEffects.bmat \
    Assets/Engine/Materials/PR_DirectionalLight.bmat \
    Assets/Engine/Materials/PR_PointLight.bmat \
    Assets/Behaviours/RandomScale.h \
    Assets/Behaviours/RandomScale.cpp \
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
    Assets/Engine/Textures/PointLightIcon.btex2d \
    Assets/Engine/Shaders/PR_AmbientLight.frag \
    Assets/Engine/Shaders/D2G.vert \
    Assets/Engine/Shaders/PR.vert \
    Assets/Engine/Shaders/PR.frag \
    Assets/Engine/Shaders/Include/D2G.frag \
    Assets/Engine/Shaders/Include/PR.frag \
    Assets/Engine/Shaders/Include/D2G.vert \
    Assets/Engine/Shaders/Include/PR.vert \
    Assets/Engine/Shaders/Include/Uniforms.glsl \
    Assets/Engine/Shaders/Include/PointLight.glsl \
    Assets/Engine/Shaders/Include/DirectionalLight.glsl \
    Assets/Engine/Materials/PR_AmbientLight.bmat \
    Assets/Engine/Shaders/PR_MeshPass.vert \
    Assets/Engine/Materials/PR_DirectionalLight_Screen.bmat \
    Assets/Engine/Materials/PR_PointLight_Screen.bmat \
    Assets/Engine/Shaders/UI/PR_UIImage.frag \
    Assets/Engine/Materials/UI/PR_UIImage.bmat \
    Assets/Engine/Shaders/PR_RenderBackground.frag \
    Assets/Engine/Shaders/Include/Main.glsl \
    Assets/Engine/Shaders/SelectionBuffer.vert \
    Assets/Engine/Shaders/PR_SelectionEffect.frag \
    Assets/Engine/Shaders/D2G_RotationAxisLine.frag \
    Assets/Engine/Materials/D2G_RotationAxisLine.bmat \
    Assets/Engine/Materials/PR_MeshPass.bmat \
    Assets/Engine/Materials/RenderGBufferToScreen.bmat \
    Assets/Engine/Shaders/PR_ScreenPass.vert \
    Assets/Engine/Shaders/PR_MeshPass.vert \
    Assets/Engine/Materials/PR_AmbientLight_Screen.bmat \
    Assets/Engine/Materials/PR_SelectionEffect.bmat \
    Assets/Engine/Shaders/UI/D2G_UIText.frag \
    Assets/Engine/Materials/UI/D2G_UIText.bmat

HEADERS += \
    src/Domain/Engine/Physics/Rect.h \
    src/View/EditorWindow.h \
    src/Domain/Interfaces/IToString.h \
    src/Domain/Engine/Interfaces/ISceneEventListener.h \
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
    src/Domain/Engine/Assets/Meshes/MeshFactory.h \
    src/Domain/Engine/Interfaces/ICloneable.h \
    src/Domain/Engine/Others/GameBuilder.h \
    src/View/Screen.h \
    src/Domain/Engine/Others/StringUtils.h \
    src/View/IWindow.h \
    src/Domain/Engine/Components/DirectionalLight.h \
    src/Domain/Engine/Components/Light.h \
    src/Domain/Graphics/ShaderPreprocessor.h \
    src/View/GameObjectClipboard.h \
    src/Domain/Engine/Components/PointLight.h \
    src/Domain/Engine/Others/Property.h \
    src/Persistence/XMLParser.h \
    src/Persistence/XMLNode.h \
    src/Persistence/XMLAttribute.h \
    src/Persistence/XMLProperty.h \
    src/Domain/Engine/Others/NamedEnum.h \
    src/Domain/Engine/Physics/Vector4.h \
    src/Domain/Engine/Physics/Vector2.h \
    src/Domain/Engine/Physics/Color.h \
    src/View/Application.h \
    src/Domain/Engine/Others/String.h \
    src/Domain/Engine/Others/SceneManager.h \
    src/Domain/Engine/Others/Debug.h \
    src/Persistence/BehaviourManager.h \
    src/Persistence/BehaviourManagerCompileThread.h \
    src/Domain/Engine/Others/Chrono.h \
    src/Persistence/ImageFile.h \
    src/Persistence/MeshFile.h \
    src/Persistence/Texture2DAssetFile.h \
    src/Persistence/MeshAssetFile.h \
    src/Persistence/MaterialAssetFile.h \
    src/Persistence/File.h \
    src/Domain/Engine/Others/Map.h \
    src/Domain/Engine/Others/List.h \
    src/Domain/Engine/Others/Array.h \
    src/Domain/Engine/Others/GraphicPipeline.h \
    src/Domain/Engine/Components/Canvas.h \
    src/Domain/Engine/Components/UIImage.h \
    src/Domain/Engine/Components/UIRenderer.h \
    src/Domain/Graphics/FontSheetCreator.h \
    src/Domain/Engine/Assets/Font.h \
    src/Persistence/FontAssetFile.h \
    src/Domain/Engine/Components/UIText.h

EDITOR {
HEADERS += \
    src/View/EditorGameObject/EditorSelectionGameObject.h \
    src/View/EditorScene/EditorDebugGameObject.h \
    src/View/EditorGameObject/EditorTranslateAxis.h \
    src/View/EditorGameObject/EditorTranslateAxisGroup.h \
    src/View/Inspector/AttrWidgetButton.h \
    src/View/Toolbar.h \
    src/View/WindowEventManager.h \
    src/View/Inspector/AttrWidgetFile.h \
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
    src/View/Inspector/IInspectable.h \
    src/View/MenuBar.h \
    src/View/Explorer/ExplorerDirTree.h \
    src/View/Inspector/InspectorWidget.h \
    src/View/Inspector/TextFile.h \
    src/View/Inspector/ImageFileInspectable.h \
    src/View/Inspector/MeshFileInspectable.h \
    src/View/Inspector/MeshAssetFileInspectable.h \
    src/View/Inspector/MaterialAssetFileInspectable.h \
    src/View/Inspector/AttrWidgetString.h \
    src/View/Inspector/Texture2DAssetFileInspectable.h \
    src/View/Inspector/PrefabAssetFileInspectable.h \
    src/View/Dialogs/FileDialog.h \
    src/View/Inspector/TextFileInspectable.h \
    src/View/EditorCamera/EditorCamera.h \
    src/View/EditorGameObject/EditorGameObject.h \
    src/View/EditorScene/EditorScene.h \
    src/Domain/Graphics/SelectionFramebuffer.h \
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
    src/View/Interfaces/DragDropAgent.h \
    src/View/Inspector/IAttrWidgetButtonListener.h \
    src/View/Inspector/ComponentClipboard.h \
    src/View/DragDropManager.h \
    src/View/Hierarchy/HierarchyDragDropManager.h \
    src/View/Interfaces/IDragDropListener.h \
    src/View/Hierarchy/HierarchyContextMenu.h \
    src/View/Explorer/ExplorerContextMenu.h \
    src/View/Inspector/ComponentWidgetContextMenu.h \
    src/View/Inspector/InspectorContextMenu.h \
    src/View/ContextMenu.h \
    src/View/ShortcutManager.h \
    src/View/EditorGameObject/EditorAxisPlane.h \
    src/View/IShortcutListener.h
}

GAME {
    HEADERS += \
        src/View/GameWindow.h
}


SOURCES += \
    src/Domain/Engine/Physics/Rect.cpp \
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
    src/Persistence/ImageFile.cpp \
    src/Persistence/MeshFile.cpp \
    src/Persistence/Texture2DAssetFile.cpp \
    src/Persistence/MeshAssetFile.cpp \
    src/Persistence/MaterialAssetFile.cpp \
    src/View/Inspector/TextFile.cpp \
    src/Persistence/File.cpp \
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
    src/Domain/Engine/Assets/Meshes/MeshFactory.cpp \
    src/Domain/Engine/Interfaces/ICloneable.cpp \
    src/Domain/Engine/Others/GameBuilder.cpp \
    src/View/Screen.cpp \
    src/Domain/Engine/Others/StringUtils.cpp \
    src/View/IWindow.cpp \
    src/Domain/Engine/Components/DirectionalLight.cpp \
    src/Domain/Engine/Components/Light.cpp \
    src/Domain/Graphics/ShaderPreprocessor.cpp \
    src/View/GameObjectClipboard.cpp \
    src/Domain/Engine/Interfaces/IFileable.cpp \
    src/Domain/Engine/Components/PointLight.cpp \
    src/Domain/Engine/Others/Property.cpp \
    src/Persistence/XMLParser.cpp \
    src/Persistence/XMLNode.cpp \
    src/Persistence/XMLAttribute.cpp \
    src/Persistence/XMLProperty.cpp \
    src/Domain/Engine/Physics/Vector4.cpp \
    src/Domain/Engine/Physics/Vector2.cpp \
    src/Domain/Engine/Physics/Color.cpp \
    src/View/Application.cpp \
    src/View/Inspector/IAttrWidgetButtonListener.cpp \
    src/Domain/Engine/Others/String.cpp \
    src/Domain/Engine/Others/SceneManager.cpp \
    src/Domain/Engine/Others/Debug.cpp \
    src/Persistence/BehaviourManager.cpp \
    src/Persistence/BehaviourManagerCompileThread.cpp \
    src/Domain/Engine/Others/Chrono.cpp \
    src/Domain/Engine/Others/Map.cpp \
    src/Domain/Engine/Others/List.cpp \
    src/Domain/Engine/Others/Array.cpp \
    src/Domain/Engine/Others/GraphicPipeline.cpp \
    src/Domain/Engine/Components/Canvas.cpp \
    src/Domain/Engine/Components/UIImage.cpp \
    src/Domain/Engine/Components/UIRenderer.cpp \
    src/Domain/Graphics/FontSheetCreator.cpp \
    src/Domain/Engine/Assets/Font.cpp \
    src/Persistence/FontAssetFile.cpp \
    src/Domain/Engine/Components/UIText.cpp

EDITOR {
    SOURCES += \
        src/View/EditorGameObject/EditorRotateAxisGroup.cpp \
        src/View/EditorGameObject/EditorRotateAxis.cpp \
        src/View/EditorGameObject/EditorAxisGroup.cpp \
        src/View/EditorGameObject/EditorAxis.cpp \
        src/View/EditorGameObject/EditorScaleAxisGroup.cpp \
        src/View/EditorGameObject/EditorScaleAxis.cpp \
        src/View/EditorScene/EditorFloor.cpp \
        src/View/EditorWindow.cpp \
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
        src/View/Inspector/IInspectable.cpp \
        src/View/MenuBar.cpp \
        src/View/Inspector/AttrWidgetFile.cpp \
        src/View/Explorer/ExplorerDirTree.cpp \
        src/View/Inspector/InspectorWidget.cpp \
        src/View/Inspector/AttrWidgetString.cpp \
        src/View/EditorCamera/EditorCamera.cpp \
        src/View/EditorGameObject/EditorGameObject.cpp \
        src/View/EditorScene/EditorScene.cpp \
        src/View/Inspector/MeshAssetFileInspectable.cpp \
        src/View/Inspector/Texture2DAssetFileInspectable.cpp \
        src/View/Dialogs/FileDialog.cpp \
        src/View/Inspector/ImageFileInspectable.cpp \
        src/View/Inspector/MeshFileInspectable.cpp \
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
        src/View/Inspector/PrefabAssetFileInspectable.cpp \
        src/View/Interfaces/DragDropAgent.cpp \
        src/View/Inspector/ComponentClipboard.cpp \
        src/View/DragDropManager.cpp \
        src/View/Hierarchy/HierarchyDragDropManager.cpp \
        src/View/Interfaces/IDragDropListener.cpp \
        src/View/Hierarchy/HierarchyContextMenu.cpp \
        src/View/Explorer/ExplorerContextMenu.cpp \
        src/View/Inspector/ComponentWidgetContextMenu.cpp \
        src/View/Inspector/InspectorContextMenu.cpp \
        src/View/ContextMenu.cpp \
        src/View/ShortcutManager.cpp \
        src/View/IShortcutListener.cpp \
        src/View/EditorGameObject/EditorAxisPlane.cpp
}

GAME {
    SOURCES += \
        src/View/GameWindow.cpp
}

SUBDIRS += \
    BangPreprocessor/BangPreprocessor.pro
