TEMPLATE = app
QT_VERSION = 5
QT += core gui opengl widgets

# Call this qmake using either:
#   qmake "BUILD_MODE=EDITOR RELEASE_OR_DEBUG=RELEASE"
#   qmake "BUILD_MODE=EDITOR RELEASE_OR_DEBUG=DEBUG"
#   qmake "BUILD_MODE=GAME RELEASE_OR_DEBUG=RELEASE"
#   qmake "BUILD_MODE=GAME RELEASE_OR_DEBUG=DEBUG"


# PARAMETERS ###############

# $$BUILD_MODE:
#    EDITOR
#    GAME

# $$RELEASE_OR_DEBUG:
#    RELEASE_MODE
#    DEBUG_MODE

# DEFAULT PARAMETERS
BUILD_MODE = $$BUILD_MODE # EDITOR or GAME
isEmpty(BUILD_MODE) {
  BUILD_MODE = EDITOR
} # message(BUILD_MODE: $$BUILD_MODE)

RELEASE_OR_DEBUG = $$RELEASE_OR_DEBUG # RELEASE or DEBUG
isEmpty(RELEASE_OR_DEBUG) {
  RELEASE_OR_DEBUG = RELEASE_MODE
} # message(RELEASE_OR_DEBUG: $$RELEASE_OR_DEBUG)
#

###################################




# QMake config #############
CONFIG += $$BUILD_MODE $$RELEASE_OR_DEBUG
CONFIG += qt
MOC_DIR = src/Qt_MOCs/
EDITOR {
    FORMS   += EngineAssets/EditorWindow.ui
    FORMS   += EngineAssets/SelectProjectWindow.ui
}
####################################



# G++ options (release, debug, etc.) ############
DEBUG_MODE {
    OPTIMIZATION_OPT = -O0 -Wl,-O0
    DEBUG_OPT = -g
}
RELEASE_MODE {
    OPTIMIZATION_OPT = -O3 -Wl,-O3
}
#OTHER_OPTS = -Wall --std=c++11 -Wl,--export-dynamic -Wno-unused-parameter \
             #-Wunused-variable -Wno-sign-compare -fPIC
OTHER_OPTS = -Wall --std=c++11 -Wl,--export-dynamic -Wno-unused-parameter \
             -Wno-sign-compare -fPIC
QMAKE_CFLAGS           -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CFLAGS_DEBUG     -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CFLAGS_RELEASE   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS         -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS_DEBUG   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_CXXFLAGS_RELEASE -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS           -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS_DEBUG     -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS_RELEASE   -= -O0 -O1 -O2 -O3 -Wl,-O0 -Wl,-O1 -Wl,-O2 -Wl,-O3 -g
QMAKE_LFLAGS           += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT
QMAKE_CXXFLAGS         += $$OTHER_OPTS $$DEBUG_OPT $$OPTIMIZATION_OPT

DEFINES += QT_PROJECT_PATH=\\\"$$PWD\\\"

EDITOR {
    DEFINES += BANG_EDITOR
    RESOURCES += EngineAssets/qdarkstyle/style.qrc
    RESOURCES += EngineAssets/QtResources.qrc
    OBJECTS_DIR = bin/objEditor/
    TARGET = bin/Bang
}
GAME {
    OBJECTS_DIR = bin/objGame/
    TARGET = bin/Game.exe
}
###################################################




# LIBS ###########################
#!exists (src/Domain/Graphics/FreeType/build) {
#    FreeTypeTarget.commands += && sh autogen.sh \
#                               && mkdir build ; cd build \
#                               && cmake .. && ../configure
#}
#FreeTypeTarget.commands = cd src/Domain/Graphics/FreeType && \
#                          rm -rf build ; \
#                          sh autogen.sh && \
#                          mkdir build ; \
#                          cd build && \
#                          #../configure && \
#                          cmake .. && make
#
#QMAKE_EXTRA_TARGETS += FreeTypeTarget
#PRE_TARGETDEPS = FreeTypeTarget
#LIBS += src/Domain/Graphics/FreeType/build/*.o
LIBS += \
    -lGLEW            \ # GLEW
    -ldl              \ # To load libraries and stuff
    -lalut -lopenal   \ # Alut and OpenAL
    -lfreetype
########################################



