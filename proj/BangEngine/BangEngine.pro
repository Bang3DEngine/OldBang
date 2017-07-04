include(../BangCommon.pri)
system(rm -f Makefile)
system(cd $$PWD/../.. ; sh ./scripts/preprocessHeaders.sh)

TEMPLATE = lib
INCLUDEPATH += $$PWD/../../include
QT += core gui opengl widgets

CONFIG += staticlib

TARGET = $$BIN_DIR/lib/BangEngine

MOC_DIR = $$PWD/../../bin/Qt_MOCs/

UI_DIR  = $$PWD/../../include/Bang

EDITOR {
    FORMS   += $$PWD/../../res/EngineAssets/EditorWindow.ui
    FORMS   += $$PWD/../../res/EngineAssets/SelectProjectWindow.ui
}

SOURCES += \
    $$PWD/../../src/Engine/General/Time.cpp \
    $$PWD/../../src/Engine/Math/AABox.cpp \
    $$PWD/../../src/Engine/Math/Sphere.cpp \
    $$PWD/../../src/Engine/GameObjects/GameObject.cpp \
    $$PWD/../../src/Engine/GameObjects/Scene.cpp \
    $$PWD/../../src/Engine/Components/Transform.cpp \
    $$PWD/../../src/Engine/Components/Component.cpp \
    $$PWD/../../src/Engine/Components/MeshRenderer.cpp \
    $$PWD/../../src/Engine/Components/Camera.cpp \
    $$PWD/../../src/Engine/Assets/Asset.cpp \
    $$PWD/../../src/Engine/Assets/Mesh.cpp \
    $$PWD/../../src/Engine/Assets/Material.cpp \
    $$PWD/../../src/Engine/Components/Behaviour.cpp \
    $$PWD/../../src/Engine/Assets/Texture2D.cpp \
    $$PWD/../../src/Engine/Assets/Prefab.cpp \
    $$PWD/../../src/Engine/IO/General/AssetsManager.cpp \
    $$PWD/../../src/Engine/General/Input.cpp \
    $$PWD/../../src/Engine/IO/Files/TextFile.cpp \
    $$PWD/../../src/Engine/Components/LineRenderer.cpp \
    $$PWD/../../src/Engine/Components/Renderer.cpp \
    $$PWD/../../src/Engine/General/SystemUtils.cpp \
    $$PWD/../../src/Engine/General/SingletonManager.cpp \
    $$PWD/../../src/Engine/Components/CircleRenderer.cpp \
    $$PWD/../../src/Engine/Components/SingleLineRenderer.cpp \
    $$PWD/../../src/Engine/Assets/MeshFactory.cpp \
    $$PWD/../../src/Engine/General/ICloneable.cpp \
    $$PWD/../../src/Engine/Components/DirectionalLight.cpp \
    $$PWD/../../src/Engine/Components/Light.cpp \
    $$PWD/../../src/Engine/Components/PointLight.cpp \
    $$PWD/../../src/Engine/General/SceneManager.cpp \
    $$PWD/../../src/Engine/Components/Canvas.cpp \
    $$PWD/../../src/Engine/Components/UIImage.cpp \
    $$PWD/../../src/Engine/Components/UIRenderer.cpp \
    $$PWD/../../src/Engine/Assets/Font.cpp \
    $$PWD/../../src/Engine/Components/UIText.cpp \
    $$PWD/../../src/Engine/IO/General/EngineConfig.cpp \
    $$PWD/../../src/Engine/Assets/AudioClip.cpp \
    $$PWD/../../src/Engine/Components/AudioSource.cpp \
    $$PWD/../../src/Engine/Audio/AudioManager.cpp \
    $$PWD/../../src/Engine/Audio/AudioPlayerRunnable.cpp \
    $$PWD/../../src/Engine/Debug/Gizmos.cpp \
    $$PWD/../../src/Engine/Debug/Debug.cpp \
    $$PWD/../../src/Engine/Debug/Chrono.cpp \
    $$PWD/../../src/Engine/IO/Files/FileReader.cpp \
    $$PWD/../../src/Engine/IO/Files/ImageFile.cpp \
    $$PWD/../../src/Engine/IO/Behaviours/BehaviourManager.cpp \
    $$PWD/../../src/Engine/IO/XML/XMLAttribute.cpp \
    $$PWD/../../src/Engine/IO/XML/XMLNode.cpp \
    $$PWD/../../src/Engine/IO/XML/XMLParser.cpp \
    $$PWD/../../src/Engine/IO/XML/XMLProperty.cpp \
    $$PWD/../../src/Engine/Components/AudioListener.cpp \
    $$PWD/../../src/Engine/IO/Behaviours/BehaviourManagerStatus.cpp \
    $$PWD/../../src/Engine/Components/RectTransform.cpp \
    $$PWD/../../src/Engine/General/Object.cpp \
    $$PWD/../../src/Engine/Debug/GraphicPipelineDebugger.cpp \
    $$PWD/../../src/Engine/Compiler/CodePreprocessor.cpp \
    $$PWD/../../src/Engine/IO/Project/ProjectManager.cpp \
    $$PWD/../../src/Engine/IO/Project/Project.cpp \
    $$PWD/../../src/Engine/Debug/ChronoGL.cpp \
    $$PWD/../../src/Engine/IO/General/SerializableObject.cpp \
    $$PWD/../../src/Engine/IO/Files/ExtensionManager.cpp \
    $$PWD/../../src/Engine/Components/CircleCulledRenderer.cpp \
    $$PWD/../../src/Engine/IO/Files/PrefabFile.cpp \
    $$PWD/../../src/Engine/IO/Files/SoundFile.cpp \
    $$PWD/../../src/Engine/Audio/AnonymousAudioPlayer.cpp \
    $$PWD/../../src/Engine/Components/PostProcessEffect.cpp \
    $$PWD/../../src/Engine/IO/Files/ModelFile.cpp \
    $$PWD/../../src/Engine/IO/Files/MeshFile.cpp \
    $$PWD/../../src/Engine/IO/Files/Texture2DFile.cpp \
    $$PWD/../../src/Engine/IO/Files/ShaderProgramFile.cpp \
    $$PWD/../../src/Engine/IO/Files/MaterialFile.cpp \
    $$PWD/../../src/Engine/IO/Files/AudioClipFile.cpp \
    $$PWD/../../src/Engine/IO/Files/FontFile.cpp \
    $$PWD/../../src/Engine/IO/General/Paths.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BangPreprocessor.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.cpp \
    $$PWD/../../src/Engine/Compiler/Compiler.cpp \
    $$PWD/../../src/Engine/Compiler/BangCompiler.cpp \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.cpp \
    $$PWD/../../src/Engine/IO/Files/BFile.cpp \
    $$PWD/../../src/Engine/Compiler/BangCompilerAsyncJob.cpp

