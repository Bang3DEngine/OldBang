#include "GBuffer.h"

#include "VAO.h"
#include "Debug.h"
#include "Mesh.h"
#include "Rect.h"
#include "Scene.h"
#include "Color.h"
#include "Array.h"
#include "Input.h"
#include "Camera.h"
#include "Screen.h"
#include "Material.h"
#include "Transform.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "RenderTexture.h"
#include "GraphicPipeline.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment( AttachmentId( AttNormal  ) );
    CreateColorAttachment( AttachmentId( AttDiffuse ) );
    CreateColorAttachment( AttachmentId( AttMisc    ) );
    CreateColorAttachment( AttachmentId( AttColor  ) );
    CreateColorAttachment( AttachmentId( AttColorRead  ) );
    CreateDepthRenderbufferAttachment();

    m_normalTexture    = GetAttachmentTexture(AttNormal);
    m_diffuseTexture   = GetAttachmentTexture(AttDiffuse);
    m_miscTexture      = GetAttachmentTexture(AttMisc);
    m_colorTexture     = GetAttachmentTexture(AttColor);
    m_colorReadTexture = GetAttachmentTexture(AttColorRead);
}

GBuffer::~GBuffer()
{
}

void GBuffer::BindTextureBuffersTo(ShaderProgram *sp) const
{
    // Color Attachments bindings as Shader Inputs
    sp->SetTexture("B_GTex_Normal",    m_normalTexture);
    sp->SetTexture("B_GTex_DiffColor", m_diffuseTexture);
    sp->SetTexture("B_GTex_Misc",      m_miscTexture);
    sp->SetTexture("B_GTex_Color",     m_colorTexture);

    // Stencil uniforms
    sp->SetFloat("B_StencilWriteEnabled", m_stencilWriteEnabled ? 1.0f : 0.0f);
    sp->SetFloat("B_StencilTestEnabled",  m_stencilTestEnabled  ? 1.0f : 0.0f);
}


void GBuffer::ApplyPass(ShaderProgram *sp,
                        bool copyColorBuffer,
                        const Rect &mask)
{
    sp->SetVec2("B_ScreenSize", Screen::GetSize());

    bool prevStencilWrite = m_stencilWriteEnabled;
    SetStencilWrite(false);

    Bind();
    BindTextureBuffersTo(sp);

    if (copyColorBuffer)
    {
        SetReadBuffer(AttColor);
        SetDrawBuffers({AttColorRead});
        glBlitFramebuffer(0, 0, GetWidth(), GetHeight(),
                          0, 0, GetWidth(), GetHeight(),
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
        sp->SetTexture("B_GTex_Color", m_colorReadTexture);
    }

    SetColorDrawBuffer();
    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    UnBind();

    SetStencilWrite(prevStencilWrite);
}

void GBuffer::RenderToScreen(GBuffer::AttachmentId attId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    Texture *tex = GetAttachmentTexture(attId); ASSERT(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void GBuffer::RenderToScreen()
{
    RenderToScreen(GBuffer::AttColor);
}

void GBuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc,
                    GBuffer::AttColor});
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
}

void GBuffer::SetStencilWrite(bool writeEnabled)
{
    m_stencilWriteEnabled = writeEnabled;
}

void GBuffer::SetStencilTest(bool testEnabled)
{
    m_stencilTestEnabled = testEnabled;
}

void GBuffer::ClearStencil()
{
    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero, false, false, false, true);
}

void GBuffer::ClearDepth(float clearDepth)
{
    Framebuffer::ClearDepth(clearDepth); // Clear typical depth buffer
    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::One, false, false, true, false);
}

void GBuffer::ClearStencilDepth()
{
    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color(0,0,1,0), false, false, true, true);
    GL::ClearDepthBuffer(1.0f);
}

void GBuffer::ClearBuffersAndBackground(const Color &backgroundColor,
                                        const Color &clearValue)
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse});
    GL::ClearColorBuffer(clearValue);

    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color(0,0,1,0));

    GL::ClearDepthBuffer(1.0f);

    //*
    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
    /*/
    SetDrawBuffers({GBuffer::AttColor0});
    GL::ClearColorBuffer(Color::Red);
    SetDrawBuffers({GBuffer::AttColor1});
    GL::ClearColorBuffer(Color::Blue);
    //*/
}