# INCLUDE PATH ###################
INCLUDEPATH +=                                   \
    /usr/include                                 \
    /usr/include/freetype2                       \
    /usr/include/freetype2/freetype              \
                                                 \
    $$PWD/src/                                   \
    $$PWD/src/Domain/Interfaces/                 \
    $$PWD/src/Domain/Graphics/                   \
    $$PWD/src/Domain/Graphics/Buffers/           \
    $$PWD/src/Domain/Graphics/Shaders/           \
    $$PWD/src/Domain/Graphics/Interfaces/        \
    $$PWD/src/Domain/Engine/                     \
    $$PWD/src/Domain/Engine/Interfaces/          \
    $$PWD/src/Domain/Engine/GraphicPipeline/     \
    $$PWD/src/Domain/Engine/Debug/               \
    $$PWD/src/Domain/Engine/GameObjects/         \
    $$PWD/src/Domain/Engine/DataStructures/      \
    $$PWD/src/Domain/Engine/Others/              \
    $$PWD/src/Domain/Engine/Audio/               \
    $$PWD/src/Domain/Engine/Components/          \
    $$PWD/src/Domain/Engine/Readers/             \
    $$PWD/src/Domain/Engine/Assets/              \
    $$PWD/src/Domain/Engine/Assets/Meshes/       \
    $$PWD/src/Domain/Engine/Assets/Behaviours/   \
    $$PWD/src/Domain/Engine/Physics/             \
    $$PWD/src/View/                              \
    $$PWD/src/Persistence/                       \
    $$PWD/src/Persistence/Project                \
    $$PWD/src/Persistence/Behaviours/            \
    $$PWD/src/Persistence/Files/                 \
    $$PWD/src/Persistence/IO/                    \
    $$PWD/src/Persistence/XML/                   \
                                                 \
    $$PWD/src/View/                              \
    $$PWD/src/View/Windows

EDITOR {
    INCLUDEPATH +=                                           \
        $$PWD/src/View/Interfaces/                           \
        $$PWD/src/View/DragDrop/                             \
        $$PWD/src/View/Dialogs/                              \
        $$PWD/src/View/Hierarchy/                            \
        $$PWD/src/View/Explorer/                             \
        $$PWD/src/View/Console/                              \
        $$PWD/src/View/EditorCamera/                         \
        $$PWD/src/View/EditorGameObject/                     \
        $$PWD/src/View/EditorGameObject/EditorRectTransform  \
        $$PWD/src/View/EditorScene/                          \
        $$PWD/src/View/Inspector/                            \
        $$PWD/src/View/Inspector/Inspectables/               \
        $$PWD/src/View/Inspector/AttributeWidgets/
}
######################################