HEADERS += \
    $$PWD/../../src/Engine/General/IToString.h \
    $$PWD/../../src/Engine/General/ISceneEventListener.h \
    $$PWD/../../src/Engine/GameObjects/GameObject.h \
    $$PWD/../../src/Engine/GameObjects/Scene.h \
    $$PWD/../../src/Engine/Components/Transform.h \
    $$PWD/../../src/Engine/Components/Component.h \
    $$PWD/../../src/Engine/Components/MeshRenderer.h \
    $$PWD/../../src/Engine/Components/Camera.h \
    $$PWD/../../src/Engine/Assets/Asset.h \
    $$PWD/../../src/Engine/Assets/Material.h \
    $$PWD/../../src/Engine/Assets/Mesh.h \
    $$PWD/../../src/Engine/Components/Behaviour.h \
    $$PWD/../../src/Engine/Assets/Texture2D.h \
    $$PWD/../../src/Engine/Assets/Prefab.h \
    $$PWD/../../include/Bang/Bang.h \
    $$PWD/../../src/Engine/IO/General/AssetsManager.h \
    $$PWD/../../src/Engine/General/Input.h \
    $$PWD/../../src/Engine/Components/LineRenderer.h \
    $$PWD/../../src/Engine/Components/Renderer.h \
    $$PWD/../../src/Engine/Math/Sphere.h \
    $$PWD/../../src/Engine/General/SystemUtils.h \
    $$PWD/../../src/Engine/General/SingletonManager.h \
    $$PWD/../../src/Engine/Components/CircleRenderer.h \
    $$PWD/../../src/Engine/Components/SingleLineRenderer.h \
    $$PWD/../../src/Engine/Assets/MeshFactory.h \
    $$PWD/../../src/Engine/General/ICloneable.h \
    $$PWD/../../src/Engine/Components/DirectionalLight.h \
    $$PWD/../../src/Engine/Components/Light.h \
    $$PWD/../../src/Engine/Components/PointLight.h \
    $$PWD/../../src/Engine/General/SceneManager.h \
    $$PWD/../../src/Engine/Components/Canvas.h \
    $$PWD/../../src/Engine/Components/UIImage.h \
    $$PWD/../../src/Engine/Components/UIRenderer.h \
    $$PWD/../../src/Engine/Assets/Font.h \
    $$PWD/../../src/Engine/Components/UIText.h \
    $$PWD/../../src/Engine/IO/General/EngineConfig.h \
    $$PWD/../../src/Engine/Math/AABox.h \
    $$PWD/../../src/Engine/Assets/AudioClip.h \
    $$PWD/../../src/Engine/Components/AudioSource.h \
    $$PWD/../../src/Engine/Audio/AudioManager.h \
    $$PWD/../../src/Engine/Audio/AudioPlayerRunnable.h \
    $$PWD/../../src/Engine/Debug/Chrono.h \
    $$PWD/../../src/Engine/Debug/Debug.h \
    $$PWD/../../src/Engine/Debug/Gizmos.h \
    $$PWD/../../src/Engine/General/Time.h \
    $$PWD/../../src/Engine/IO/Behaviours/BehaviourManager.h \
    $$PWD/../../src/Engine/IO/Files/FileReader.h \
    $$PWD/../../src/Engine/IO/Files/ImageFile.h \
    $$PWD/../../src/Engine/IO/XML/XMLAttribute.h \
    $$PWD/../../src/Engine/IO/XML/XMLNode.h \
    $$PWD/../../src/Engine/IO/XML/XMLParser.h \
    $$PWD/../../src/Engine/IO/XML/XMLProperty.h \
    $$PWD/../../src/Engine/IO/Files/TextFile.h \
    $$PWD/../../src/Engine/IO/Files/ExtensionManager.h \
    $$PWD/../../src/Engine/Components/AudioListener.h \
    $$PWD/../../src/Engine/IO/Behaviours/BehaviourManagerStatus.h \
    $$PWD/../../src/Engine/Components/RectTransform.h \
    $$PWD/../../src/Engine/Debug/GraphicPipelineDebugger.h \
    $$PWD/../../src/Engine/General/Object.h \
    $$PWD/../../src/Engine/Compiler/CodePreprocessor.h \
    $$PWD/../../src/Engine/IO/Project/Project.h \
    $$PWD/../../src/Engine/IO/Project/ProjectManager.h \
    $$PWD/../../src/Engine/Debug/ChronoGL.h \
    $$PWD/../../src/Engine/IO/General/SerializableObject.h \
    $$PWD/../../src/Engine/Components/CircleCulledRenderer.h \
    $$PWD/../../src/Engine/IO/Files/PrefabFile.h \
    $$PWD/../../src/Engine/IO/Files/SoundFile.h \
    $$PWD/../../src/Engine/Audio/AnonymousAudioPlayer.h \
    $$PWD/../../src/Engine/DataStructures/Flags.h \
    $$PWD/../../src/Engine/Components/PostProcessEffect.h \
    $$PWD/../../src/Engine/IO/Files/ModelFile.h \
    $$PWD/../../src/Engine/IO/Files/MeshFile.h \
    $$PWD/../../src/Engine/DataStructures/Regex.h \
    $$PWD/../../src/Engine/IO/Files/AudioClipFile.h \
    $$PWD/../../src/Engine/IO/Files/FontFile.h \
    $$PWD/../../src/Engine/IO/Files/MaterialFile.h \
    $$PWD/../../src/Engine/IO/Files/ShaderProgramFile.h \
    $$PWD/../../src/Engine/IO/Files/Texture2DFile.h \
    $$PWD/../../src/Engine/IO/General/Paths.h \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BangPreprocessor.h \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.h \
    $$PWD/../../src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.h \
    $$PWD/../../src/Engine/IO/Files/BFile.h \
    $$PWD/../../src/Engine/Compiler/Compiler.h \
    $$PWD/../../src/Engine/Compiler/BangCompiler.h \
    $$PWD/../../src/Engine/Compiler/BangCompilerAsyncJob.h

