#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    const GLint texInternalType = GL_FLOAT;
    CreateColorAttachment(Attachment::Position,      GL_RGBA32F, GL_RGBA,  texInternalType);
    CreateColorAttachment(Attachment::Normal,        GL_RGBA32F, GL_RGBA,  texInternalType);
    CreateColorAttachment(Attachment::Uv,            GL_RGBA32F, GL_RGBA,  texInternalType);
    CreateColorAttachment(Attachment::Diffuse,       GL_RGBA,    GL_RGBA, texInternalType);
    CreateColorAttachment(Attachment::MaterialBools, GL_RGBA,    GL_RGBA, texInternalType);
    CreateColorAttachment(Attachment::Depth,         GL_RGBA,    GL_RGBA, texInternalType);
    CreateColorAttachment(Attachment::Color,         GL_RGBA,    GL_RGBA, texInternalType);
    CreateDepthBufferAttachment();

    p_renderGBufferToScreenMaterial = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/RenderGBufferToScreen.bmat");
    p_planeMeshToRenderEntireScreen = MeshFactory::GetPlane();
}

GBuffer::~GBuffer()
{
    if(p_renderGBufferToScreenMaterial) delete p_renderGBufferToScreenMaterial;
    if(p_planeMeshToRenderEntireScreen) delete p_planeMeshToRenderEntireScreen;
}

void GBuffer::BindGBufferInTexturesTo(Material *mat) const
{
    TextureRender *positionTex  = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex    = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex        = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex   = GetTextureAttachment(GBuffer::Attachment::Diffuse);
    TextureRender *matBoolsTex  = GetTextureAttachment(GBuffer::Attachment::MaterialBools);
    TextureRender *depthTex     = GetTextureAttachment(GBuffer::Attachment::Depth);
    TextureRender *colorTex     = GetTextureAttachment(GBuffer::Attachment::Color);

    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    mat->SetTexture(positionTex,  "B_position_gout_fin");
    mat->SetTexture(normalTex,    "B_normal_gout_fin");
    mat->SetTexture(uvTex,        "B_uv_gout_fin");
    mat->SetTexture(diffuseTex,   "B_diffuse_gout_fin");
    mat->SetTexture(matBoolsTex,  "B_materialBools_gout_fin");
    mat->SetTexture(depthTex,     "B_depth_gout_fin");
    mat->SetTexture(colorTex,     "B_color_gout_fin");
}

void GBuffer::RenderPassWithMaterial(Material *mat) const
{
    NONULL(mat);

    p_planeMeshToRenderEntireScreen->
            BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                         *(mat->GetShaderProgram()));

    BindGBufferInTexturesTo(mat);

    mat->Bind();

    // Set as only draw output: "B_color_gout_gin". To accumulate color in there
    SetDrawBuffers({GBuffer::Attachment::Color});

    RenderScreenPlane();

    mat->UnBind();
}

void GBuffer::RenderScreenPlane() const
{
    p_planeMeshToRenderEntireScreen->GetVAO()->Bind();

    glDepthFunc(GL_LEQUAL); //Overwrite last screen plane!
    glDrawArrays(GL_TRIANGLES, 0, p_planeMeshToRenderEntireScreen->GetVertexCount());

    p_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
}

void GBuffer::RenderToScreen() const
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(p_renderGBufferToScreenMaterial->GetShaderProgram()));

    TextureRender *colorTex = GetTextureAttachment(GBuffer::Attachment::Color);
    p_renderGBufferToScreenMaterial->SetTexture(colorTex, "B_color_gout_fin");

    p_renderGBufferToScreenMaterial->Bind();
    RenderScreenPlane();
    p_renderGBufferToScreenMaterial->UnBind();
}
