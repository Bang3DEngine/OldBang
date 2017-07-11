include(../BangCommon.pri)
system(rm -f Makefile)
system(cd $$BANG_ROOT ; sh ./scripts/preprocessHeaders.sh)

TEMPLATE = lib
CONFIG += staticlib
QT += core gui opengl widgets

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics

TARGET = $$BIN_DIR/lib/BangEngine

MOC_DIR = $$BANG_ROOT/bin/Qt_MOCs/

UI_DIR  = $$BANG_ROOT/include/Bang/UI_Qt
EDITOR {
    FORMS   += $$BANG_ROOT/res/EngineAssets/EditorWindow.ui
    FORMS   += $$BANG_ROOT/res/EngineAssets/SelectProjectWindow.ui
}

SOURCES += \
    $$BANG_ROOT/src/Engine/General/Time.cpp \
    $$BANG_ROOT/src/Editor/General/Cursor.cpp \
    $$BANG_ROOT/src/Editor/Graphics/ShaderManager.cpp \
    $$BANG_ROOT/src/Editor/General/Application.cpp \
    $$BANG_ROOT/src/Editor/Windows/GameWindow.cpp \
    $$BANG_ROOT/src/Editor/Windows/IWindow.cpp \
    $$BANG_ROOT/src/Engine/Math/AABox.cpp \
    $$BANG_ROOT/src/Engine/Math/Sphere.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/Shader.cpp \
    $$BANG_ROOT/src/Engine/GameObjects/GameObject.cpp \
    $$BANG_ROOT/src/Engine/GameObjects/Scene.cpp \
    $$BANG_ROOT/src/Engine/Components/Transform.cpp \
    $$BANG_ROOT/src/Engine/Components/Component.cpp \
    $$BANG_ROOT/src/Engine/Components/MeshRenderer.cpp \
    $$BANG_ROOT/src/Engine/Components/Camera.cpp \
    $$BANG_ROOT/src/Engine/Assets/Asset.cpp \
    $$BANG_ROOT/src/Engine/Assets/Mesh.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/Screen.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GraphicPipeline.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_SP_DeferredLights.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_G_Gizmos.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_G.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_SP_PostProcessEffects.cpp \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_RenderLayer.cpp \
    $$BANG_ROOT/src/Engine/Components/Behaviour.cpp \
    $$BANG_ROOT/src/Engine/Assets/Texture2D.cpp \
    $$BANG_ROOT/src/Engine/Assets/Prefab.cpp \
    $$BANG_ROOT/src/Engine/Assets/ShaderProgram.cpp \
    $$BANG_ROOT/src/Engine/IO/General/AssetsManager.cpp \
    $$BANG_ROOT/src/Engine/General/Input.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/TextFile.cpp \
    $$BANG_ROOT/src/Engine/Assets/Material.cpp \
    $$BANG_ROOT/src/Engine/Components/LineRenderer.cpp \
    $$BANG_ROOT/src/Engine/Components/Renderer.cpp \
    $$BANG_ROOT/src/Engine/General/SystemUtils.cpp \
    $$BANG_ROOT/src/Engine/General/SingletonManager.cpp \
    $$BANG_ROOT/src/Engine/Components/CircleRenderer.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/ShaderPreprocessor.cpp \
    $$BANG_ROOT/src/Engine/Components/SingleLineRenderer.cpp \
    $$BANG_ROOT/src/Engine/Assets/MeshFactory.cpp \
    $$BANG_ROOT/src/Engine/General/ICloneable.cpp \
    $$BANG_ROOT/src/Engine/Components/DirectionalLight.cpp \
    $$BANG_ROOT/src/Engine/Components/Light.cpp \
    $$BANG_ROOT/src/Engine/Components/PointLight.cpp \
    $$BANG_ROOT/src/Engine/General/SceneManager.cpp \
    $$BANG_ROOT/src/Engine/Components/Canvas.cpp \
    $$BANG_ROOT/src/Engine/Components/UIImage.cpp \
    $$BANG_ROOT/src/Engine/Components/UIRenderer.cpp \
    $$BANG_ROOT/src/Engine/Assets/Font.cpp \
    $$BANG_ROOT/src/Engine/Components/UIText.cpp \
    $$BANG_ROOT/src/Engine/IO/General/EngineConfig.cpp \
    $$BANG_ROOT/src/Engine/Assets/AudioClip.cpp \
    $$BANG_ROOT/src/Engine/Components/AudioSource.cpp \
    $$BANG_ROOT/src/Engine/Audio/AudioManager.cpp \
    $$BANG_ROOT/src/Engine/Audio/AudioPlayerRunnable.cpp \
    $$BANG_ROOT/src/Engine/Debug/Gizmos.cpp \
    $$BANG_ROOT/src/Engine/Debug/Debug.cpp \
    $$BANG_ROOT/src/Engine/Debug/Chrono.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/FileReader.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/ImageFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourManager.cpp \
    $$BANG_ROOT/src/Engine/IO/XML/XMLAttribute.cpp \
    $$BANG_ROOT/src/Engine/IO/XML/XMLNode.cpp \
    $$BANG_ROOT/src/Engine/IO/XML/XMLParser.cpp \
    $$BANG_ROOT/src/Engine/IO/XML/XMLProperty.cpp \
    $$BANG_ROOT/src/Engine/Components/AudioListener.cpp \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourManagerStatus.cpp \
    $$BANG_ROOT/src/Engine/Components/RectTransform.cpp \
    $$BANG_ROOT/src/Engine/General/Object.cpp \
    $$BANG_ROOT/src/Engine/Debug/GraphicPipelineDebugger.cpp \
    $$BANG_ROOT/src/Engine/Compiler/CodePreprocessor.cpp \
    $$BANG_ROOT/src/Engine/IO/Project/ProjectManager.cpp \
    $$BANG_ROOT/src/Engine/IO/Project/Project.cpp \
    $$BANG_ROOT/src/Engine/Debug/ChronoGL.cpp \
    $$BANG_ROOT/src/Engine/IO/General/SerializableObject.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/ExtensionManager.cpp \
    $$BANG_ROOT/src/Engine/Components/CircleCulledRenderer.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/PrefabFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/SoundFile.cpp \
    $$BANG_ROOT/src/Engine/Audio/AnonymousAudioPlayer.cpp \
    $$BANG_ROOT/src/Engine/Components/PostProcessEffect.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/ModelFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/MeshFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/Texture2DFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/ShaderProgramFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/MaterialFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/AudioClipFile.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/FontFile.cpp \
    $$BANG_ROOT/src/Engine/IO/General/Paths.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BangPreprocessor.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.cpp \
    $$BANG_ROOT/src/Engine/Compiler/Compiler.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangCompiler.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.cpp \
    $$BANG_ROOT/src/Engine/IO/Files/BFile.cpp \
    $$BANG_ROOT/src/Engine/Compiler/BangCompilerAsyncJob.cpp

