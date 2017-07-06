#include "Bang/G_GBuffer.h"

#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/G_VAO.h"
#include "Bang/Color.h"
#include "Bang/Array.h"
#include "Bang/G_Screen.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/G_RenderTexture.h"
#include "Bang/G_ShaderProgram.h"

G_GBuffer::G_GBuffer(int width, int height) : G_Framebuffer(width, height)
{
    CreateColorAttachment( AttachmentId( AttNormalDepth  ),
                           G_Texture::Format::RGBA_Float16 );
    CreateColorAttachment( AttachmentId( AttDiffuse ),
                           G_Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttMisc    ),
                           G_Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttColor  ),
                           G_Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttColorRead  ),
                           G_Texture::Format::RGBA_Byte8 );
    CreateDepthRenderbufferAttachment();

    m_normalTexture    = GetAttachmentTexture(AttNormalDepth);
    m_diffuseTexture   = GetAttachmentTexture(AttDiffuse);
    m_miscTexture      = GetAttachmentTexture(AttMisc);
    m_colorTexture     = GetAttachmentTexture(AttColor);
    m_colorReadTexture = GetAttachmentTexture(AttColorRead);
}

G_GBuffer::~G_GBuffer()
{
}

void G_GBuffer::BindTextureBuffersTo(G_ShaderProgram *sp,
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


void G_GBuffer::ApplyPass(G_ShaderProgram *sp,
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

void G_GBuffer::RenderToScreen(G_GBuffer::AttachmentId attId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    G_Texture *tex = GetAttachmentTexture(attId); ENSURE(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void G_GBuffer::RenderToScreen()
{
    RenderToScreen(G_GBuffer::AttColor);
}

void G_GBuffer::PrepareColorReadBuffer(const Rect &readNDCRect)
{
    PushDrawAttachmentIds();
    SetReadBuffer(AttColor);
    SetDrawBuffers({AttColorRead});
    Rect r = (readNDCRect * 0.5f + 0.5f) * GetSize();
    glBlitFramebuffer(r.GetMin().x, r.GetMin().x, r.GetMax().x,
                      r.GetMax().y, r.GetMin().x, r.GetMin().x,
                      r.GetMax().x, r.GetMax().y,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    PopDrawAttachmentIds();
}

void G_GBuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers({G_GBuffer::AttNormalDepth, G_GBuffer::AttDiffuse,
                    G_GBuffer::AttMisc, G_GBuffer::AttColor
                   });
}

void G_GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({G_GBuffer::AttNormalDepth, G_GBuffer::AttDiffuse,
                    G_GBuffer::AttMisc});
}

void G_GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({G_GBuffer::AttColor});
}

void G_GBuffer::SetStencilWrite(bool writeEnabled)
{
    // Replace is to ref value 1
    if (m_stencilWrite != writeEnabled)
    {
        glStencilOp(writeEnabled ? GL_REPLACE : GL_KEEP, GL_KEEP, GL_KEEP);
    }
    m_stencilWrite = writeEnabled;
}

void G_GBuffer::SetStencilTest(bool testEnabled)
{
    if (m_stencilTest != testEnabled)
    {
        glStencilFunc(testEnabled ? GL_GEQUAL : GL_ALWAYS, 1, 0xFF);
    }
    m_stencilTest = testEnabled;
}

void G_GBuffer::ClearStencil()
{
    GL::ClearStencilBuffer();
}

void G_GBuffer::ClearDepth(float clearDepth)
{
    G_Framebuffer::ClearDepth(clearDepth); // Clear typical depth buffer

    const float encodedDepthHigh = Math::Floor(clearDepth * 1024);
    const float encodedDepthLow  = Math::Fract(clearDepth * 1024);

    SetDrawBuffers({G_GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color(0, 0, encodedDepthHigh, encodedDepthLow),
                         false, false, true, true);
}

void G_GBuffer::ClearBuffersAndBackground(const Color &backgroundColor)
{
    ClearStencil();
    SetDrawBuffers({G_GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color::Zero, true, true, false, false);

    ClearDepth(1.0f);

    SetDrawBuffers({G_GBuffer::AttDiffuse, G_GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero);

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
}
