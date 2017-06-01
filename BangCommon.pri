TEMPLATE = lib
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
MOC_DIR = ../src/Qt_MOCs/
UI_DIR  = ../include/Bang
EDITOR {
    FORMS   += ../EngineAssets/EditorWindow.ui
    FORMS   += ../EngineAssets/SelectProjectWindow.ui
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
#OTHER_OPTS = -Wall --std=c++14 -Wl,--export-dynamic -Wno-unused-parameter \
             #-Wunused-variable -Wswitch -Wno-sign-compare -fPIC
OTHER_OPTS = -Wall --std=c++14 -Wl,--export-dynamic -Wno-unused-parameter \
             -Wno-sign-compare -Wswitch -fPIC
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
    RESOURCES += ../EngineAssets/qdarkstyle/style.qrc
    RESOURCES += ../EngineAssets/QtResources.qrc
    TARGET = ../bin/Bang
    DEBUG_MODE { OBJECTS_DIR = ../bin/objEditorDebug/ }
    RELEASE_MODE { OBJECTS_DIR = ../bin/objEditorRelease/ }
}

GAME {
    TARGET = ../bin/Game.exe
    DEBUG_MODE { OBJECTS_DIR = ../bin/objGameDebug/ }
    RELEASE_MODE { OBJECTS_DIR = ../bin/objGameRelease/ }
}
###################################################




# LIBS ###########################
#!exists (../src/Domain/Graphics/FreeType/build) {
#    FreeTypeTarget.commands += && sh autogen.sh \
#                               && mkdir build ; cd build \
#                               && cmake .. && ../configure
#}
#FreeTypeTarget.commands = cd ../src/Domain/Graphics/FreeType && \
#                          rm -rf build ; \
#                          sh autogen.sh && \
#                          mkdir build ; \
#                          cd build && \
#                          #../configure && \
#                          cmake .. && make
#
#QMAKE_EXTRA_TARGETS += FreeTypeTarget
#PRE_TARGETDEPS = FreeTypeTarget
#LIBS += ../src/Domain/Graphics/FreeType/build/*.o
LIBS += \
    -L../bin \
    -lGLEW            \ # GLEW
    -lGLU             \
    -ldl              \ # To load libraries and stuff
    -lalut -lopenal   \ # Alut and OpenAL
    -lassimp          \
    -lfreetype
########################################


# INCLUDE PATH ###################
INCLUDEPATH +=                       \
    /usr/include                     \
    ../include                       \
    ../include/freetype
######################################

