#include "MeshRenderer.h"
#include "Entity.h"
#include "StageReader.h"

MeshRenderer::MeshRenderer()
{
#ifdef BANG_EDITOR
    inspectorPartInfo.slotInfos =
    {
        new InspectorPartInfoSlotEnum(
            "Drawing Mode", {"Triangles", "Quads"}
        )
    };
#endif
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::_OnRender()
{
    Stage *stage = GetOwner()->GetStage();
    Camera *cam = stage->GetCamera();
    if(CAN_USE_PART(cam))
    {
        Render(Mesh::RenderMode::Triangles);
    }
    else
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << stage << " does not have a set Camera.");
    }
}

void MeshRenderer::SetMesh(Mesh *m)
{
    mesh = m;
    if(mesh != nullptr && material != nullptr && material->GetShaderProgram() != nullptr)
    {
        mesh->BindAllVBOsToShaderProgram(*(material->GetShaderProgram()));
    }
}

const Mesh *MeshRenderer::GetMesh()
{
    return mesh;
}

void MeshRenderer::SetMaterial(Material *m)
{
    material = m;
    if(mesh != nullptr && material != nullptr && material->GetShaderProgram() != nullptr)
    {
        mesh->BindAllVBOsToShaderProgram(*(material->GetShaderProgram()));
    }
}

Material *MeshRenderer::GetMaterial()
{
    return material;
}

void MeshRenderer::Write(std::ostream &f) const
{

}

void MeshRenderer::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this);
    SetMesh( AssetsManager::GetAsset<Mesh>( FileReader::ReadString(f) ) );
    SetMaterial( AssetsManager::GetAsset<Material>( FileReader::ReadString(f) ) );
    FileReader::ReadNextLine(f); //Consume close tag
}

const std::string MeshRenderer::ToString() const
{
    return "MeshRenderer";
}

void MeshRenderer::Render(Mesh::RenderMode drawingMode) const
{
    Transform *t = owner->GetPart<Transform>();
    if(!CAN_USE_PART(t) || mesh == nullptr || material == nullptr)
    {
        if(!CAN_USE_PART(t)) Logger_Verbose(owner << " could not be rendered because it does not have a Transform (or it's disabled')");
        if(mesh == nullptr) Logger_Verbose(owner << " could not be rendered because it does not have a Mesh (or it's disabled')");
        if(material == nullptr) Logger_Verbose(owner << " could not be rendered because it does not have a Material (or it's disabled')");
        return;
    }
    else if(material->GetShaderProgram() == nullptr)
    {
        Logger_Error(owner << " has a Material with no ShaderProgram. Can't render.");
        return;
    }

    glm::mat4 model; t->GetMatrix(model);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    //In case the parent stage has a camera, retrieve the view and proj matrices
    Camera *camera = owner->GetStage()->GetCamera();
    if( CAN_USE_PART(camera) )
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

    //Logger_Warning("Rendering " << material->GetShaderProgram());
    glDrawArrays(drawingMode, 0, mesh->GetVertexCount());

    material->UnBind();

    mesh->GetVAO()->UnBind();
}