HEADERS += \
    $$BANG_ROOT/src/Engine/General/IToString.h \
    $$BANG_ROOT/src/Editor/General/Cursor.h \
    $$BANG_ROOT/src/Editor/Graphics/ShaderManager.h \
    $$BANG_ROOT/src/Editor/Windows/GameWindow.h \
    $$BANG_ROOT/src/Editor/General/Application.h \
    $$BANG_ROOT/src/Editor/Windows/IWindow.h \
    $$BANG_ROOT/src/Engine/Graphics/General/Screen.h \
    $$BANG_ROOT/src/Engine/General/ISceneEventListener.h \
    $$BANG_ROOT/src/Engine/GameObjects/GameObject.h \
    $$BANG_ROOT/src/Engine/GameObjects/Scene.h \
    $$BANG_ROOT/src/Engine/Components/Transform.h \
    $$BANG_ROOT/src/Engine/Components/Component.h \
    $$BANG_ROOT/src/Engine/Components/MeshRenderer.h \
    $$BANG_ROOT/src/Engine/Components/Camera.h \
    $$BANG_ROOT/src/Engine/Assets/Asset.h \
    $$BANG_ROOT/src/Engine/Assets/Material.h \
    $$BANG_ROOT/src/Engine/Assets/Mesh.h \
    $$BANG_ROOT/src/Engine/Components/Behaviour.h \
    $$BANG_ROOT/src/Engine/Assets/Texture2D.h \
    $$BANG_ROOT/src/Engine/Assets/Prefab.h \
    $$BANG_ROOT/include/Bang/Bang.h \
    $$BANG_ROOT/src/Engine/IO/General/AssetsManager.h \
    $$BANG_ROOT/src/Engine/General/Input.h \
    $$BANG_ROOT/src/Engine/Components/LineRenderer.h \
    $$BANG_ROOT/src/Engine/Components/Renderer.h \
    $$BANG_ROOT/src/Engine/Math/Sphere.h \
    $$BANG_ROOT/src/Engine/General/SystemUtils.h \
    $$BANG_ROOT/src/Engine/General/SingletonManager.h \
    $$BANG_ROOT/src/Engine/Components/CircleRenderer.h \
    $$BANG_ROOT/src/Engine/Components/SingleLineRenderer.h \
    $$BANG_ROOT/src/Engine/Assets/MeshFactory.h \
    $$BANG_ROOT/src/Engine/General/ICloneable.h \
    $$BANG_ROOT/src/Engine/Components/DirectionalLight.h \
    $$BANG_ROOT/src/Engine/Components/Light.h \
    $$BANG_ROOT/src/Engine/Components/PointLight.h \
    $$BANG_ROOT/src/Engine/General/SceneManager.h \
    $$BANG_ROOT/src/Engine/Components/Canvas.h \
    $$BANG_ROOT/src/Engine/Components/UIImage.h \
    $$BANG_ROOT/src/Engine/Components/UIRenderer.h \
    $$BANG_ROOT/src/Engine/Assets/Font.h \
    $$BANG_ROOT/src/Engine/Assets/ShaderProgram.h \
    $$BANG_ROOT/src/Engine/Components/UIText.h \
    $$BANG_ROOT/src/Engine/IO/General/EngineConfig.h \
    $$BANG_ROOT/src/Engine/Math/AABox.h \
    $$BANG_ROOT/src/Engine/Assets/AudioClip.h \
    $$BANG_ROOT/src/Engine/Assets/Material.h \
    $$BANG_ROOT/src/Engine/Components/AudioSource.h \
    $$BANG_ROOT/src/Engine/Audio/AudioManager.h \
    $$BANG_ROOT/src/Engine/Audio/AudioPlayerRunnable.h \
    $$BANG_ROOT/src/Engine/Debug/Chrono.h \
    $$BANG_ROOT/src/Engine/Debug/Debug.h \
    $$BANG_ROOT/src/Engine/Debug/Gizmos.h \
    $$BANG_ROOT/src/Engine/General/Time.h \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/ShaderPreprocessor.h \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourManager.h \
    $$BANG_ROOT/src/Engine/IO/Files/FileReader.h \
    $$BANG_ROOT/src/Engine/IO/Files/ImageFile.h \
    $$BANG_ROOT/src/Engine/IO/XML/XMLAttribute.h \
    $$BANG_ROOT/src/Engine/IO/XML/XMLNode.h \
    $$BANG_ROOT/src/Engine/IO/XML/XMLParser.h \
    $$BANG_ROOT/src/Engine/IO/XML/XMLProperty.h \
    $$BANG_ROOT/src/Engine/IO/Files/TextFile.h \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/Shader.h \
    $$BANG_ROOT/src/Engine/IO/Files/ExtensionManager.h \
    $$BANG_ROOT/src/Engine/Components/AudioListener.h \
    $$BANG_ROOT/src/Engine/IO/Behaviours/BehaviourManagerStatus.h \
    $$BANG_ROOT/src/Engine/Components/RectTransform.h \
    $$BANG_ROOT/src/Engine/Debug/GraphicPipelineDebugger.h \
    $$BANG_ROOT/src/Engine/General/Object.h \
    $$BANG_ROOT/src/Engine/Compiler/CodePreprocessor.h \
    $$BANG_ROOT/src/Engine/IO/Project/Project.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GraphicPipeline.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_SP_DeferredLights.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_G_Gizmos.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_G.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_SP_PostProcessEffects.h \
    $$BANG_ROOT/src/Engine/Graphics/GraphicPipeline/GPPass_RenderLayer.h \
    $$BANG_ROOT/src/Engine/IO/Project/ProjectManager.h \
    $$BANG_ROOT/src/Engine/Debug/ChronoGL.h \
    $$BANG_ROOT/src/Engine/IO/General/SerializableObject.h \
    $$BANG_ROOT/src/Engine/Components/CircleCulledRenderer.h \
    $$BANG_ROOT/src/Engine/IO/Files/PrefabFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/SoundFile.h \
    $$BANG_ROOT/src/Engine/Audio/AnonymousAudioPlayer.h \
    $$BANG_ROOT/src/Engine/DataStructures/Flags.h \
    $$BANG_ROOT/src/Engine/Components/PostProcessEffect.h \
    $$BANG_ROOT/src/Engine/IO/Files/ModelFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/MeshFile.h \
    $$BANG_ROOT/src/Engine/DataStructures/Regex.h \
    $$BANG_ROOT/src/Engine/IO/Files/AudioClipFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/FontFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/MaterialFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/ShaderProgramFile.h \
    $$BANG_ROOT/src/Engine/IO/Files/Texture2DFile.h \
    $$BANG_ROOT/src/Engine/IO/General/Paths.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BangPreprocessor.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedVariable.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/BPReflectedStruct.h \
    $$BANG_ROOT/src/Engine/IO/Files/BFile.h \
    $$BANG_ROOT/src/Engine/Compiler/BangPreprocessor/IReflectable.h \
    $$BANG_ROOT/src/Engine/Compiler/Compiler.h \
    $$BANG_ROOT/src/Engine/Compiler/BangCompiler.h \
    $$BANG_ROOT/src/Engine/Compiler/BangCompilerAsyncJob.h