# HEADERS ##################
HEADERS +=                                                          \
    ../src/Domain/Engine/Physics/Rect.h                                \
    ../src/Domain/Interfaces/IToString.h                               \
    ../src/Domain/Engine/Interfaces/ISceneEventListener.h              \
    ../src/Domain/Engine/GameObjects/GameObject.h                      \
    ../src/Domain/Engine/GameObjects/Scene.h                           \
    ../src/Domain/Graphics/Texture.h                                   \
    ../src/Domain/Graphics/VAO.h                                       \
    ../src/Domain/Graphics/VBO.h                                       \
    ../src/Domain/Graphics/Interfaces/IToString.h                      \
    ../src/Domain/Engine/Components/Transform.h                        \
    ../src/Domain/Engine/Components/Component.h                        \
    ../src/Domain/Engine/Components/MeshRenderer.h                     \
    ../src/Domain/Engine/Components/Camera.h                           \
    ../src/Domain/Engine/Assets/Asset.h                                \
    ../src/Domain/Engine/Assets/Material.h                             \
    ../src/Domain/Engine/Assets/Meshes/Mesh.h                          \
    ../src/Domain/Engine/Components/Behaviour.h                        \
    ../src/Domain/Engine/Assets/Texture2D.h                            \
    ../src/Domain/Engine/Assets/Prefab.h                               \
    ../include/Bang/Bang.h                                             \
    ../src/Persistence/AssetsManager.h                                 \
    ../src/Domain/Engine/Others/Input.h                                \
    ../src/Domain/Engine/Components/LineRenderer.h                     \
    ../src/Domain/Engine/Components/Renderer.h                         \
    ../src/Domain/Engine/Physics/Vector3.h                             \
    ../src/Domain/Engine/Physics/Quaternion.h                          \
    ../src/Domain/Engine/Physics/Matrix4.h                             \
    ../src/Domain/Engine/Physics/Sphere.h                              \
    ../src/Domain/Engine/Others/SystemUtils.h                          \
    ../src/Domain/Engine/Others/SingletonManager.h                     \
    ../src/Domain/Engine/Components/CircleRenderer.h                   \
    ../src/Domain/Engine/Components/SingleLineRenderer.h               \
    ../src/Domain/Engine/Assets/Meshes/MeshFactory.h                   \
    ../src/Domain/Engine/Interfaces/ICloneable.h                       \
    ../src/View/Screen.h                                               \
    ../src/Domain/Engine/Components/DirectionalLight.h                 \
    ../src/Domain/Engine/Components/Light.h                            \
    ../src/View/GameObjectClipboard.h                                  \
    ../src/Domain/Engine/Components/PointLight.h                       \
    ../src/Domain/Engine/Physics/Vector4.h                             \
    ../src/Domain/Engine/Physics/Vector2.h                             \
    ../src/Domain/Engine/Physics/Color.h                               \
    ../src/View/Application.h                                          \
    ../src/Domain/Engine/Others/SceneManager.h                         \
    ../src/Domain/Engine/Components/Canvas.h                           \
    ../src/Domain/Engine/Components/UIImage.h                          \
    ../src/Domain/Engine/Components/UIRenderer.h                       \
    ../src/Domain/Graphics/FontSheetCreator.h                          \
    ../src/Domain/Engine/Assets/Font.h                                 \
    ../src/Domain/Engine/Components/UIText.h                           \
    ../src/Persistence/EngineConfig.h                                  \
    ../src/Domain/Engine/Physics/AABox.h                               \
    ../src/Domain/Engine/Assets/AudioClip.h                            \
    ../src/Domain/Engine/Components/AudioSource.h                      \
    ../src/Domain/Engine/Audio/AudioManager.h                          \
    ../src/Domain/Engine/Audio/AudioPlayerRunnable.h                   \
    ../src/Domain/Engine/DataStructures/Array.h                        \
    ../src/Domain/Engine/DataStructures/List.h                         \
    ../src/Domain/Engine/DataStructures/Map.h                          \
    ../src/Domain/Engine/DataStructures/Property.h                     \
    ../src/Domain/Engine/DataStructures/String.h                       \
    ../src/Domain/Engine/DataStructures/NamedEnum.h                    \
    ../src/Domain/Engine/Debug/Chrono.h                                \
    ../src/Domain/Engine/Debug/Debug.h                                 \
    ../src/Domain/Engine/Debug/Gizmos.h                                \
    ../src/Domain/Engine/Debug/Time.h                                  \
    ../src/Domain/Engine/DataStructures/Timer.h                        \
    ../src/Domain/Graphics/Buffers/Framebuffer.h                       \
    ../src/Domain/Graphics/Buffers/GBuffer.h                           \
    ../src/Domain/Graphics/Shaders/Shader.h                            \
    ../src/Domain/Graphics/Shaders/ShaderProgram.h                     \
    ../src/Persistence/Behaviours/BehaviourManager.h                   \
    ../src/Persistence/Behaviours/BehaviourRefresherTimer.h            \
    ../src/Persistence/Files/File.h                                    \
    ../src/Persistence/IO/FileReader.h                                 \
    ../src/Persistence/Files/ImageFile.h                               \
    ../src/Persistence/XML/XMLAttribute.h                              \
    ../src/Persistence/XML/XMLNode.h                                   \
    ../src/Persistence/XML/XMLParser.h                                 \
    ../src/Persistence/XML/XMLProperty.h                               \
    ../src/Persistence/Files/TextFile.h \
    ../src/View/Windows/IWindow.h \
    ../src/View/Windows/GameWindow.h \
    ../src/Domain/Engine/Physics/Math.h \
    ../src/Domain/Engine/Physics/Random.h \
    ../src/Domain/Engine/Components/AudioListener.h \
    ../src/Persistence/Behaviours/BehaviourManagerStatus.h \
    ../src/Domain/Engine/Components/RectTransform.h \
    ../src/View/Cursor.h \
    ../src/Domain/Graphics/RenderTexture.h \
    ../src/Domain/Engine/Object.h \
    ../src/Domain/Engine/Others/GraphicPipelineDebugger.h \
    ../src/Domain/Engine/GraphicPipeline/GraphicPipeline.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass_SP_DeferredLights.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass_G_Gizmos.h \
    ../src/Domain/Graphics/GLContext.h \
    ../src/Domain/Graphics/GL.h \
    ../src/Domain/Engine/DataStructures/TypeMap.h \
    ../src/Domain/Engine/CodePreprocessor.h \
    ../src/Domain/Graphics/Shaders/ShaderPreprocessor.h \
    ../src/Persistence/Project/Project.h \
    ../src/Persistence/Project/ProjectManager.h \
    ../src/Persistence/Behaviours/BehaviourObjectCompileRunnable.h \
    ../src/Persistence/Behaviours/BehaviourMergeObjectsRunnable.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass_G.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass.h \
    ../src/Domain/Graphics/Interfaces/GLObject.h \
    ../src/Domain/Engine/Debug/ChronoGL.h \
    ../src/Domain/Engine/DataStructures/Set.h \
    ../src/Domain/Engine/Interfaces/SerializableObject.h \
    ../src/Domain/Engine/GraphicPipeline/TextureUnitManager.h \
    ../src/Domain/Engine/DataStructures/UMap.h \
    ../src/View/Shortcut.h \
    ../src/View/IconManager.h \
    ../src/Domain/Engine/Image.h \
    ../src/Domain/Engine/Components/CircleCulledRenderer.h \
    ../src/Persistence/Files/PrefabFile.h \
    ../src/Persistence/Files/SoundFile.h \
    ../src/Domain/Engine/Audio/AnonymousAudioPlayer.h \
    ../src/Domain/Engine/DataStructures/Flags.h \
    ../src/Domain/Graphics/Shaders/ShaderManager.h \
    ../src/Domain/Engine/Components/PostProcessEffect.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass_SP_PostProcessEffects.h \
    ../src/Persistence/Files/Path.h \
    ../src/Persistence/Files/ModelFile.h \
    ../src/Persistence/Files/MeshFile.h \
    ../src/Domain/Engine/DataStructures/Regex.h \
    ../src/Persistence/Files/AudioClipFile.h \
    ../src/Persistence/Files/FontFile.h \
    ../src/Persistence/Files/MaterialFile.h \
    ../src/Persistence/Files/ShaderProgramFile.h \
    ../src/Persistence/Files/Texture2DFile.h \
    ../src/Persistence/Paths.h \
    ../EngineAssets/Shaders/G_Missing.frag_g \
    ../EngineAssets/Shaders/InvertColors.frag_pp \
    ../EngineAssets/Shaders/Blur.frag_pp \
    ../EngineAssets/Shaders/GrayScale.frag_pp \
    ../EngineAssets/Shaders/Outline.frag_pp \
    ../src/Domain/Engine/GraphicPipeline/GPPass_RenderLayer.h \
    ../src/Domain/Engine/DataStructures/Collection.h \
    ../src/Domain/BangPreprocessor/BangPreprocessor.h \
    ../src/Domain/BangPreprocessor/BPProperty.h \
    ../src/Domain/BangPreprocessor/BPStruct.h