# HEADERS ##################
HEADERS +=                                                          \
    src/Domain/Engine/Physics/Rect.h                                \
    src/Domain/Interfaces/IToString.h                               \
    src/Domain/Engine/Interfaces/ISceneEventListener.h              \
    src/Domain/Engine/GameObjects/GameObject.h                      \
    src/Domain/Engine/GameObjects/Scene.h                           \
    src/Domain/Graphics/Texture.h                                   \
    src/Domain/Graphics/VAO.h                                       \
    src/Domain/Graphics/VBO.h                                       \
    src/Domain/Graphics/Interfaces/IGLBindable.h                    \
    src/Domain/Graphics/Interfaces/IToString.h                      \
    src/Domain/Graphics/Interfaces/IGLIdable.h                      \
    src/Domain/Engine/Components/Transform.h                        \
    src/Domain/Engine/Components/Component.h                        \
    src/Domain/Engine/Components/MeshRenderer.h                     \
    src/Domain/Engine/Components/Camera.h                           \
    src/Domain/Engine/Assets/Asset.h                                \
    src/Domain/Engine/Assets/Material.h                             \
    src/Domain/Engine/Assets/Meshes/Mesh.h                          \
    src/Domain/Engine/Components/Behaviour.h                        \
    src/Domain/Engine/Interfaces/IFileable.h                        \
    src/Domain/Engine/Assets/Texture2D.h                            \
    src/Domain/Engine/Assets/Prefab.h                               \
    src/Bang.h                                                      \
    src/Persistence/AssetsManager.h                                 \
    src/Domain/Engine/Others/Input.h                                \
    src/Persistence/Persistence.h                                   \
    src/Domain/Engine/Components/LineRenderer.h                     \
    src/Domain/Engine/Components/Renderer.h                         \
    src/Domain/Engine/Physics/Vector3.h                             \
    src/Domain/Engine/Physics/Quaternion.h                          \
    src/Domain/Engine/Physics/Matrix4.h                             \
    src/Domain/Engine/Physics/Sphere.h                              \
    src/Domain/Engine/Others/SystemUtils.h                          \
    src/Domain/Engine/Components/BehaviourHolder.h                  \
    src/Domain/Engine/Others/SingletonManager.h                     \
    src/Domain/Engine/Components/CircleRenderer.h                   \
    src/Domain/Engine/Components/SingleLineRenderer.h               \
    src/Domain/Engine/Assets/Meshes/MeshFactory.h                   \
    src/Domain/Engine/Interfaces/ICloneable.h                       \
    src/View/Screen.h                                               \
    src/Domain/Engine/Others/StringUtils.h                          \
    src/Domain/Engine/Components/DirectionalLight.h                 \
    src/Domain/Engine/Components/Light.h                            \
    src/View/GameObjectClipboard.h                                  \
    src/Domain/Engine/Components/PointLight.h                       \
    src/Domain/Engine/Physics/Vector4.h                             \
    src/Domain/Engine/Physics/Vector2.h                             \
    src/Domain/Engine/Physics/Color.h                               \
    src/View/Application.h                                          \
    src/Domain/Engine/Others/SceneManager.h                         \
    src/Domain/Engine/Components/Canvas.h                           \
    src/Domain/Engine/Components/UIImage.h                          \
    src/Domain/Engine/Components/UIRenderer.h                       \
    src/Domain/Graphics/FontSheetCreator.h                          \
    src/Domain/Engine/Assets/Font.h                                 \
    src/Domain/Engine/Components/UIText.h                           \
    src/Persistence/EngineConfig.h                                  \
    src/Domain/Engine/Physics/AABox.h                               \
    src/Domain/Engine/Assets/AudioClip.h                            \
    src/Domain/Engine/Components/AudioSource.h                      \
    src/Domain/Engine/Audio/AudioManager.h                          \
    src/Domain/Engine/Audio/AudioPlayerRunnable.h                   \
    src/Domain/Engine/DataStructures/Array.h                        \
    src/Domain/Engine/DataStructures/List.h                         \
    src/Domain/Engine/DataStructures/Map.h                          \
    src/Domain/Engine/DataStructures/NamedEnum.h                    \
    src/Domain/Engine/DataStructures/Property.h                     \
    src/Domain/Engine/DataStructures/String.h                       \
    src/Domain/Engine/Debug/Chrono.h                                \
    src/Domain/Engine/Debug/Debug.h                                 \
    src/Domain/Engine/Debug/Gizmos.h                                \
    src/Domain/Engine/Debug/Time.h                                  \
    src/Domain/Engine/DataStructures/Timer.h                        \
    src/Domain/Graphics/Buffers/Framebuffer.h                       \
    src/Domain/Graphics/Buffers/GBuffer.h                           \
    src/Domain/Graphics/Shaders/Shader.h                            \
    src/Domain/Graphics/Shaders/ShaderContract.h                    \
    src/Domain/Graphics/Shaders/ShaderProgram.h                     \
    src/Persistence/Behaviours/BehaviourManager.h                   \
    src/Persistence/Behaviours/BehaviourRefresherTimer.h            \
    src/Persistence/Files/File.h                                    \
    src/Persistence/IO/FileReader.h                                 \
    src/Persistence/Files/AudioClipAssetFile.h                      \
    src/Persistence/IO/FileWriter.h                                 \
    src/Persistence/Files/ImageFile.h                               \
    src/Persistence/Files/FontAssetFile.h                           \
    src/Persistence/Files/MaterialAssetFile.h                       \
    src/Persistence/Files/MeshAssetFile.h                           \
    src/Persistence/Files/MeshFile.h                                \
    src/Persistence/Files/Texture2DAssetFile.h                      \
    src/Persistence/XML/XMLAttribute.h                              \
    src/Persistence/XML/XMLNode.h                                   \
    src/Persistence/XML/XMLParser.h                                 \
    src/Persistence/XML/XMLProperty.h                               \
    src/Persistence/IO/stb_image.h                                  \
    src/Persistence/Files/TextFile.h \
    src/View/Windows/IWindow.h \
    src/View/Windows/GameWindow.h \
    src/Persistence/Files/AudioFile.h \
    src/Domain/Engine/Physics/Math.h \
    src/Domain/Engine/Physics/Random.h \
    src/Domain/Engine/Components/AudioListener.h \
    src/Persistence/Behaviours/BehaviourManagerStatus.h \
    src/Domain/Engine/Components/RectTransform.h \
    src/View/Cursor.h \
    src/Domain/Graphics/RenderTexture.h \
    src/Domain/Engine/HideFlags.h \
    src/Domain/Engine/Object.h \
    src/Persistence/IO/stb_image_write.h \
    src/Domain/Engine/Others/GraphicPipelineDebugger.h \
    src/Domain/Engine/GraphicPipeline/GraphicPipeline.h \
    src/Domain/Engine/GraphicPipeline/GraphicPipelinePass.h \
    src/Domain/Engine/GraphicPipeline/GPPass_SP_DeferredLights.h \
    src/Domain/Engine/GraphicPipeline/GPPass_DepthLayer.h \
    src/Domain/Engine/GraphicPipeline/GPPass_G_Gizmos.h \
    src/Domain/Graphics/Interfaces/IRenderAgent.h \
    src/Domain/Graphics/GLContext.h \
    src/Domain/Graphics/GL.h \
    src/Domain/Engine/DataStructures/TypeMap.h \
    src/Domain/Engine/CodePreprocessor.h \
    src/Domain/Graphics/Shaders/ShaderPreprocessor.h \
    src/Persistence/Project/Project.h \
    src/Persistence/Project/ProjectManager.h \
    src/Persistence/Behaviours/BehaviourObjectCompileRunnable.h \
    src/Persistence/Behaviours/BehaviourMergeObjectsRunnable.h \
    src/Domain/Engine/GraphicPipeline/GPPass_G.h

