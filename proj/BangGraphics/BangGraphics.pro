include(../BangCommon.pri)
system(rm -f Makefile)

TEMPLATE = lib
CONFIG += staticlib
QT += core gui opengl widgets

INCLUDEPATH += \
    $$BANG_ROOT/include/Bang/BangGraphics

TARGET = $$BIN_DIR/lib/BangGraphics

SOURCES += \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Screen.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_TextureUnitManager.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Texture.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_VAO.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_VBO.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Font.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Material.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_FontSheetCreator.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Buffers/G_Framebuffer.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Buffers/G_GBuffer.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/G_Shader.cpp \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/G_ShaderProgram.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_RenderTexture.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/GLContext.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/GL.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/GLObject.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Image.cpp \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Texture2D.cpp

HEADERS += \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Screen.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Texture.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_VAO.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_VBO.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Font.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Material.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_FontSheetCreator.h \
    $$BANG_ROOT/src/Engine/Graphics/Buffers/G_Framebuffer.h \
    $$BANG_ROOT/src/Engine/Graphics/Buffers/G_GBuffer.h \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/G_Shader.h \
    $$BANG_ROOT/src/Engine/Graphics/Shaders/G_ShaderProgram.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_RenderTexture.h \
    $$BANG_ROOT/src/Engine/Graphics/General/GLContext.h \
    $$BANG_ROOT/src/Engine/Graphics/General/GL.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Texture2D.h \
    $$BANG_ROOT/src/Engine/Graphics/General/GLObject.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_TextureUnitManager.h \
    $$BANG_ROOT/src/Engine/Graphics/General/G_Image.h

######################################


# OTHER FILES #############
DISTFILES += \
    ../.gitignore \
    $$BANG_ROOT/src/Engine \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/RenderGBufferToScreen.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Template.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Default.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Template.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Default.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/UI/SP_UIImage.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/PointLight.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/Main.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/DirectionalLight.glsl \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/G.frag_g \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/G.vert_g \
    $$BANG_ROOT/src/res/EngineAssets/Materials/RenderGBufferToScreen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/G_Default.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/UI/G_UIImage.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/G_Missing.frag \
    $$BANG_ROOT/src/res/EngineAssets/Materials/Missing.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_AmbientLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_PointLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Materials/SP_DirectionalLight_Screen.bmat \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/SP.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/Include/SP.vert_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_DirectionalLight.frag_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_ScreenPass.vert_pp \
    $$BANG_ROOT/src/res/EngineAssets/Shaders/SP_PointLight.frag_pp \
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
