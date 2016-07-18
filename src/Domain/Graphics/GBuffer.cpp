#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment(Attachment::Position);
    CreateColorAttachment(Attachment::Normal);
    CreateColorAttachment(Attachment::Uv);
    CreateColorAttachment(Attachment::Diffuse);
    CreateColorAttachment(Attachment::Depth);
    CreateDepthBufferAttachment();

    p_renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_Render_To_Screen,
                                          ShaderContract::Filepath_Shader_Fragment_Render_To_Screen);
    p_renderToScreenMaterial->SetShaderProgram(sp);

    p_renderToScreenPlaneMesh = MeshFactory::GetPlane();
    p_renderToScreenPlaneMesh->BindPositionsToShaderProgram(ShaderContract::Vertex_In_Position_Raw,
                                                            *(p_renderToScreenMaterial->GetShaderProgram()));
}

GBuffer::~GBuffer()
{
    if(p_renderToScreenMaterial ) delete p_renderToScreenMaterial;
    if(p_renderToScreenPlaneMesh ) delete p_renderToScreenPlaneMesh;
}

void GBuffer::BindTexturesTo(Material *mat) const
{
    TextureRender *positionTex = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex   = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex       = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex  = GetTextureAttachment(GBuffer::Attachment::Diffuse);
    TextureRender *depthTex    = GetTextureAttachment(GBuffer::Attachment::Depth);

    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    mat->SetTexture(positionTex, "BANG_gout_fin_position");
    mat->SetTexture(normalTex,   "BANG_gout_fin_normal");
    mat->SetTexture(uvTex,       "BANG_gout_fin_uv");
    mat->SetTexture(diffuseTex,  "BANG_gout_fin_diffuse");
    mat->SetTexture(depthTex,    "BANG_gout_fin_depth");
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
