#include "MeshRenderer.h"
#include "Entity.h"

MeshRenderer::MeshRenderer() : mesh(nullptr), material(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::_OnRender()
{
    Stage *stage = GetParent()->GetStage();
    Camera *cam = stage->GetCamera();
    if(cam != nullptr)
    {
        Render(MeshRenderer::RenderMode::Triangles);
    }
    else
    {
        Logger_Warning("Can't render " << GetParent() << " because "
                       << stage << " does not have a set Camera.");
    }
}

void MeshRenderer::SetMesh(const Mesh *m)
{
    mesh = m;
}

const Mesh *MeshRenderer::GetMesh()
{
    return mesh;
}

void MeshRenderer::SetMaterial(const Material *m)
{
    material = m;
}

const Material *MeshRenderer::GetMaterial()
{
    return material;
}

void MeshRenderer::Render(MeshRenderer::RenderMode drawingMode) const
{
    Transform *t = parent->GetPart<Transform>();
    if(t == nullptr)
    {
        Logger_Error(parent << "does not have a Transform. Can't render.");
        return;
    }

    if(mesh == nullptr)
    {
        Logger_Error(parent << " does not have a Mesh. Can't render.");
        return;
    }

    if(material == nullptr)
    {
        Logger_Error(parent << " does not have a Material. Can't render.");
        return;
    }
    else
    {
        if(material->GetShaderProgram() == nullptr)
        {
            Logger_Error(parent << " has a Material with no ShaderProgram. Can't render.");
            return;
        }
    }

    glm::mat4 model; t->GetMatrix(model);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    //In case the parent stage has a camera, retrieve the view and proj matrices
    Camera *camera = parent->GetStage()->GetCamera();
    if(camera != nullptr)
    {
        camera->GetViewMatrix(view);
        camera->GetProjectionMatrix(projection);
    }

    glm::mat4 pvm = projection * view * model;

    mesh->GetVAO()->Bind();

    material->Bind();

    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_Model, model, false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_View, view, false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection, projection, false);
    material->shaderProgram->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM, pvm, false);

    glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

    material->UnBind();

    mesh->GetVAO()->UnBind();
}
