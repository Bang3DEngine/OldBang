#include "Bang/GBuffer.h"

#include "Bang/Math.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Texture2D.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GEngine.h"
#include "Bang/TextureUnitManager.h"

USING_NAMESPACE_BANG

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    Bind();
    CreateAttachment(AttNormal,       GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttDiffuse,      GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttMisc,         GL::ColorFormat::RGB10_A2_UByte);
    CreateAttachment(AttColor,        GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttColorRead,    GL::ColorFormat::RGBA_UByte8);
    CreateAttachment(AttDepthStencil, GL::ColorFormat::Depth24_Stencil8);
    UnBind();
}

GBuffer::~GBuffer()
{
}

void GBuffer::PrepareForRender(ShaderProgram *sp)
{
    ENSURE(sp); ASSERT(GL::IsBound(sp));

    sp->Set("B_GTex_Normal",       GetAttachmentTexture(AttNormal));
    sp->Set("B_GTex_DiffColor",    GetAttachmentTexture(AttDiffuse));
    sp->Set("B_GTex_Misc",         GetAttachmentTexture(AttMisc));
    sp->Set("B_GTex_Color",        GetAttachmentTexture(AttColorRead));
    sp->Set("B_GTex_DepthStencil", GetAttachmentTexture(AttDepthStencil));
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

    GEngine::GetInstance()->ApplyScreenPass(sp, mask);

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
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse,
                    GBuffer::AttMisc, GBuffer::AttColor
                   });
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse,
                    GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
}

void GBuffer::ClearAllBuffersExceptColor()
{
    GL::ClearStencilBuffer(0);
    ClearDepth(1.0f);

    // SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc});
    // GL::ClearColorBuffer(Color::Zero);
}

void GBuffer::ClearBuffersAndBackground(const Color &backgroundColor)
{
    ClearAllBuffersExceptColor();
    SetDrawBuffers({GBuffer::AttColor}); // , GBuffer::AttColorRead});
    GL::ClearColorBuffer(backgroundColor);
}
