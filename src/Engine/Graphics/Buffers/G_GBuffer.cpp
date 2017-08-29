#include "Bang/G_GBuffer.h"

#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/G_RenderTexture.h"
#include "Bang/G_ShaderProgram.h"

G_GBuffer::G_GBuffer(int width, int height) : G_Framebuffer(width, height)
{
    CreateColorAttachment(AttNormalDepth,
                          GL::ColorInternalFormat::RGBA_Float16);
    CreateColorAttachment(AttDiffuse,
                          GL::ColorInternalFormat::RGBA_UByte8);
    CreateColorAttachment(AttMisc,
                          GL::ColorInternalFormat::RGBA_UByte8);
    CreateColorAttachment(AttColor,
                          GL::ColorInternalFormat::RGBA_UByte8);
    CreateColorAttachment(AttColorRead,
                          GL::ColorInternalFormat::RGBA_UByte8);
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
    ENSURE(sp); ASSERT(GL::IsBound(sp));

    sp->Set("B_GTex_NormalDepth", m_normalTexture);
    sp->Set("B_GTex_DiffColor",   m_diffuseTexture);
    sp->Set("B_GTex_Misc",        m_miscTexture);
    sp->Set("B_GTex_Color", willReadFromColor ? m_colorReadTexture :
                                                       m_colorTexture);
}


void G_GBuffer::ApplyPass(G_ShaderProgram *sp,
                          bool prepareReadFromColorBuffer,
                          const Rect &mask)
{
    ENSURE(sp); ASSERT(GL::IsBound(this)); ASSERT(GL::IsBound(sp));
    bool prevStencilWrite = GL::IsStencilWrite();
    GL::SetStencilWrite(false);

    BindTextureBuffersTo(sp, prepareReadFromColorBuffer);

    if (prepareReadFromColorBuffer) { PrepareColorReadBuffer(); }
    SetColorDrawBuffer();

    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    GL::SetStencilWrite(prevStencilWrite);
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
    GL::ClearStencilBuffer();
    SetDrawBuffers({G_GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color::Zero, true, true, false, false);

    ClearDepth(1.0f);

    SetDrawBuffers({G_GBuffer::AttDiffuse, G_GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero);

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
}
