#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    const GLint texInternalType = GL_FLOAT;
    CreateColorAttachment(Attachment::Position,      GL_RGBA32F, GL_RGB,  texInternalType);
    CreateColorAttachment(Attachment::Normal,        GL_RGBA32F, GL_RGB,  texInternalType);
    CreateColorAttachment(Attachment::Uv,            GL_RGB,     GL_RGB,  texInternalType);
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

void GBuffer::BindInTexturesTo(Material *mat) const
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
    mat->SetTexture(positionTex,  "B_gout_fin_position");
    mat->SetTexture(normalTex,    "B_gout_fin_normal");
    mat->SetTexture(uvTex,        "B_gout_fin_uv");
    mat->SetTexture(diffuseTex,   "B_gout_fin_diffuse");
    mat->SetTexture(matBoolsTex,  "B_gout_fin_materialBools");
    mat->SetTexture(depthTex,     "B_gout_fin_depth");
    mat->SetTexture(colorTex,     "B_gout_fin_color");
}

void GBuffer::RenderPassWithMaterial(Material *mat) const
{
    if(mat)
    {
        p_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(mat->GetShaderProgram()));
        BindInTexturesTo(mat);
        mat->Bind();
    }

    // Set as only draw output: "B_fout_gin_color"
    // To accumulate color in there
    SetDrawBuffers({GBuffer::Attachment::Color});

    RenderScreenPlane();

    if(mat)
    {
        mat->UnBind();
    }
}

void GBuffer::RenderScreenPlane() const
{
    p_planeMeshToRenderEntireScreen->GetVAO()->Bind();

    glEnable(GL_CULL_FACE); glPolygonMode(GL_FRONT, GL_FILL);
    glDepthFunc(GL_LEQUAL); //Overwrite last screen plane!
    glDrawArrays(GL_TRIANGLES, 0, p_planeMeshToRenderEntireScreen->GetVertexCount());

    p_planeMeshToRenderEntireScreen->GetVAO()->UnBind();
}

void GBuffer::RenderToScreen() const
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p_planeMeshToRenderEntireScreen->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(p_renderGBufferToScreenMaterial->GetShaderProgram()));

    TextureRender *colorTex = GetTextureAttachment(GBuffer::Attachment::Color);
    p_renderGBufferToScreenMaterial->SetTexture(colorTex, "B_gout_fin_color");

    p_renderGBufferToScreenMaterial->Bind();
    RenderScreenPlane();
    p_renderGBufferToScreenMaterial->UnBind();
}
