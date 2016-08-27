#include "GBuffer.h"

#include "Screen.h"
#include "Color.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    m_positionTexture = new TextureRender();
    m_normalTexture   = new TextureRender();
    m_uvTexture       = new TextureRender();
    m_diffuseTexture  = new TextureRender();
    m_matBoolsTexture = new TextureRender();
    m_depthTexture    = new TextureRender();
    m_colorTexture    = new TextureRender();

    SetColorAttachment(Attachment::Position,      m_positionTexture);
    SetColorAttachment(Attachment::Normal,        m_normalTexture);
    SetColorAttachment(Attachment::Uv,            m_uvTexture);
    SetColorAttachment(Attachment::Diffuse,       m_diffuseTexture);
    SetColorAttachment(Attachment::MaterialBools, m_matBoolsTexture);
    SetColorAttachment(Attachment::Depth,         m_depthTexture);
    SetColorAttachment(Attachment::Color,         m_colorTexture);
    CreateDepthRenderbufferAttachment();

    String renderToScreenMatFilepath =
            "Assets/Engine/Materials/RenderGBufferToScreen.bmat";
    m_renderGBufferToScreenMaterial =
            AssetsManager::LoadAsset<Material>(renderToScreenMatFilepath);
    m_planeMeshToRenderEntireScreen = MeshFactory::GetPlane();
}

GBuffer::~GBuffer()
{
    if (m_renderGBufferToScreenMaterial) delete m_renderGBufferToScreenMaterial;
    if (m_planeMeshToRenderEntireScreen) delete m_planeMeshToRenderEntireScreen;
}

void GBuffer::BindGBufferInTexturesTo(Material *mat) const
{
    ShaderProgram *sp =mat->GetShaderProgram(); NONULL(sp);
    sp->SetUniformTexture("B_position_gout_fin",      m_positionTexture, false);
    sp->SetUniformTexture("B_normal_gout_fin",        m_normalTexture,   false);
    sp->SetUniformTexture("B_uv_gout_fin",            m_uvTexture,       false);
    sp->SetUniformTexture("B_diffuse_gout_fin",       m_diffuseTexture,  false);
    sp->SetUniformTexture("B_materialBools_gout_fin", m_matBoolsTexture, false);
    sp->SetUniformTexture("B_depth_gout_fin",         m_depthTexture,    false);
    sp->SetUniformTexture("B_color_gout_fin",         m_colorTexture,    false);
}

void GBuffer::RenderPassWithMaterial(Material *mat) const
{
    NONULL(mat);

    m_planeMeshToRenderEntireScreen->
            BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                         *(mat->GetShaderProgram()));

    BindGBufferInTexturesTo(mat);

    mat->Bind();

    // Set as only draw output: "B_color_gout_gin". To accumulate color in there
    SetDrawBuffers({GBuffer::Attachment::Color});

    mat->GetShaderProgram()->SetUniformVec2("B_screen_size", Screen::GetSize(), false);

    RenderScreenPlane();

    mat->UnBind();
}

void GBuffer::RenderScreenPlane() const
{
    m_planeMeshToRenderEntireScreen->GetVAO()->Bind();

    glDepthFunc(GL_LEQUAL); //Overwrite last screen plane!
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, m_planeMeshToRenderEntireScreen->GetVertexCount());

    m_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
}

void GBuffer::RenderToScreen() const
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(m_renderGBufferToScreenMaterial->GetShaderProgram()));

    TextureRender *colorTex = GetColorAttachment(GBuffer::Attachment::Color);
    ShaderProgram *sp =m_renderGBufferToScreenMaterial->GetShaderProgram(); NONULL(sp);
    sp->SetUniformTexture("B_color_gout_fin", colorTex, false);

    m_renderGBufferToScreenMaterial->Bind();
    RenderScreenPlane();
    m_renderGBufferToScreenMaterial->UnBind();
}

void GBuffer::ClearBuffersAndBackground(const ::Color &backgroundColor, const ::Color &clearValue)
{
    Bind();

    SetAllDrawBuffers();
    glClearColor(clearValue.r, clearValue.g, clearValue.b, clearValue.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetDrawBuffers({GBuffer::Attachment::Color});
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UnBind();
}
