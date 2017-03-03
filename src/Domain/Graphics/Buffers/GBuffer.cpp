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
    m_normalTexture   = new RenderTexture();
    m_diffuseTexture  = new RenderTexture();
    m_miscTexture     = new RenderTexture();
    m_colorTexture    = new RenderTexture();

    SetAttachment(Attachment::Normal,   m_normalTexture);
    SetAttachment(Attachment::Diffuse,  m_diffuseTexture);
    SetAttachment(Attachment::Misc,     m_miscTexture);
    SetAttachment(Attachment::Color,    m_colorTexture);
    CreateDepthRenderbufferAttachment();
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
    SaveCurrentDrawBuffers();

    // Set as only draw output: "B_color_gout_gin". Accumulate color there.
    SetColorDrawBuffer();
    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    SetStencilWrite(prevStencilWrite);
    LoadSavedDrawBuffers();
}

void GBuffer::RenderToScreen(GBuffer::Attachment attachmentId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    Texture *tex = GetColorAttachment(attachmentId); ASSERT(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void GBuffer::SaveCurrentDrawBuffers()
{
    m_previousDrawAttachmentsIds = GetCurrentDrawAttachmentIds();
}

void GBuffer::LoadSavedDrawBuffers()
{
    SetDrawBuffers(m_previousDrawAttachmentsIds);
}

void GBuffer::RenderToScreen()
{
    RenderToScreen(GBuffer::Attachment::Color);
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::Misc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::Attachment::Color});
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
    Bind();
    SaveCurrentDrawBuffers();
    SetDrawBuffers({GBuffer::Attachment::Misc});
    GL::ClearColorBuffer(Color::Zero, false, false, false, true);
    LoadSavedDrawBuffers();
    UnBind();
}

void GBuffer::ClearDepth(float clearDepth)
{
    Framebuffer::ClearDepth(clearDepth); // Clear typical depth buffer

    Bind();
    SaveCurrentDrawBuffers();
    SetDrawBuffers({GBuffer::Attachment::Misc});
    GL::ClearColorBuffer(Color::One, false, false, true, false);
    LoadSavedDrawBuffers();
    UnBind();
}

void GBuffer::ClearStencilDepth()
{
    Bind();
    SaveCurrentDrawBuffers();
    SetDrawBuffers({GBuffer::Attachment::Misc});
    GL::ClearColorBuffer(::Color(0,0,1,0), false, false, true, true);
    GL::ClearDepthBuffer(1.0f);
    LoadSavedDrawBuffers();
    UnBind();
}

void GBuffer::ClearBuffersAndBackground(const ::Color &backgroundColor,
                                        const ::Color &clearValue)
{
    Bind();

    SaveCurrentDrawBuffers();

    SetDrawBuffers({GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Diffuse});
    GL::ClearColorBuffer(clearValue);

    SetDrawBuffers({GBuffer::Attachment::Misc});
    GL::ClearColorBuffer(::Color(0,0,1,0));

    GL::ClearDepthBuffer(1.0f);

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);

    LoadSavedDrawBuffers();

    UnBind();
}