EDITOR {
HEADERS += \
    $$PWD/../../src/Engine/IO/Files/AssetFileCreator.h \
    $$PWD/../../src/Engine/IO/Project/QtProjectManager.h \
    $$PWD/../../src/Engine/IO/Behaviours/BehaviourRefresherTimer.h \
    $$PWD/../../src/Engine/IO/Files/FileTracker.h
}

EDITOR {
    SOURCES += \
        $$PWD/../../src/Engine/IO/Files/AssetFileCreator.cpp \
        $$PWD/../../src/Engine/IO/Project/QtProjectManager.cpp \
        $$PWD/../../src/Engine/IO/Behaviours/BehaviourRefresherTimer.cpp \
        $$PWD/../../src/Engine/IO/Files/FileTracker.cpp
}
######################################


# OTHER FILES #############
DISTFILES += \
    ../.gitignore \
    $$PWD/../../src/Engine \
    $$PWD/../../src/res/EngineAssets/Shaders/SelectionBuffer.frag_sel \
    $$PWD/../../src/res/EngineAssets/Shaders/RenderGBufferToScreen.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Template.frag_g \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Default.frag_g \
    $$PWD/../../src/res/EngineAssets/Shaders/SelectionBuffer.vert_sel \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Template.vert_g \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Default.vert_g \
    $$PWD/../../src/res/EngineAssets/Shaders/UI/SP_UIImage.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/PointLight.glsl \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/Main.glsl \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/DirectionalLight.glsl \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/G.frag_g \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/G.vert_g \
    $$PWD/../../src/res/EngineAssets/Materials/RenderGBufferToScreen.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/G_Default.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/UI/G_UIImage.bmat \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Missing.frag \
    $$PWD/../../src/res/EngineAssets/Materials/Missing.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/SP_AmbientLight_Screen.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/SP_SelectionEffect.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/SP_PointLight_Screen.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/SP_DirectionalLight_Screen.bmat \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/SP.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/SP.vert_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_DirectionalLight.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_ScreenPass.vert_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_PointLight.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_SelectionEffect.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_Template.frag_pp \
    $$PWD/../../src/res/EngineAssets/Materials/G_DefaultNoSP.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/UI/G_UIImage.bmat \
    $$PWD/../../src/res/EngineAssets/Materials/UI/SP_UIImage.bmat \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_FXAA.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/Include/Common.glsl \
    $$PWD/../../src/res/EngineAssets/Fonts/GreatFont.bfont \
    $$PWD/../../src/res/EngineAssets/Fonts/HandwrittenFont.bfont \
    $$PWD/../../src/res/EngineAssets/Fonts/PrettyFont.bfont \
    $$PWD/../../src/res/EngineAssets/Fonts/UbuntuFont.bfont \
    $$PWD/../../src/res/EngineAssets/Camera.bmesh \
    $$PWD/../../src/res/EngineAssets/Cone.bmesh \
    $$PWD/../../src/res/EngineAssets/Cube.bmesh \
    $$PWD/../../src/res/EngineAssets/Plane.bmesh \
    $$PWD/../../src/res/EngineAssets/Pyramid.bmesh \
    $$PWD/../../src/res/EngineAssets/Sphere.bmesh \
    $$PWD/../../src/res/EngineAssets/UIPlane.bmesh \
    $$PWD/../../src/res/EngineAssets/Textures/AudioSourceIcon.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/CameraIcon.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/CircleIcon.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/DefaultUIImageTexture.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/DirectionalLightIcon.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/PointLightIcon.btex2d \
    $$PWD/../../src/res/EngineAssets/Textures/RectTransformAnchor.btex2d \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_Default.vert_pp \
    $$PWD/../../src/res/EngineAssets/Materials/UI/G_UIText_Pass2.bmat \
    $$PWD/../../src/res/EngineAssets/Shaders/UI/G_UIText_Pass2.frag_g \
    $$PWD/../../src/res/EngineAssets/Materials/UI/G_UIText.bmat \
    $$PWD/../../src/res/EngineAssets/Shaders/UI/G_UIText.frag_g \
    $$PWD/../../src/res/EngineAssets/Shaders/SP_Default.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Default.bshaderprogram \
    $$PWD/../../src/res/EngineAssets/Shaders/Cartoon.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/G_Missing.frag_g \
    $$PWD/../../src/res/EngineAssets/Shaders/InvertColors.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/Blur.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/GrayScale.frag_pp \
    $$PWD/../../src/res/EngineAssets/Shaders/Outline.frag_pp
