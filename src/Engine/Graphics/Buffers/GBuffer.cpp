#include "Bang/GBuffer.h"

#include "Bang/VAO.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Array.h"
#include "Bang/Screen.h"
#include "Bang/RenderTexture.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GraphicPipeline.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment( AttachmentId( AttNormalDepth  ),
                           Texture::Format::RGBA_Float16 );
    CreateColorAttachment( AttachmentId( AttDiffuse ),
                           Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttMisc    ),
                           Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttColor  ),
                           Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttColorRead  ),
                           Texture::Format::RGBA_Byte8 );
    CreateDepthRenderbufferAttachment();

    m_normalTexture    = GetAttachmentTexture(AttNormalDepth);
    m_diffuseTexture   = GetAttachmentTexture(AttDiffuse);
    m_miscTexture      = GetAttachmentTexture(AttMisc);
    m_colorTexture     = GetAttachmentTexture(AttColor);
    m_colorReadTexture = GetAttachmentTexture(AttColorRead);
}

GBuffer::~GBuffer()
{
}

void GBuffer::BindTextureBuffersTo(ShaderProgram *sp,
                                   bool willReadFromColor) const
{
    // Color Attachments bindings as Shader Inputs
    Bind();
    sp->SetTexture("B_GTex_NormalDepth", m_normalTexture);
    sp->SetTexture("B_GTex_DiffColor",   m_diffuseTexture);
    sp->SetTexture("B_GTex_Misc",        m_miscTexture);

    sp->SetTexture("B_GTex_Color", willReadFromColor ? m_colorReadTexture :
                                                       m_colorTexture);
    UnBind();
}


void GBuffer::ApplyPass(ShaderProgram *sp,
                        bool prepareReadFromColorBuffer,
                        const Rect &mask)
{
    bool prevStencilWrite = m_stencilWrite;
    SetStencilWrite(false);

    Bind();
    BindTextureBuffersTo(sp, prepareReadFromColorBuffer);

    if (prepareReadFromColorBuffer) { PrepareColorReadBuffer(); }
    SetColorDrawBuffer();

    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    UnBind();

    SetStencilWrite(prevStencilWrite);
}

void GBuffer::RenderToScreen(GBuffer::AttachmentId attId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    Texture *tex = GetAttachmentTexture(attId); ENSURE(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void GBuffer::RenderToScreen()
{
    RenderToScreen(GBuffer::AttColor);
}

void GBuffer::PrepareColorReadBuffer(const Rect &readNDCRect)
{
    PushDrawAttachmentIds();
    SetReadBuffer(AttColor);
    SetDrawBuffers({AttColorRead});
    Rect r = (readNDCRect * 0.5f + 0.5f) * GetSize();
    glBlitFramebuffer(r.GetMin().x, r.GetMin().x, r.GetMax().x, r.GetMax().y,
                      r.GetMin().x, r.GetMin().x, r.GetMax().x, r.GetMax().y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    PopDrawAttachmentIds();
}

void GBuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers({GBuffer::AttNormalDepth, GBuffer::AttDiffuse, GBuffer::AttMisc,
                    GBuffer::AttColor});
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttNormalDepth, GBuffer::AttDiffuse, GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
}

void GBuffer::SetStencilWrite(bool writeEnabled)
{
    // Replace is to ref value 1
    if (m_stencilWrite != writeEnabled)
    {
        glStencilOp(writeEnabled ? GL_REPLACE : GL_KEEP, GL_KEEP, GL_KEEP);
    }
    m_stencilWrite = writeEnabled;
}

void GBuffer::SetStencilTest(bool testEnabled)
{
    if (m_stencilTest != testEnabled)
    {
        glStencilFunc(testEnabled ? GL_GEQUAL : GL_ALWAYS, 1, 0xFF);
    }
    m_stencilTest = testEnabled;
}

void GBuffer::ClearStencil()
{
    GL::ClearStencilBuffer();
}

void GBuffer::ClearDepth(float clearDepth)
{
    Framebuffer::ClearDepth(clearDepth); // Clear typical depth buffer

    const float encodedDepthHigh = Math::Floor(clearDepth * 1024);
    const float encodedDepthLow  = Math::Fract(clearDepth * 1024);

    SetDrawBuffers({GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color(0, 0, encodedDepthHigh, encodedDepthLow),
                         false, false, true, true);
}

void GBuffer::ClearBuffersAndBackground(const Color &backgroundColor)
{
    ClearStencil();
    SetDrawBuffers({GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color::Zero, true, true, false, false);

    ClearDepth(1.0f);

    SetDrawBuffers({GBuffer::AttDiffuse, GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero);

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
}
