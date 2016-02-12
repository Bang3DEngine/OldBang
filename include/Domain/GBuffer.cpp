#include "GBuffer.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateDepthBufferAttachment();
    CreateTextureAttachment(Attachment::Position);
    CreateTextureAttachment(Attachment::Normal);
    CreateTextureAttachment(Attachment::Uv);
    CreateTextureAttachment(Attachment::Diffuse);

    renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_Render_To_Screen,
                                          ShaderContract::Filepath_Shader_Fragment_Render_To_Screen);
    renderToScreenMaterial->SetShaderProgram(sp);

    screenPlaneMesh = new MeshScreenPlane();
}

GBuffer::~GBuffer()
{
    if(renderToScreenMaterial != nullptr) delete renderToScreenMaterial;
    if(screenPlaneMesh != nullptr) delete screenPlaneMesh;
}

void GBuffer::RenderToScreen() const
{
    const TextureRender *positionTex = GetTextureAttachment(GBuffer::Attachment::Position);
    const TextureRender *normalTex   = GetTextureAttachment(GBuffer::Attachment::Normal);
    const TextureRender *uvTex       = GetTextureAttachment(GBuffer::Attachment::Uv);
    const TextureRender *diffuseTex  = GetTextureAttachment(GBuffer::Attachment::Diffuse);

    screenPlaneMesh->GetVAO()->Bind();

    renderToScreenMaterial->SetTexture(positionTex);
    renderToScreenMaterial->Bind();

    //Render the screen plane!
    glDrawArrays(screenPlaneMesh->GetRenderMode(), 0, screenPlaneMesh->GetVertexCount());

    renderToScreenMaterial->UnBind();
    screenPlaneMesh->GetVAO()->UnBind();
}
