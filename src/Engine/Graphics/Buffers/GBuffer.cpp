#include "Bang/GBuffer.h"

#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/RenderTexture.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/TextureUnitManager.h"

USING_NAMESPACE_BANG

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    Bind();
    CreateAttachment(AttNormalDepth,  GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttDiffuse,      GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttMisc,         GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttColor,        GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttColorRead,    GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttDepthStencil, GL::ColorFormat::Depth24_Stencil8);
    UnBind();

    m_normalTexture       = GetAttachmentTexture(AttNormalDepth);
    m_diffuseTexture      = GetAttachmentTexture(AttDiffuse);
    m_miscTexture         = GetAttachmentTexture(AttMisc);
    m_colorTexture        = GetAttachmentTexture(AttColor);
    m_colorReadTexture    = GetAttachmentTexture(AttColorRead);
    m_depthStencilTexture = GetAttachmentTexture(AttDepthStencil);
}

GBuffer::~GBuffer()
{
}

void GBuffer::PrepareForRender(ShaderProgram *sp)
{
    ENSURE(sp); ASSERT(GL::IsBound(sp));

    sp->Set("B_GTex_Normal",      m_normalTexture);
    sp->Set("B_GTex_DiffColor",    m_diffuseTexture);
    sp->Set("B_GTex_Misc",         m_miscTexture);
    sp->Set("B_GTex_Color",        m_colorReadTexture);
    sp->Set("B_GTex_DepthStencil", m_depthStencilTexture);
}


void GBuffer::ApplyPass(ShaderProgram *sp,
                        bool willReadFromColor,
                        const Rect &mask)
{
    ENSURE(sp); ASSERT(GL::IsBound(this)); ASSERT(GL::IsBound(sp));

    GL::StencilOperation prevStencilOp = GL::GetStencilOp();
    GL::SetStencilOp(GL::StencilOperation::Keep); // Dont modify stencil

    if (willReadFromColor) { PrepareColorReadBuffer(mask); }

    PrepareForRender(sp);

    SetColorDrawBuffer();

    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    GL::SetStencilOp(prevStencilOp);
}

void GBuffer::PrepareColorReadBuffer(const Rect &readNDCRect)
{
    PushDrawAttachmentIds();
    SetReadBuffer(AttColor);
    SetDrawBuffers({AttColorRead});
    Rect rf (readNDCRect * 0.5f + 0.5f);
    Recti r ( Rect(Vector2::Floor(rf.GetMin()),
                   Vector2::Ceil(rf.GetMax())) * GetSize() );
    GL::BlitFramebuffer(r, r, GL::FilterMode::Nearest,
                        GL::BufferBit::Color);
    PopDrawAttachmentIds();
}

void GBuffer::SetAllColorDrawBuffers() const
{
    SetDrawBuffers({GBuffer::AttNormalDepth, GBuffer::AttDiffuse,
                    GBuffer::AttMisc, GBuffer::AttColor
                   });
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttNormalDepth, GBuffer::AttDiffuse,
                    GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
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
    GL::ClearStencilBuffer();
    SetDrawBuffers({GBuffer::AttNormalDepth});
    GL::ClearColorBuffer(Color::Zero, true, true, false, false);

    ClearDepth(1.0f);

    SetDrawBuffers({GBuffer::AttDiffuse, GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero);

    SetDrawBuffers({GBuffer::AttColor, GBuffer::AttColorRead});
    GL::ClearColorBuffer(backgroundColor);
}