EDITOR {
HEADERS += \
    src/View/Windows/EditorWindow.h \
    src/Domain/Engine/GraphicPipeline/GPPass_Selection.h \
    src/View/EditorGameObject/EditorGizmosGameObject.h \
    src/View/DragDrop/DragDropQWidget.h \
    src/Persistence/Project/QtProjectManager.h \
    src/View/DragDrop/DragDropQListView.h \
    src/View/Explorer/FileReferencesManager.h \
    src/View/DragDrop/DragDropQTreeWidget.h \
    src/View/DragDrop/DragDropQListWidget.h \
    src/View/DragDrop/DragDropAgent.h \
    src/View/DragDrop/IDragDropListener.h \
    src/View/DragDrop/DragDropManager.h \
    src/View/Explorer/FileSystemModel.h \
    src/View/EditorGameObject/EditorSelectionGameObject.h \
    src/View/EditorScene/EditorDebugGameObject.h \
    src/Domain/Graphics/Buffers/SelectionFramebuffer.h              \
    src/View/EditorGameObject/EditorTranslateAxis.h \
    src/View/EditorGameObject/EditorTranslateAxisGroup.h \
    src/View/EditorPlayStopFlowController.h \
    src/View/Dialogs/DialogBrowseAssetFile.h                        \
    src/View/Inspector/AttributeWidgets/AttributeWidget.h           \
    src/View/Inspector/AttributeWidgets/AttrWidgetBool.h            \
    src/View/Inspector/AttributeWidgets/AttrWidgetButton.h          \
    src/View/Inspector/AttributeWidgets/AttrWidgetColor.h           \
    src/View/Inspector/AttributeWidgets/AttrWidgetEnum.h            \
    src/View/Inspector/AttributeWidgets/AttrWidgetFile.h            \
    src/View/Inspector/AttributeWidgets/AttrWidgetFloat.h           \
    src/View/Inspector/AttributeWidgets/AttrWidgetString.h          \
    src/View/Inspector/AttributeWidgets/AttrWidgetVectorFloat.h     \
    src/View/Inspector/AttributeWidgets/IAttrWidgetButtonListener.h \
    src/View/Inspector/Inspectables/IInspectable.h                  \
    src/View/Inspector/Inspectables/ImageFileInspectable.h          \
    src/View/Inspector/Inspectables/MaterialAssetFileInspectable.h  \
    src/View/Inspector/Inspectables/MeshAssetFileInspectable.h      \
    src/View/Inspector/Inspectables/MeshFileInspectable.h           \
    src/View/Inspector/Inspectables/PrefabAssetFileInspectable.h    \
    src/View/Inspector/Inspectables/TextFileInspectable.h           \
    src/View/Inspector/Inspectables/Texture2DAssetFileInspectable.h \
    src/View/Windows/WindowEventManager.h \
    src/View/Windows/SelectProjectWindow.h \
    src/Persistence/Files/AudioFileInspectable.h \
    src/View/Toolbar.h \
    src/View/Inspector/Inspectables/AudioClipAssetFileInspectable.h \
    src/View/EditorState.h \
    src/View/Dialogs/GameBuildDialog.h \
    src/View/GameBuilderJob.h \
    src/View/Dialogs/Dialog.h \
    src/View/Interfaces/IWindowEventManagerListener.h \
    src/View/Inspector/Inspector.h \
    src/View/Hierarchy/Hierarchy.h \
    src/View/Inspector/ComponentWidget.h \
    src/View/Console/Console.h \
    src/View/Explorer/Explorer.h \
    src/View/MenuBar.h \
    src/View/Explorer/ExplorerDirTree.h \
    src/View/Inspector/InspectorWidget.h \
    src/View/EditorCamera/EditorCamera.h \
    src/View/EditorScene/EditorScene.h \
    src/View/EditorGameObject/EditorBBox.h \
    src/View/EditorGameObject/EditorRotateAxisGroup.h \
    src/View/EditorGameObject/EditorRotateAxis.h \
    src/View/EditorGameObject/EditorAxisGroup.h \
    src/View/EditorGameObject/EditorAxis.h \
    src/View/EditorGameObject/EditorScaleAxisGroup.h \
    src/View/EditorGameObject/EditorScaleAxis.h \
    src/View/EditorScene/EditorFloor.h \
    src/View/Inspector/ComponentClipboard.h \
    src/View/Hierarchy/HierarchyDragDropManager.h \
    src/View/Hierarchy/HierarchyContextMenu.h \
    src/View/Explorer/ExplorerContextMenu.h \
    src/View/Inspector/ComponentWidgetContextMenu.h \
    src/View/Inspector/InspectorContextMenu.h \
    src/View/ContextMenu.h \
    src/Domain/Engine/Others/GameBuilder.h \
    src/View/ShortcutManager.h \
    src/View/EditorGameObject/EditorAxisPlane.h \
    src/View/Explorer/ExplorerFileSortProxy.h \
    src/View/Inspector/AttributeWidgets/AttrWidgetInt.h \
    src/View/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.h \
    src/View/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.h \
    src/View/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.h \
    src/View/EditorGameObject/EditorGizmo.h \
    src/View/IShortcutListener.h
}
GAME {
    HEADERS +=
}
######################################



