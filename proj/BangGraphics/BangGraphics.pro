include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
INCLUDEPATH += $$PWD/../../include
QT += core gui opengl widgets

CONFIG += dll

TARGET = $$BIN_DIR/lib/BangGraphics

UI_DIR  = $$PWD/../../include/Bang
EDITOR {
    FORMS   += $$PWD/../../res/EngineAssets/EditorWindow.ui
    FORMS   += $$PWD/../../res/EngineAssets/SelectProjectWindow.ui
}

MOC_DIR = $$PWD/../../bin/Qt_MOCs/

SOURCES += \
    $$PWD/../../src/Engine/Graphics/General/G_Screen.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_TextureUnitManager.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_Texture.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_VAO.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_VBO.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_Font.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_Material.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_FontSheetCreator.cpp \
    $$PWD/../../src/Engine/Graphics/Buffers/G_Framebuffer.cpp \
    $$PWD/../../src/Engine/Graphics/Buffers/G_GBuffer.cpp \
    $$PWD/../../src/Engine/Graphics/Shaders/G_Shader.cpp \
    $$PWD/../../src/Engine/Graphics/Shaders/G_ShaderProgram.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_RenderTexture.cpp \
    $$PWD/../../src/Engine/Graphics/General/GLContext.cpp \
    $$PWD/../../src/Engine/Graphics/General/GL.cpp \
    $$PWD/../../src/Engine/Graphics/General/GLObject.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_Image.cpp \
    $$PWD/../../src/Engine/Graphics/General/G_Texture2D.cpp

HEADERS += \
    $$PWD/../../src/Engine/Graphics/General/G_Screen.h \
    $$PWD/../../src/Engine/Graphics/General/G_Texture.h \
    $$PWD/../../src/Engine/Graphics/General/G_VAO.h \
    $$PWD/../../src/Engine/Graphics/General/G_VBO.h \
    $$PWD/../../src/Engine/Graphics/General/G_Font.h \
    $$PWD/../../src/Engine/Graphics/General/G_Material.h \
    $$PWD/../../src/Engine/Graphics/General/G_FontSheetCreator.h \
    $$PWD/../../src/Engine/Graphics/Buffers/G_Framebuffer.h \
    $$PWD/../../src/Engine/Graphics/Buffers/G_GBuffer.h \
    $$PWD/../../src/Engine/Graphics/Shaders/G_Shader.h \
    $$PWD/../../src/Engine/Graphics/Shaders/G_ShaderProgram.h \
    $$PWD/../../src/Engine/Graphics/General/G_RenderTexture.h \
    $$PWD/../../src/Engine/Graphics/General/GLContext.h \
    $$PWD/../../src/Engine/Graphics/General/GL.h \
    $$PWD/../../src/Engine/Graphics/General/G_Texture2D.h \
    $$PWD/../../src/Engine/Graphics/General/GLObject.h \
    $$PWD/../../src/Engine/Graphics/General/G_TextureUnitManager.h \
    $$PWD/../../src/Engine/Graphics/General/G_Image.h

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
