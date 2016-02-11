#include "Stage.h"
#include "Entity.h"
#include "Canvas.h"

Stage::Stage() : cameraEntity(nullptr)
{
    geometryFramebuffer = new Framebuffer(Canvas::GetWidth(), Canvas::GetHeight());
    geometryFramebuffer->CreateDepthBufferAttachment();
    geometryFramebuffer->CreateTextureAttachment(GeometryFBAttachment::Position);
    geometryFramebuffer->CreateTextureAttachment(GeometryFBAttachment::Normal);
    geometryFramebuffer->CreateTextureAttachment(GeometryFBAttachment::Uv);
    geometryFramebuffer->CreateTextureAttachment(GeometryFBAttachment::Diffuse);

    renderToScreenMaterial = new Material();
    ShaderProgram *sp = new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_Render_To_Screen,
                                          ShaderContract::Filepath_Shader_Fragment_Render_To_Screen);
    renderToScreenMaterial->SetShaderProgram(sp);
    planeMesh = new MeshPlane();
}

Stage::~Stage()
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = *it;
        child->_OnDestroy();
        this->RemoveChild(child->GetName());
        delete child;
    }

    delete renderToScreenMaterial;
    delete planeMesh;
}

void Stage::_OnRender()
{
    geometryFramebuffer->Bind();
    //All the mesh renderers now will render stuff into the geometryFramebuffer
    PROPAGATE_EVENT(_OnRender, children);
    geometryFramebuffer->UnBind();

    //Get textures where we rendered, and render them to actual screen
    const TextureRender *positionTex = geometryFramebuffer->GetTextureAttachment(GeometryFBAttachment::Position);
    const TextureRender *normalTex   = geometryFramebuffer->GetTextureAttachment(GeometryFBAttachment::Normal);
    const TextureRender *uvTex       = geometryFramebuffer->GetTextureAttachment(GeometryFBAttachment::Uv);
    const TextureRender *diffuseTex  = geometryFramebuffer->GetTextureAttachment(GeometryFBAttachment::Diffuse);

    //Render to screen
    renderToScreenMaterial->SetTexture(positionTex);
    planeMesh->GetVAO()->Bind();
    renderToScreenMaterial->Bind();

    glDrawArrays(planeMesh->GetRenderMode(), 0, planeMesh->GetVertexCount()); //Draw the screen plane!

    renderToScreenMaterial->UnBind();
    planeMesh->GetVAO()->UnBind();
    //
}

void Stage::SetCameraEntity(const Entity *cameraEntity)
{
    if(cameraEntity->HasPart<Camera>())
    {
        this->cameraEntity = cameraEntity;
    }
    else
    {
        Logger_Error("Can't set " << cameraEntity <<
                     " as camera because it does not have a Camera part.");
    }
}

Camera *Stage::GetCamera() const
{
    if(cameraEntity == nullptr) return nullptr;
    return cameraEntity->GetPart<Camera>();
}

const Canvas *Stage::GetCanvas() const
{
    return canvas;
}

