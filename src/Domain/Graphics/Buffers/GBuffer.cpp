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
    m_positionTexture = new RenderTexture();
    m_normalTexture   = new RenderTexture();
    m_diffuseTexture  = new RenderTexture();
    m_matPropsTexture = new RenderTexture();
    m_depthTexture    = new RenderTexture();
    m_stencilTexture  = new RenderTexture();
    m_colorTexture    = new RenderTexture();

    SetAttachment(Attachment::Position,           m_positionTexture);
    SetAttachment(Attachment::Normal,             m_normalTexture);
    SetAttachment(Attachment::Diffuse,            m_diffuseTexture);
    SetAttachment(Attachment::MaterialProperties, m_matPropsTexture);
    SetAttachment(Attachment::Depth,              m_depthTexture);
    SetAttachment(Attachment::Stencil,            m_stencilTexture);
    SetAttachment(Attachment::Color,              m_colorTexture);
    CreateDepthRenderbufferAttachment();

    m_stencilTexture->SetFilterMode(Texture::FilterMode::Nearest);
}

GBuffer::~GBuffer()
{
}

void GBuffer::OnRenderingStarts(GameObject *go, ShaderProgram *sp)
{
    // Color Attachments bindings as Shader Inputs
    sp->SetTexture("B_position_gout_fin",      m_positionTexture);
    sp->SetTexture("B_normal_gout_fin",        m_normalTexture);
    sp->SetTexture("B_diffuse_gout_fin",       m_diffuseTexture);
    sp->SetTexture("B_materialProps_gout_fin", m_matPropsTexture);
    sp->SetTexture("B_depth_gout_fin",         m_depthTexture);
    sp->SetTexture("B_stencil_gout_fin",       m_stencilTexture);
    sp->SetTexture("B_color_gout_fin",         m_colorTexture);

    // Stencil uniforms
    sp->SetFloat("B_stencilWriteEnabled",  m_stencilWriteEnabled ? 1.0f : 0.0f);
    sp->SetFloat("B_stencilTestEnabled", m_stencilTestEnabled  ? 1.0f : 0.0f);
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
    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties,
                    GBuffer::Attachment::Depth,
                    GBuffer::Attachment::Stencil});
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
    SaveCurrentDrawBuffers();

    Bind();
    SetDrawBuffers({GBuffer::Attachment::Stencil});
    GL::ClearColorBuffer(Color::Zero);
    UnBind();

    LoadSavedDrawBuffers();
}

void GBuffer::ClearDepth()
{
    Framebuffer::ClearDepth();

    SaveCurrentDrawBuffers();
    SetDrawBuffers({GBuffer::Attachment::Depth});
    GL::ClearDepthBuffer(1.0f);
    GL::ClearColorBuffer(Color::One);
    LoadSavedDrawBuffers();
}

void GBuffer::ClearAllBuffersExceptColor()
{
    SaveCurrentDrawBuffers();

    Bind();
    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties
                   });
    GL::ClearColorBuffer();
    ClearDepth();
    ClearStencil();
    UnBind();

    LoadSavedDrawBuffers();
}

void GBuffer::ClearBuffersAndBackground(const ::Color &backgroundColor,
                                        const ::Color &clearValue)
{
    Bind();

    SaveCurrentDrawBuffers();

    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties,
                    GBuffer::Attachment::Color});

    GL::ClearColorBuffer(clearValue);

    // Clear stored color to backgroundColor
    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);

    // Clear stored depth
    SetDrawBuffers({GBuffer::Attachment::Depth});
    GL::ClearColorBuffer(Color::One);

    ClearDepth();
    ClearStencil();

    LoadSavedDrawBuffers();

    UnBind();
}
