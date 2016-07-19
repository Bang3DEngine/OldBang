#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment(Attachment::Position,      GL_RGBA32F, GL_RGBA);
    CreateColorAttachment(Attachment::Normal,        GL_RGBA32F, GL_RGBA);
    CreateColorAttachment(Attachment::Uv,            GL_RGBA32F, GL_RGBA);
    CreateColorAttachment(Attachment::Diffuse,       GL_RGBA32F, GL_RGBA);
    CreateColorAttachment(Attachment::MaterialBools, GL_RGBA32F, GL_RGBA);
    CreateColorAttachment(Attachment::Depth,         GL_RGBA32F, GL_RGBA);
    CreateDepthBufferAttachment();

    p_renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_PR_Default_VS,
                                          ShaderContract::Filepath_Shader_PR_Default_FS);
    p_renderToScreenMaterial->SetShaderProgram(sp);

    p_renderToScreenPlaneMesh = MeshFactory::GetPlane();
    p_renderToScreenPlaneMesh->BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                                            *(p_renderToScreenMaterial->GetShaderProgram()));
}

GBuffer::~GBuffer()
{
    if(p_renderToScreenMaterial ) delete p_renderToScreenMaterial;
    if(p_renderToScreenPlaneMesh ) delete p_renderToScreenPlaneMesh;
}

void GBuffer::BindTexturesTo(Material *mat) const
{
    TextureRender *positionTex  = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex    = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex        = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex   = GetTextureAttachment(GBuffer::Attachment::Diffuse);
    TextureRender *matBoolsTex  = GetTextureAttachment(GBuffer::Attachment::MaterialBools);
    TextureRender *depthTex     = GetTextureAttachment(GBuffer::Attachment::Depth);

    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    mat->SetTexture(positionTex,  "B_gout_fin_position");
    mat->SetTexture(normalTex,    "B_gout_fin_normal");
    mat->SetTexture(uvTex,        "B_gout_fin_uv");
    mat->SetTexture(diffuseTex,   "B_gout_fin_diffuse");
    mat->SetTexture(matBoolsTex,  "B_gout_fin_materialBools");
    mat->SetTexture(depthTex,     "B_gout_fin_depth");
    mat->Bind();
}

void GBuffer::RenderToScreenWithoutMaterial() const
{
    p_renderToScreenPlaneMesh->GetVAO()->Bind();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDepthFunc(GL_LEQUAL);

    //Render the screen plane!
    glDrawArrays(GL_TRIANGLES, 0, p_renderToScreenPlaneMesh->GetVertexCount());

    p_renderToScreenPlaneMesh->GetVAO()->UnBind();
}

void GBuffer::RenderToScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    BindTexturesTo(p_renderToScreenMaterial);

    p_renderToScreenMaterial->Bind();
    RenderToScreenWithoutMaterial();
    p_renderToScreenMaterial->UnBind();
}