######################################


# OTHER FILES #############
DISTFILES += \
    ../.gitignore \
    $$BANG_ROOT/src/Engine \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SelectionBuffer.frag_sel \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/RenderG_GBufferToScreen.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Template.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Default.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SelectionBuffer.vert_sel \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Template.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Default.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/UI/SP_UIImage.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/PointLight.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/Main.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/DirectionalLight.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/G.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/G.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Materials/RenderG_GBufferToScreen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/G_Default.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/G_UIImage.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Missing.frag \
    $$BANG_ROOT/src/res/EngineAssets/Materials/Missing.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_AmbientLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_SelectionEffect.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_PointLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_DirectionalLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/SP.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/SP.vert_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_DirectionalLight.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_ScreenPass.vert_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_PointLight.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_SelectionEffect.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_Template.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Materials/G_DefaultNoSP.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/G_UIImage.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/SP_UIImage.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_FXAA.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/Common.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Fonts/GreatFont.bfont \
    $$BANG_ROOT/src/res/EngineAssets/Fonts/HandwrittenFont.bfont \
    $$BANG_ROOT/src/res/EngineAssets/Fonts/PrettyFont.bfont \
    $$BANG_ROOT/src/res/EngineAssets/Fonts/UbuntuFont.bfont \
    $$BANG_ROOT/src/res/EngineAssets/Camera.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Cone.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Cube.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Plane.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Pyramid.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Sphere.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/UIPlane.bmesh \
    $$BANG_ROOT/src/res/EngineAssets/Textures/AudioSourceIcon.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/CameraIcon.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/CircleIcon.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/DefaultUIImageTexture.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/DirectionalLightIcon.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/PointLightIcon.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Textures/RectTransformAnchor.btex2d \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_Default.vert_pp \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/G_UIText_Pass2.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/UI/G_UIText_Pass2.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/G_UIText.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/UI/G_UIText.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_Default.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Default.bshaderprogram \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Cartoon.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Missing.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/InvertColors.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Blur.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/GrayScale.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Outline.frag_pp
