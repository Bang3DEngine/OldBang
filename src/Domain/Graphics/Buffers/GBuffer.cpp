#include "GBuffer.h"

#include "VAO.h"
#include "Debug.h"
#include "Mesh.h"
#include "Rect.h"
#include "Scene.h"
#include "Color.h"
#include "Array.h"
#include "Camera.h"
#include "Screen.h"
#include "Material.h"
#include "Transform.h"
#include "MeshFactory.h"
#include "ShaderProgram.h"
#include "AssetsManager.h"
#include "RenderTexture.h"
#include "ShaderContract.h"
#include "GraphicPipeline.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment( AttachmentId( AttNormal  ) );
    CreateColorAttachment( AttachmentId( AttDiffuse ) );
    CreateColorAttachment( AttachmentId( AttMisc    ) );
    CreateColorAttachment( AttachmentId( AttColor   ) );
    CreateDepthRenderbufferAttachment();

    m_normalTexture  = GetAttachmentTexture(AttNormal);
    m_diffuseTexture = GetAttachmentTexture(AttDiffuse);
    m_miscTexture    = GetAttachmentTexture(AttMisc);
    m_colorTexture   = GetAttachmentTexture(AttColor);
}

GBuffer::~GBuffer()
{
}

void GBuffer::OnRenderingStarts(GameObject *go, ShaderProgram *sp)
{
    // Color Attachments bindings as Shader Inputs
    sp->SetTexture("B_normal_gout_fin",   m_normalTexture);
    sp->SetTexture("B_diffuse_gout_fin",  m_diffuseTexture);
    sp->SetTexture("B_misc_gout_fin",     m_miscTexture);
    sp->SetTexture("B_color_gout_fin",    m_colorTexture);

    // Stencil uniforms
    sp->SetFloat("B_stencilWriteEnabled", m_stencilWriteEnabled ? 1.0f : 0.0f);
    sp->SetFloat("B_stencilTestEnabled",  m_stencilTestEnabled  ? 1.0f : 0.0f);
}


void GBuffer::ApplyPass(ShaderProgram *sp, const Rect &mask)
{
    Vector2 buffersSize = GraphicPipeline::GetBuffersSize();
    sp->SetVec2("B_buffer_size", buffersSize);
    sp->SetVec2("B_screen_size", Screen::GetSize());

    bool prevStencilWrite = m_stencilWriteEnabled;
    SetStencilWrite(false);

    OnRenderingStarts(nullptr, sp);

    // Set as only draw output: "B_color_gout_gin". Accumulate color there.
    SetColorDrawBuffer();
    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    SetStencilWrite(prevStencilWrite);
}

void GBuffer::RenderToScreen(GBuffer::AttachmentId attachmentId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    Texture *tex = GetAttachmentTexture(attachmentId); ASSERT(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void GBuffer::RenderToScreen()
{
    RenderToScreen(GBuffer::AttColor);
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

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
}
