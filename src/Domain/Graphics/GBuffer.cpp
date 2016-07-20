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
    CreateDepthBufferAttachment();

    p_renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_PR_Default_VS,
                                          ShaderContract::Filepath_Shader_PR_Default_FS);
    p_renderToScreenMaterial->SetShaderProgram(sp);

    p_renderToScreenPlaneMesh = MeshFactory::GetPlane();
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
}

void GBuffer::RenderToScreenWithMaterial(Material *mat) const
{
    if(mat)
    {
        p_renderToScreenPlaneMesh->
                BindPositionsToShaderProgram(ShaderContract::Attr_Vertex_In_Position_Raw,
                                             *(mat->GetShaderProgram()));
        BindTexturesTo(mat);
        mat->Bind();
    }

    p_renderToScreenPlaneMesh->GetVAO()->Bind();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    //Render the screen plane!
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, p_renderToScreenPlaneMesh->GetVertexCount());

    p_renderToScreenPlaneMesh->GetVAO()->UnBind();

    if(mat)
    {
        mat->UnBind();
    }
}

void GBuffer::RenderToScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderToScreenWithMaterial(p_renderToScreenMaterial);
}
