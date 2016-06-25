#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateTextureAttachment(Attachment::Position);
    CreateTextureAttachment(Attachment::Normal);
    CreateTextureAttachment(Attachment::Uv);
    CreateTextureAttachment(Attachment::Diffuse);
    CreateTextureAttachment(Attachment::Depth);
    CreateDepthBufferAttachment();

    renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_Render_To_Screen,
                                          ShaderContract::Filepath_Shader_Fragment_Render_To_Screen);
    renderToScreenMaterial->SetShaderProgram(sp);

    renderToScreenPlaneMesh = new MeshScreenPlane();
    renderToScreenPlaneMesh->BindPositionsToShaderProgram(ShaderContract::Vertex_In_Position_Raw,
                                                        *(renderToScreenMaterial->GetShaderProgram()));
}

GBuffer::~GBuffer()
{
    if(renderToScreenMaterial != nullptr) delete renderToScreenMaterial;
    if(renderToScreenPlaneMesh != nullptr) delete renderToScreenPlaneMesh;
}

void GBuffer::RenderToScreen() const
{
    TextureRender *positionTex = GetTextureAttachment(GBuffer::Attachment::Position);
    TextureRender *normalTex   = GetTextureAttachment(GBuffer::Attachment::Normal);
    TextureRender *uvTex       = GetTextureAttachment(GBuffer::Attachment::Uv);
    TextureRender *diffuseTex  = GetTextureAttachment(GBuffer::Attachment::Diffuse);
    TextureRender *depthTex    = GetTextureAttachment(GBuffer::Attachment::Depth);

    renderToScreenPlaneMesh->GetVAO()->Bind();

    //Pass the textures to the render to screen shaderProgram, through the Material
    //First set the corresponding texture slots for every texture
    positionTex->SetTextureSlot(GBuffer::Attachment::Position); //0
    normalTex->SetTextureSlot(GBuffer::Attachment::Normal);     //1
    uvTex->SetTextureSlot(GBuffer::Attachment::Uv);             //2
    diffuseTex->SetTextureSlot(GBuffer::Attachment::Diffuse);   //3
    depthTex->SetTextureSlot(GBuffer::Attachment::Depth);       //4

    //Now attach to the material, with its corresponding index for the name (BANG_texture_0)
    //which in this case are the same as each respective texture slot
    renderToScreenMaterial->SetTexture(positionTex, GBuffer::Attachment::Position); //0
    renderToScreenMaterial->SetTexture(normalTex,   GBuffer::Attachment::Normal);   //1
    renderToScreenMaterial->SetTexture(uvTex,       GBuffer::Attachment::Uv);       //2
    renderToScreenMaterial->SetTexture(diffuseTex,  GBuffer::Attachment::Diffuse);  //3
    renderToScreenMaterial->SetTexture(depthTex,    GBuffer::Attachment::Depth);    //4
    renderToScreenMaterial->Bind();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Render the screen plane!
    glDrawArrays(GL_TRIANGLES, 0, renderToScreenPlaneMesh->GetVertexCount());

    renderToScreenMaterial->UnBind();
    renderToScreenPlaneMesh->GetVAO()->UnBind();
}