# SOURCES #####################
SOURCES += \
    src/Domain/Engine/Physics/Rect.cpp \
    src/Domain/Engine/Others/Time.cpp \
    src/Domain/Engine/GameObjects/GameObject.cpp \
    src/Domain/Engine/GameObjects/Scene.cpp \
    src/Domain/Graphics/Texture.cpp \
    src/Domain/Graphics/VAO.cpp \
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
    src/Persistence/AssetsManager.cpp \
    src/Domain/Engine/Others/Input.cpp \
    src/Persistence/Persistence.cpp \
    src/Persistence/Files/TextFile.cpp \
    src/Domain/Engine/Components/LineRenderer.cpp \
    src/Domain/Engine/Components/Renderer.cpp \
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
    src/View/Screen.cpp \
    src/Domain/Engine/Others/StringUtils.cpp \
    src/Domain/Engine/Components/DirectionalLight.cpp \
    src/Domain/Engine/Components/Light.cpp \
    src/View/GameObjectClipboard.cpp \
    src/Domain/Engine/Interfaces/IFileable.cpp \
    src/Domain/Engine/Components/PointLight.cpp \
    src/Domain/Engine/Physics/Vector4.cpp \
    src/Domain/Engine/Physics/Vector2.cpp \
    src/Domain/Engine/Physics/Color.cpp \
    src/View/Application.cpp \
    src/Domain/Engine/Others/SceneManager.cpp \
    src/Domain/Engine/Components/Canvas.cpp \
    src/Domain/Engine/Components/UIImage.cpp \
    src/Domain/Engine/Components/UIRenderer.cpp \
    src/Domain/Graphics/FontSheetCreator.cpp \
    src/Domain/Engine/Assets/Font.cpp \
    src/Domain/Engine/Components/UIText.cpp \
    src/Persistence/EngineConfig.cpp \
    src/Domain/Engine/Physics/AABox.cpp \
    src/Domain/Engine/Assets/AudioClip.cpp \
    src/Domain/Engine/Components/AudioSource.cpp \
    src/Domain/Engine/Audio/AudioManager.cpp \
    src/Domain/Engine/Audio/AudioPlayerRunnable.cpp \
    src/Domain/Engine/DataStructures/List.cpp \
    src/Domain/Engine/DataStructures/Map.cpp \
    src/Domain/Engine/DataStructures/Array.cpp \
    src/Domain/Engine/DataStructures/Timer.cpp \
    src/Domain/Engine/DataStructures/String.cpp \
    src/Domain/Engine/DataStructures/Property.cpp \
    src/Domain/Engine/Debug/Gizmos.cpp \
    src/Domain/Engine/Debug/Debug.cpp \
    src/Domain/Engine/Debug/Chrono.cpp \
    src/Domain/Graphics/Buffers/Framebuffer.cpp \
    src/Domain/Graphics/Buffers/GBuffer.cpp \
    src/Domain/Graphics/Shaders/Shader.cpp \
    src/Domain/Graphics/Shaders/ShaderContract.cpp \
    src/Domain/Graphics/Shaders/ShaderProgram.cpp \
    src/Persistence/Files/AudioClipAssetFile.cpp \
    src/Persistence/Files/File.cpp \
    src/Persistence/IO/FileReader.cpp \
    src/Persistence/Files/ImageFile.cpp \
    src/Persistence/Files/FontAssetFile.cpp \
    src/Persistence/Files/MaterialAssetFile.cpp \
    src/Persistence/Files/MeshAssetFile.cpp \
    src/Persistence/Files/MeshFile.cpp \
    src/Persistence/Files/Texture2DAssetFile.cpp \
    src/Persistence/Behaviours/BehaviourManager.cpp \
    src/Persistence/Behaviours/BehaviourRefresherTimer.cpp \
    src/Persistence/IO/FileWriter.cpp \
    src/Persistence/XML/XMLAttribute.cpp \
    src/Persistence/IO/stb_image.cpp \
    src/Persistence/XML/XMLNode.cpp \
    src/Persistence/XML/XMLParser.cpp \
    src/Persistence/XML/XMLProperty.cpp \
    src/View/Windows/IWindow.cpp \
    src/View/Windows/GameWindow.cpp \
    src/Persistence/Files/AudioFile.cpp \
    src/Domain/Engine/Physics/Math.cpp \
    src/Domain/Engine/Physics/Random.cpp \
    src/Domain/Engine/Components/AudioListener.cpp \
    src/Persistence/Behaviours/BehaviourManagerStatus.cpp \
    src/Domain/Engine/Components/RectTransform.cpp \
    src/View/Cursor.cpp \
    src/Domain/Graphics/RenderTexture.cpp \
    src/Domain/Engine/Object.cpp \
    src/Domain/Engine/Others/GraphicPipelineDebugger.cpp \
    src/Domain/Engine/GraphicPipeline/GraphicPipeline.cpp \
    src/Domain/Engine/GraphicPipeline/GraphicPipelinePass.cpp \
    src/Domain/Engine/GraphicPipeline/GPPass_DepthLayer.cpp \
    src/Domain/Engine/GraphicPipeline/GPPass_SP_DeferredLights.cpp \
    src/Domain/Engine/GraphicPipeline/GPPass_G_Gizmos.cpp \
    src/Domain/Graphics/GLContext.cpp \
    src/Domain/Graphics/GL.cpp \
    src/Domain/Engine/DataStructures/TypeMap.cpp \
    src/Domain/Engine/CodePreprocessor.cpp \
    src/Domain/Graphics/Shaders/ShaderPreprocessor.cpp \
    src/Persistence/Project/ProjectManager.cpp \
    src/Persistence/Project/Project.cpp \
    src/Persistence/Behaviours/BehaviourObjectCompileRunnable.cpp \
    src/Persistence/Behaviours/BehaviourMergeObjectsRunnable.cpp \
    src/Domain/Engine/GraphicPipeline/GPPass_G.cpp