EDITOR {
HEADERS += \
    ../src/View/Windows/EditorWindow.h \
    ../src/Domain/Engine/GraphicPipeline/GPPass_Selection.h \
    ../src/View/EditorGameObject/EditorGizmosGameObject.h \
    ../src/View/DragDrop/DragDropQWidget.h \
    ../src/Persistence/Files/AssetFileCreator.h \
    ../src/Persistence/Project/QtProjectManager.h \
    ../src/View/DragDrop/DragDropQListView.h \
    ../src/View/Explorer/FileReferencesManager.h \
    ../src/View/DragDrop/DragDropQTreeWidget.h \
    ../src/View/Inspector/Inspectables/Inspectable.h \
    ../src/View/Inspector/Inspectables/GameObjectInspectable.h \
    ../src/View/Inspector/Inspectables/IInspectable.h \
    ../src/View/Inspector/Inspectables/FileInspectable.h \
    ../src/View/Inspector/Inspectables/ComponentInspectable.h \
    ../src/View/DragDrop/DragDropQListWidget.h \
    ../src/View/DragDrop/DragDropAgent.h \
    ../src/View/DragDrop/IDragDropListener.h \
    ../src/View/DragDrop/DragDropManager.h \
    ../src/View/Explorer/FileSystemModel.h \
    ../src/View/EditorGameObject/EditorSelectionGameObject.h \
    ../src/View/EditorScene/EditorDebugGameObject.h \
    ../src/Domain/Graphics/Buffers/SelectionFramebuffer.h              \
    ../src/View/EditorGameObject/EditorTranslateAxis.h \
    ../src/View/EditorGameObject/EditorTranslateAxisGroup.h \
    ../src/View/Dialogs/DialogBrowseAssetFile.h                        \
    ../src/View/Inspector/AttributeWidgets/AttributeWidget.h           \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetBool.h            \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetButton.h          \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetColor.h           \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetEnum.h            \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetFile.h            \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetFloat.h           \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetString.h          \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetVectorFloat.h     \
    ../src/View/Inspector/AttributeWidgets/IAttrWidgetButtonListener.h \
    ../src/View/Inspector/Inspectables/PrefabFileInspectable.h    \
    ../src/View/Windows/WindowEventManager.h \
    ../src/View/Windows/SelectProjectWindow.h \
    ../src/View/Toolbar.h \
    ../src/View/EditorState.h \
    ../src/View/Dialogs/GameBuildDialog.h \
    ../src/View/GameBuilderJob.h \
    ../src/View/Dialogs/Dialog.h \
    ../src/View/Interfaces/IWindowEventManagerListener.h \
    ../src/View/Inspector/Inspector.h \
    ../src/View/Hierarchy/Hierarchy.h \
    ../src/View/Inspector/ComponentWidget.h \
    ../src/View/Console/Console.h \
    ../src/View/Explorer/Explorer.h \
    ../src/View/MenuBar.h \
    ../src/View/Explorer/ExplorerDirTree.h \
    ../src/View/Inspector/InspectorWidget.h \
    ../src/View/EditorCamera/EditorCamera.h \
    ../src/View/EditorScene/EditorScene.h \
    ../src/View/EditorGameObject/EditorBBox.h \
    ../src/View/EditorGameObject/EditorRotateAxisGroup.h \
    ../src/View/EditorGameObject/EditorRotateAxis.h \
    ../src/View/EditorGameObject/EditorAxisGroup.h \
    ../src/View/EditorGameObject/EditorAxis.h \
    ../src/View/EditorGameObject/EditorScaleAxisGroup.h \
    ../src/View/EditorGameObject/EditorScaleAxis.h \
    ../src/View/EditorScene/EditorFloor.h \
    ../src/View/Inspector/ComponentClipboard.h \
    ../src/View/Hierarchy/HierarchyDragDropManager.h \
    ../src/View/Hierarchy/HierarchyContextMenu.h \
    ../src/View/Explorer/ExplorerContextMenu.h \
    ../src/View/Inspector/ComponentWidgetContextMenu.h \
    ../src/View/Inspector/InspectorContextMenu.h \
    ../src/View/ContextMenu.h \
    ../src/Domain/Engine/Others/GameBuilder.h \
    ../src/View/EditorGameObject/EditorAxisPlane.h \
    ../src/View/Explorer/ExplorerFileSortProxy.h \
    ../src/View/Inspector/AttributeWidgets/AttrWidgetInt.h \
    ../src/View/EditorGameObject/EditorRectTransform/EditorRectTransformGizmo.h \
    ../src/View/EditorGameObject/EditorRectTransform/EditorRectTransformCornerGizmo.h \
    ../src/View/EditorGameObject/EditorRectTransform/EditorRectTransformAnchorGizmo.h \
    ../src/View/EditorGameObject/EditorGizmo.h \
    ../src/View/DragDrop/DragDropInfo.h \
    ../src/View/EditorPlayFlow.h \
    ../src/Persistence/FileTracker.h
}
GAME {
    HEADERS +=
}
######################################
