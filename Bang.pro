TEMPLATE = app

QT_VERSION = 4

OBJECTS_DIR = obj/

include(deployment.pri)
qtcAddDeployment()

QT += core gui opengl

CONFIG += qt

QMAKE_CXXFLAGS += -g --std=c++11 -Wno-unused-parameter -Wno-sign-compare

LIBS += -lGLEW

DEFINES += QT_PROJECT_PATH=\\\"$$PWD\\\"

INCLUDEPATH += \
    $$PWD/src/ \
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
    $$PWD/BangPreprocessor/GeneratedFiles/ \
    $$PWD/res/Assets/UserBehaviours/


OTHER_FILES += \
    .gitignore \
    run.sh \
    updateUI.sh \
    src/Domain \
    res/Scenes/sceneTest1.scene \
    res/Scenes/sceneFileFormat.txt \
    res/Scenes/assetsTest.assets \
    res/Assets/Meshes/Pyramid.obj \
    res/Shaders/pass_pos.frag \
    res/Shaders/pass_pos_normal.frag \
    res/Shaders/pass_pos_normal_uv.frag \
    res/Shaders/render_to_screen.frag \
    res/Shaders/model_pos.vert \
    res/Shaders/model_pos_normal.vert \
    res/Shaders/pass_pos.vert \
    res/Shaders/pass_pos_normal.vert \
    res/Shaders/pvm_pos_normal.vert \
    res/Shaders/pvm_pos_normal_uv.vert \
    res/Shaders/render_to_screen.vert \
    res/Assets/Meshes/Gandalf.obj \
    res/Assets/Meshes/Boy.obj \
    res/Assets/testMaterial.asset \
    res/Assets/woodTexture.asset \
    res/Assets/boyMesh.asset \
    res/Assets/carpetTexture.asset \
    res/Assets/testMaterial2.asset \
    res/Assets/prefabTest.asset \
    res/Assets/userBehavioursNamesList.ubnl \
    res/Assets/luigiPrefab.asset \
    res/Assets/luigiMesh.asset \
    res/Assets/luigiMaterial.asset \
    res/Assets/luigiTexture.asset \
    res/Shaders/lines.frag \
    res/Shaders/lines.vert \
    res/Shaders/pvm_pos.vert \
    res/Shaders/selection.frag

HEADERS += \
    src/Domain/Engine/Physics/Rect.h \
    src/View/Canvas.h \
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
    src/Domain/Engine/Assets/Meshes/MeshPlane.h \
    src/Domain/Engine/Assets/Meshes/MeshPyramid.h \
    src/Domain/Engine/Assets/Meshes/MeshScreenPlane.h \
    src/Domain/Engine/Components/Behaviour.h \
    src/Domain/Engine/Interfaces/IFileable.h \
    src/Domain/Engine/Assets/Texture2D.h \
    src/Domain/Engine/Assets/Prefab.h \
    $$files(res/Assets/UserBehaviours/*.h,true) \
    src/View/WindowEventManager.h \
    src/View/Interfaces/IWindowEventManagerListener.h \
    src/Bang.h \
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
    src/Persistence/AssetsManager.h \
    src/Persistence/FileReader.h \
    src/Persistence/stb_image.h \
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
    src/Domain/Engine/Others/Input.h \
    src/Persistence/FileWriter.h \
    src/Persistence/Persistence.h \
    src/Domain/Engine/Components/LineRenderer.h \
    src/Domain/Engine/Components/Renderer.h \
    src/View/Explorer/FileTexture2DAsset.h \
    src/View/Inspector/InspectorTexture2DAssetWidget.h \
    src/View/Dialogs/FileDialog.h \
    src/View/EditorGameObject/EditorBBox.h \
    src/Domain/Engine/Physics/Box.h \
    src/View/EditorGameObject/EditorSelectionGameObject.h \
    src/Domain/Engine/Physics/Vector3.h \
    src/Domain/Engine/Physics/Quaternion.h \
    src/Domain/Engine/Physics/Matrix4.h \
    src/View/EditorScene/EditorDebugGameObject.h \
    src/Domain/Engine/Physics/Sphere.h \
    src/Domain/Graphics/SelectionFramebuffer.h \
    src/View/EditorGameObject/EditorAxisGroup.h \
    src/View/EditorGameObject/EditorAxis.h

SOURCES += \
    src/Domain/Engine/Physics/Rect.cpp \
    src/View/Canvas.cpp \
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
    src/Domain/Engine/Assets/Meshes/MeshPlane.cpp \
    src/Domain/Engine/Assets/Meshes/MeshScreenPlane.cpp \
    src/Domain/Engine/Assets/Meshes/MeshPyramid.cpp \
    src/Domain/Engine/Assets/Material.cpp \
    src/Domain/Engine/Components/Behaviour.cpp \
    src/Domain/Engine/Assets/Texture2D.cpp \
    src/Domain/Engine/Assets/Prefab.cpp \
    $$files(res/Assets/UserBehaviours/*.cpp,true) \
    src/View/WindowMain.cpp \
    src/View/WindowEventManager.cpp \
    src/View/Interfaces/IWindowEventManagerListener.cpp \
    src/View/Inspector/Inspector.cpp \
    src/View/Hierarchy/Hierarchy.cpp \
    src/View/Inspector/InspectorWidgetInfo.cpp \
    src/View/Inspector/InspectorComponentWidget.cpp \
    src/View/Inspector/InspectorSW.cpp \
    src/View/Inspector/InspectorFloatSW.cpp \
    src/View/Inspector/InspectorVFloatSW.cpp \
    src/View/Logger/ListLogger.cpp \
    src/View/Logger/LoggerWidget.cpp \
    src/View/Inspector/InspectorEnumSW.cpp \
    src/Persistence/stb_image.cpp \
    src/Persistence/AssetsManager.cpp \
    src/Persistence/FileReader.cpp \
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
    src/Domain/Engine/Others/Input.cpp \
    src/Persistence/FileWriter.cpp \
    src/Persistence/Persistence.cpp \
    src/Domain/Engine/Components/LineRenderer.cpp \
    src/Domain/Engine/Components/Renderer.cpp \
    src/View/Explorer/FileTexture2DAsset.cpp \
    src/View/Inspector/InspectorTexture2DAssetWidget.cpp \
    src/View/Dialogs/FileDialog.cpp \
    src/View/EditorGameObject/EditorBBox.cpp \
    src/Domain/Engine/Physics/Box.cpp \
    src/View/EditorGameObject/EditorSelectionGameObject.cpp \
    src/Domain/Engine/Physics/Vector3.cpp \
    src/Domain/Engine/Physics/Quaternion.cpp \
    src/Domain/Engine/Physics/Matrix4.cpp \
    src/View/EditorScene/EditorDebugGameObject.cpp \
    src/Domain/Engine/Physics/Sphere.cpp \
    src/Domain/Graphics/SelectionFramebuffer.cpp \
    src/View/EditorGameObject/EditorAxisGroup.cpp \
    src/View/EditorGameObject/EditorAxis.cpp

SUBDIRS += \
    BangPreprocessor/BangPreprocessor.pro