EDITOR {
    SOURCES += \
        src/View/EditorGameObject/EditorGizmosGameObject.cpp \
        src/Domain/Engine/GraphicPipeline/GPPass_Selection.cpp \
        src/View/GameBuilderJob.cpp \
        src/View/Dialogs/GameBuildDialog.cpp \
        src/View/Explorer/FileSystemModel.cpp \
        src/Persistence/Project/QtProjectManager.cpp \
        src/View/EditorState.cpp \
        src/View/DragDrop/DragDropManager.cpp \
        src/View/DragDrop/DragDropQWidget.cpp \
        src/View/Explorer/FileReferencesManager.cpp \
        src/View/DragDrop/DragDropQListView.cpp \
        src/View/DragDrop/DragDropQTreeWidget.cpp \
        src/View/DragDrop/DragDropQListWidget.cpp \
        src/View/DragDrop/DragDropAgent.cpp \
        src/View/DragDrop/IDragDropListener.cpp \
        src/Domain/Graphics/Buffers/SelectionFramebuffer.cpp \
        src/View/EditorPlayStopFlowController.cpp \
        src/View/EditorGameObject/EditorRotateAxisGroup.cpp \
        src/Persistence/Files/AudioFileInspectable.cpp \
        src/View/Windows/EditorWindow.cpp \
        src/View/Dialogs/DialogBrowseAssetFile.cpp \
        src/View/Windows/SelectProjectWindow.cpp \
        src/View/Windows/WindowEventManager.cpp \
        src/View/EditorGameObject/EditorRotateAxis.cpp \
        src/View/EditorGameObject/EditorAxisGroup.cpp \
        src/View/EditorGameObject/EditorAxis.cpp \
        src/View/EditorGameObject/EditorScaleAxisGroup.cpp \
        src/View/Inspector/AttributeWidgets/IAttrWidgetButtonListener.cpp \
        src/View/Inspector/Inspectables/AudioClipAssetFileInspectable.cpp \
        src/View/EditorGameObject/EditorScaleAxis.cpp \
        src/View/EditorScene/EditorFloor.cpp \
        src/View/Interfaces/IWindowEventManagerListener.cpp \
        src/View/Inspector/Inspector.cpp \
        src/View/Hierarchy/Hierarchy.cpp \
        src/View/Inspector/ComponentWidget.cpp \
        src/View/Inspector/AttributeWidgets/AttributeWidget.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetFloat.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetVectorFloat.cpp \
        src/View/Console/Console.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetEnum.cpp \
        src/View/Explorer/Explorer.cpp \
        src/View/Inspector/Inspectables/IInspectable.cpp \
        src/View/MenuBar.cpp \
        src/View/Dialogs/Dialog.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetFile.cpp \
        src/View/Explorer/ExplorerDirTree.cpp \
        src/View/Inspector/InspectorWidget.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetString.cpp \
        src/View/EditorCamera/EditorCamera.cpp \
        src/View/EditorScene/EditorScene.cpp \
        src/View/Inspector/Inspectables/MeshAssetFileInspectable.cpp \
        src/View/Inspector/Inspectables/Texture2DAssetFileInspectable.cpp \
        src/View/Inspector/Inspectables/ImageFileInspectable.cpp \
        src/View/Inspector/Inspectables/MeshFileInspectable.cpp \
        src/View/Inspector/Inspectables/MaterialAssetFileInspectable.cpp \
        src/View/EditorGameObject/EditorBBox.cpp \
        src/View/EditorGameObject/EditorSelectionGameObject.cpp \
        src/View/EditorScene/EditorDebugGameObject.cpp \
        src/View/EditorGameObject/EditorTranslateAxisGroup.cpp \
        src/View/EditorGameObject/EditorTranslateAxis.cpp \
        src/View/Toolbar.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetButton.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetBool.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetColor.cpp \
        src/View/Inspector/Inspectables/TextFileInspectable.cpp \
        src/View/Inspector/Inspectables/PrefabAssetFileInspectable.cpp \
        src/View/Inspector/ComponentClipboard.cpp \
        src/View/Hierarchy/HierarchyDragDropManager.cpp \
        src/View/Hierarchy/HierarchyContextMenu.cpp \
        src/View/Explorer/ExplorerContextMenu.cpp \
        src/View/Inspector/ComponentWidgetContextMenu.cpp \
        src/View/Inspector/InspectorContextMenu.cpp \
        src/Domain/Engine/Others/GameBuilder.cpp \
        src/View/ContextMenu.cpp \
        src/View/ShortcutManager.cpp \
        src/View/IShortcutListener.cpp \
        src/View/Explorer/ExplorerFileSortProxy.cpp \
        src/View/Inspector/AttributeWidgets/AttrWidgetInt.cpp \
        src/View/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.cpp \
        src/View/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.cpp \
        src/View/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.cpp \
        src/View/EditorGameObject/EditorGizmo.cpp \
        src/View/EditorGameObject/EditorAxisPlane.cpp
}
GAME {
    #SOURCES +=
}
######################################


