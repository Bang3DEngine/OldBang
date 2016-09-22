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
    m_colorTexture    = new TextureRender();
    m_colorTexture->SetGLFormat(GL_RGBA);
    m_colorTexture->SetGLInternalFormat(GL_RGBA);
    m_colorTexture->SetGLType(GL_FLOAT);

    SetColorAttachment(Attachment::Position,           m_positionTexture);
    SetColorAttachment(Attachment::Normal,             m_normalTexture);
    SetColorAttachment(Attachment::Uv,                 m_uvTexture);
    SetColorAttachment(Attachment::Diffuse,            m_diffuseTexture);
    SetColorAttachment(Attachment::MaterialProperties, m_matPropsTexture);
    SetColorAttachment(Attachment::Depth,              m_depthTexture);
    SetColorAttachment(Attachment::Color,              m_colorTexture);
    CreateDepthStencilRenderbufferAttachment();

    String renderToScreenMatFilepath =
            "Assets/Engine/Materials/RenderGBufferToScreen.bmat";
    m_renderGBufferToScreenMaterial =
            AssetsManager::LoadAsset<Material>(renderToScreenMatFilepath);
    m_planeMeshToRenderEntireScreen = MeshFactory::GetPlane();
}

GBuffer::~GBuffer()
{
}

void GBuffer::BindInputTexturesTo(Material *mat) const
{
    ShaderProgram *sp =mat->GetShaderProgram(); NONULL(sp);
    sp->SetUniformTexture("B_position_gout_fin",           m_positionTexture, false);
    sp->SetUniformTexture("B_normal_gout_fin",             m_normalTexture,   false);
    sp->SetUniformTexture("B_uv_gout_fin",                 m_uvTexture,       false);
    sp->SetUniformTexture("B_diffuse_gout_fin",            m_diffuseTexture,  false);
    sp->SetUniformTexture("B_materialProps_gout_fin",      m_matPropsTexture, false);
    sp->SetUniformTexture("B_depth_gout_fin",              m_depthTexture,    false);
    sp->SetUniformTexture("B_color_gout_fin",              m_colorTexture,    false);
}

void GBuffer::RenderPassWithMaterial(Material *mat) const
{
    NONULL(mat);
    m_planeMeshToRenderEntireScreen->
            BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                         *(mat->GetShaderProgram()));

    BindInputTexturesTo(mat);
    //mat->GetShaderProgram()->SetUniformTexture("B_color_gout_fin", m_colorTexture, false);
    mat->Bind();

    // Set as only draw output: "B_color_gout_gin". To accumulate color in there
    //Array<int> previousDrawAttIds = GetCurrentDrawAttachmentIds();
    SetColorDrawBuffer();

    Camera *camera = Scene::GetActiveScene()->GetCamera();
    if (camera)
    {
        mat->GetShaderProgram()->SetUniformVec3(
                    ShaderContract::Uniform_Position_Camera, camera->transform->GetPosition(), false);
    }

    // FAILING HERE
    //glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    RenderScreenPlane();
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);

    mat->UnBind();

    //SetDrawBuffers(previousDrawAttIds); // Restore previous draw buffers
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

    glDrawArrays(GL_TRIANGLES, 0, m_planeMeshToRenderEntireScreen->GetVertexCount());

    m_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
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
                    GBuffer::Attachment::Depth});
}

void GBuffer::SetColorDrawBuffer() const
{
    SetDrawBuffers({GBuffer::Attachment::Color});
}

void GBuffer::ClearAllBuffersExceptColor()
{
    Bind();
    SetAllDrawBuffersExceptColor();
    glClear(GL_COLOR_BUFFER_BIT);
    UnBind();
}

void GBuffer::ClearBuffersAndBackground(const ::Color &backgroundColor, const ::Color &clearValue)
{
    Bind();

    SetAllDrawBuffers();
    glClear(GL_STENCIL_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    Array<int> previousDrawAttIds = GetCurrentDrawAttachmentIds();

    SetAllDrawBuffers();
    glClearColor(clearValue.r, clearValue.g, clearValue.b, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear stored color to backgroundColor
    SetDrawBuffers({GBuffer::Attachment::Color});
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear stored depth
    SetDrawBuffers({GBuffer::Attachment::Depth});
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ClearDepth();
    ClearStencil();

    SetDrawBuffers(previousDrawAttIds); // Restore previous draw buffers
    UnBind();
}
