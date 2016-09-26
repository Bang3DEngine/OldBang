#include "GBuffer.h"

#include "Color.h"
#include "Array.h"
#include "Screen.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    m_positionTexture = new TextureRender();
    m_normalTexture   = new TextureRender();
    m_uvTexture       = new TextureRender();
    m_diffuseTexture  = new TextureRender();
    m_matPropsTexture = new TextureRender();
    m_depthTexture    = new TextureRender();
    m_stencilTexture  = new TextureRender();
    m_colorTexture    = new TextureRender();

    SetColorAttachment(Attachment::Position,           m_positionTexture);
    SetColorAttachment(Attachment::Normal,             m_normalTexture);
    SetColorAttachment(Attachment::Uv,                 m_uvTexture);
    SetColorAttachment(Attachment::Diffuse,            m_diffuseTexture);
    SetColorAttachment(Attachment::MaterialProperties, m_matPropsTexture);
    SetColorAttachment(Attachment::Depth,              m_depthTexture);
    SetColorAttachment(Attachment::Stencil,            m_stencilTexture);
    SetColorAttachment(Attachment::Color,              m_colorTexture);
    CreateDepthRenderbufferAttachment();

    String renderToScreenMatFilepath =
            "Assets/Engine/Materials/RenderGBufferToScreen.bmat";
    m_renderGBufferToScreenMaterial =
            AssetsManager::LoadAsset<Material>(renderToScreenMatFilepath);
    m_planeMeshToRenderEntireScreen = MeshFactory::GetPlane();
}

GBuffer::~GBuffer()
{
}

void GBuffer::SetUniformsBeforeRendering(Material *mat) const
{
    NONULL(mat);
    ShaderProgram *sp = mat->GetShaderProgram(); NONULL(sp);

    // Color Attachments bindings as Shader Inputs
    sp->SetUniformTexture("B_position_gout_fin",           m_positionTexture, false);
    sp->SetUniformTexture("B_normal_gout_fin",             m_normalTexture,   false);
    sp->SetUniformTexture("B_uv_gout_fin",                 m_uvTexture,       false);
    sp->SetUniformTexture("B_diffuse_gout_fin",            m_diffuseTexture,  false);
    sp->SetUniformTexture("B_materialProps_gout_fin",      m_matPropsTexture, false);
    sp->SetUniformTexture("B_depth_gout_fin",              m_depthTexture,    false);
    sp->SetUniformTexture("B_stencil_gout_fin",            m_stencilTexture,  false);
    sp->SetUniformTexture("B_color_gout_fin",              m_colorTexture,    false);

    Camera *camera = Scene::GetActiveScene()->GetCamera();
    if (camera)
    {
        sp->SetUniformVec3(ShaderContract::Uniform_Position_Camera,
                           camera->transform->GetPosition(), false);
    }

    // Stencil uniforms
    sp->SetUniformFloat("B_stencilWriteEnabled", m_stencilWriteEnabled ? 1.0f : 0.0f);
    sp->SetUniformFloat("B_stencilTestEnabled",  m_stencilTestEnabled  ? 1.0f : 0.0f);
}


void GBuffer::RenderPassWithMaterial(Material *mat) const
{
    NONULL(mat);
    m_planeMeshToRenderEntireScreen->
            BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                         *(mat->GetShaderProgram()));

    SetUniformsBeforeRendering(mat);
    mat->Bind();

    // Set as only draw output: "B_color_gout_gin". To accumulate color in there
    Array<int> previousDrawAttIds = GetCurrentDrawAttachmentIds();
    SetColorDrawBuffer();

    // FAILING HERE
    glDepthMask(GL_FALSE);
    RenderScreenPlane();
    glDepthMask(GL_TRUE);

    mat->UnBind();

    SetDrawBuffers(previousDrawAttIds); // Restore previous draw buffers
}

void GBuffer::RenderToScreen(GBuffer::Attachment attachmentId) const
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    glClear(GL_DEPTH_BUFFER_BIT);

    m_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(m_renderGBufferToScreenMaterial->GetShaderProgram()));

    ShaderProgram *sp = m_renderGBufferToScreenMaterial->GetShaderProgram(); NONULL(sp);
    Texture *tex = GetColorAttachment(attachmentId); NONULL(tex);
    sp->SetUniformTexture("B_color_gout_fin", tex, false);

    m_renderGBufferToScreenMaterial->Bind();
    RenderScreenPlane();
    m_renderGBufferToScreenMaterial->UnBind();
}

void GBuffer::RenderScreenPlane() const
{
    m_planeMeshToRenderEntireScreen->GetVAO()->Bind();

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, m_planeMeshToRenderEntireScreen->GetVertexCount());
    glDepthFunc(GL_LESS);

    m_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
}

void GBuffer::SaveCurrentDrawBuffers() const
{
    m_previousDrawAttachmentsIds = GetCurrentDrawAttachmentIds();
}

void GBuffer::LoadSavedDrawBuffers() const
{
    SetDrawBuffers(m_previousDrawAttachmentsIds);
}

void GBuffer::RenderToScreen() const
{
    RenderToScreen(GBuffer::Attachment::Color);
}

void GBuffer::SetAllDrawBuffersExceptColor() const
{
    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Uv,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties,
                    GBuffer::Attachment::Depth,
                    GBuffer::Attachment::Stencil});
}

void GBuffer::SetStencilDrawBuffer() const
{
    SetDrawBuffers({GBuffer::Attachment::Stencil});
}

void GBuffer::SetColorDrawBuffer() const
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

void GBuffer::ClearStencil() const
{
    SaveCurrentDrawBuffers();

    Bind();
    SetStencilDrawBuffer();
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    UnBind();

    LoadSavedDrawBuffers();
}

void GBuffer::ClearAllBuffersExceptColor()
{
    SaveCurrentDrawBuffers();

    Bind();
    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Uv,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties});
    glClear(GL_COLOR_BUFFER_BIT);
    ClearDepth();
    ClearStencil();
    UnBind();

    LoadSavedDrawBuffers();
}

void GBuffer::ClearBuffersAndBackground(const ::Color &backgroundColor, const ::Color &clearValue)
{
    Bind();

    SaveCurrentDrawBuffers();

    SetDrawBuffers({GBuffer::Attachment::Position,
                    GBuffer::Attachment::Normal,
                    GBuffer::Attachment::Uv,
                    GBuffer::Attachment::Diffuse,
                    GBuffer::Attachment::MaterialProperties});

    glClearColor(clearValue.r, clearValue.g, clearValue.b, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear stored color to backgroundColor
    SetColorDrawBuffer();
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear stored depth
    SetDrawBuffers({GBuffer::Attachment::Depth});
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ClearDepth();
    ClearStencil();

    LoadSavedDrawBuffers();
    UnBind();
}
