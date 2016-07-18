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

void GBuffer::RenderToScreen() const
{
    TextureRender *positionTex = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex   = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex       = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex  = GetTextureAttachment(GBuffer::Attachment::Diffuse);
    TextureRender *depthTex    = GetTextureAttachment(GBuffer::Attachment::Depth);

    p_renderToScreenPlaneMesh->GetVAO()->Bind();

    //Pass the textures to the render to screen shaderProgram, through the Material
    //First set the corresponding texture slots for every texture
    positionTex->SetTextureSlot(GBuffer::Attachment::Position); //0
    normalTex->SetTextureSlot(GBuffer::Attachment::Normal);     //1
    uvTex->SetTextureSlot(GBuffer::Attachment::Uv);             //2
    diffuseTex->SetTextureSlot(GBuffer::Attachment::Diffuse);   //3
    depthTex->SetTextureSlot(GBuffer::Attachment::Depth);       //4

    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    p_renderToScreenMaterial->SetTexture(positionTex, GBuffer::Attachment::Position); //0
    p_renderToScreenMaterial->SetTexture(normalTex,   GBuffer::Attachment::Normal);   //1
    p_renderToScreenMaterial->SetTexture(uvTex,       GBuffer::Attachment::Uv);       //2
    p_renderToScreenMaterial->SetTexture(diffuseTex,  GBuffer::Attachment::Diffuse);  //3
    p_renderToScreenMaterial->SetTexture(depthTex,    GBuffer::Attachment::Depth);    //4
    p_renderToScreenMaterial->Bind();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Render the screen plane!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, p_renderToScreenPlaneMesh->GetVertexCount());

    p_renderToScreenMaterial->UnBind();
    p_renderToScreenPlaneMesh->GetVAO()->UnBind();
}
