#include "Bang/GBuffer.h"

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/ShaderProgram.h"

USING_NAMESPACE_BANG

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    Bind();
    CreateAttachment(AttColor,        GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttDiffuse,      GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttNormal,       GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttMisc,         GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttColorRead,    GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttDepthStencil, GL::ColorFormat::Depth24_Stencil8);
    UnBind();
}

GBuffer::~GBuffer()
{
}

void GBuffer::BindAttachmentsForReading(ShaderProgram *sp)
{
    if (!sp) { return; }
    ASSERT(GL::IsBound(sp));

    sp->Set("B_GTex_Normal",       GetAttachmentTexture(AttNormal), false);
    sp->Set("B_GTex_DiffColor",    GetAttachmentTexture(AttDiffuse), false);
    sp->Set("B_GTex_Misc",         GetAttachmentTexture(AttMisc), false);
    sp->Set("B_GTex_Color",        GetAttachmentTexture(AttColorRead), false);
    sp->Set("B_GTex_DepthStencil", GetAttachmentTexture(AttDepthStencil), false);
}


void GBuffer::ApplyPass(ShaderProgram *sp,
                        bool willReadFromColor,
                        const Rect &mask)
{
    if (!sp) { return; }
    ASSERT(GL::IsBound(this));
    ASSERT(GL::IsBound(sp));

    GL::StencilOperation prevStencilOp = GL::GetStencilOp();
    GL::SetStencilOp(GL::StencilOperation::Keep); // Dont modify stencil

    if (willReadFromColor) { PrepareColorReadBuffer(mask); }

    BindAttachmentsForReading(sp);

    PushDrawAttachments();
    SetColorDrawBuffer();

    GEngine::GetActive()->RenderScreenRect(sp, mask);

    PopDrawAttachments();

    GL::SetStencilOp(prevStencilOp);
}

void GBuffer::PrepareColorReadBuffer(const Rect &readNDCRect)
{
    PushDrawAttachments();
    SetReadBuffer(AttColor);
    SetDrawBuffers({AttColorRead});
    Rect rf (readNDCRect * 0.5f + 0.5f);
    Recti r ( Rect(Vector2::Floor(rf.GetMin()),
                   Vector2::Ceil(rf.GetMax())) * GetSize() );
    GL::BlitFramebuffer(r, r, GL::FilterMode::Nearest,
                        GL::BufferBit::Color);
    PopDrawAttachments();
}

void GBuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers({GBuffer::AttColor,  GBuffer::AttDiffuse,
                    GBuffer::AttNormal, GBuffer::AttMisc
                   });
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttDiffuse, GBuffer::AttNormal, GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
}

void GBuffer::ClearAllBuffersExceptColor()
{
    GL::ClearStencilBuffer(0);
    ClearDepth(1.0f);

    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::Zero);
}

void GBuffer::ClearBuffersAndBackground(const Color &backgroundColor)
{
    ClearAllBuffersExceptColor();
    SetDrawBuffers({GBuffer::AttColor}); // , GBuffer::AttColorRead});
    GL::ClearColorBuffer(backgroundColor);
}