# OTHER FILES #############
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
    Assets/Engine/Shaders/SP_DirectionalLight.frag \
    Assets/Engine/Shaders/G_Default.frag \
    Assets/Engine/Shaders/G_Default.vert \
    Assets/Materials/luigiMaterial.bmat \
    Assets/Materials/testMaterial.bmat \
    Assets/Materials/testMaterial2.bmat \
    Assets/Engine/Shaders/SelectionBuffer.frag \
    Assets/Engine/Shaders/RenderGBufferToScreen.frag \
    Assets/Engine/Shaders/SP_PointLight.frag \
    Assets/Scenes/tmp.bscene \
    Assets/Scenes/test2.bscene \
    Assets/Engine/Materials/G_Default.bmat \
    Assets/Engine/Materials/SP_EditorEffects.bmat \
    Assets/Engine/Materials/SP_DirectionalLight.bmat \
    Assets/Engine/Materials/SP_PointLight.bmat \
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
    Assets/Engine/Shaders/G.vert \
    Assets/Engine/Shaders/SP.vert \
    Assets/Engine/Shaders/SP.frag \
    Assets/Engine/Shaders/Include/G.frag \
    Assets/Engine/Shaders/Include/SP.frag \
    Assets/Engine/Shaders/Include/G.vert \
    Assets/Engine/Shaders/Include/SP.vert \
    Assets/Engine/Shaders/Include/Uniforms.glsl \
    Assets/Engine/Shaders/Include/PointLight.glsl \
    Assets/Engine/Shaders/Include/DirectionalLight.glsl \
    Assets/Engine/Shaders/SP_MeshPass.vert \
    Assets/Engine/Materials/SP_DirectionalLight_Screen.bmat \
    Assets/Engine/Materials/SP_PointLight_Screen.bmat \
    Assets/Engine/Shaders/UI/SP_UIImage.frag \
    Assets/Engine/Materials/UI/SP_UIImage.bmat \
    Assets/Engine/Shaders/SP_RenderBackground.frag \
    Assets/Engine/Shaders/Include/Main.glsl \
    Assets/Engine/Shaders/SelectionBuffer.vert \
    Assets/Engine/Shaders/SP_SelectionEffect.frag \
    Assets/Engine/Shaders/G_RotationAxisLine.frag \
    Assets/Engine/Materials/G_RotationAxisLine.bmat \
    Assets/Engine/Materials/SP_MeshPass.bmat \
    Assets/Engine/Materials/RenderGBufferToScreen.bmat \
    Assets/Engine/Shaders/SP_ScreenPass.vert \
    Assets/Engine/Shaders/SP_MeshPass.vert \
    Assets/Engine/Materials/SP_SelectionEffect.bmat \
    Assets/Engine/Shaders/UI/G_UIText.frag \
    Assets/Engine/Materials/UI/G_UIText.bmat \
    EngineAssets/Shaders/SelectionBuffer.frag \
    EngineAssets/Shaders/RenderGBufferToScreen.frag \
    EngineAssets/Shaders/G_Template.frag \
    EngineAssets/Shaders/G_RotationAxisLine.frag \
    EngineAssets/Shaders/G_Default.frag \
    EngineAssets/Shaders/SelectionBuffer.vert \
    EngineAssets/Shaders/G_Template.vert \
    EngineAssets/Shaders/G_Default.vert \
    EngineAssets/Shaders/UI/SP_UIImage.frag \
    EngineAssets/Shaders/UI/G_UIText.frag \
    EngineAssets/Shaders/Include/Uniforms.glsl \
    EngineAssets/Shaders/Include/PointLight.glsl \
    EngineAssets/Shaders/Include/Main.glsl \
    EngineAssets/Shaders/Include/DirectionalLight.glsl \
    EngineAssets/Shaders/Include/G.frag \
    EngineAssets/Shaders/Include/G.vert \
    EngineAssets/Materials/RenderGBufferToScreen.bmat \
    EngineAssets/Materials/G_RotationAxisLine.bmat \
    EngineAssets/Materials/G_Default.bmat
######################################

DISTFILES += \
    EngineAssets/Materials/UI/G_UIImage.bmat \
    EngineAssets/Shaders/G_Missing.frag \
    EngineAssets/Materials/Missing.bmat \
    EngineAssets/Materials/SP_AmbientLight_Screen.bmat \
    EngineAssets/Materials/SP_SelectionEffect.bmat \
    EngineAssets/Materials/SP_PointLight_Screen.bmat \
    EngineAssets/Materials/SP_DirectionalLight_Screen.bmat \
    EngineAssets/Shaders/Include/SP.frag \
    EngineAssets/Shaders/Include/SP.vert \
    EngineAssets/Shaders/SP_DirectionalLight.frag \
    EngineAssets/Shaders/SP_ScreenPass.vert \
    EngineAssets/Shaders/SP_PointLight.frag \
    EngineAssets/Shaders/SP_SelectionEffect.frag \
    EngineAssets/Shaders/SP_Template.frag \
    EngineAssets/Materials/G_DefaultNoSP.bmat \
    EngineAssets/Materials/UI/G_UIImage.bmat \
    EngineAssets/Materials/UI/G_UIText.bmat \
    EngineAssets/Materials/UI/SP_UIImage.bmat
