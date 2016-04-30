#include "MeshRenderer.h"
#include "Entity.h"
#include "StageReader.h"

MeshRenderer::MeshRenderer()
{
    #ifdef BANG_EDITOR
        inspectorPartInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material", Material::GetFileExtensionStatic()),
            new InspectorFileSWInfo("Mesh", Mesh::GetFileExtensionStatic()),
        });
    #endif
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMaterial(Material *m)
{
    material = m;
    if(mesh != nullptr && material != nullptr && material->GetShaderProgram() != nullptr)
    {
        mesh->BindAllVBOsToShaderProgram(*(material->GetShaderProgram()));
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

void MeshRenderer::_OnRender()
{
    Stage *stage = GetOwner()->GetStage();
    Camera *cam = stage->GetCamera();
    if(CAN_USE_PART(cam))
    {
        Render();
    }
    else
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
                       << stage << " does not have a set Camera.");
    }
}

void MeshRenderer::Render() const
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
    glm::mat4 view(1.0f), projection(1.0f);
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
    glDrawArrays(mesh->GetRenderMode(), 0, mesh->GetVertexCount());
    material->UnBind();
    mesh->GetVAO()->UnBind();
}


#ifdef BANG_EDITOR
InspectorWidgetInfo* MeshRenderer::GetPartInfo()
{
    InspectorFileSWInfo* matInfo, *meshInfo;
    matInfo  = static_cast<InspectorFileSWInfo*>(inspectorPartInfo.GetSlotInfo(0));
    meshInfo = static_cast<InspectorFileSWInfo*>(inspectorPartInfo.GetSlotInfo(1));

    if (material != nullptr)
    {
        if(material->GetFilepath() != "")
        {
            matInfo->filepath = material->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(matInfo->filepath, (void*)material);
        }
    }
    else
    {
        matInfo->filepath = "-";
    }

    if (mesh != nullptr)
    {
        if(mesh->GetFilepath() != "")
        {
            meshInfo->filepath = mesh->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(meshInfo->filepath, (void*)mesh);
        }
    }
    else
    {
        meshInfo->filepath = "-";
    }

    return &inspectorPartInfo;
}

void MeshRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    std::string meshFilepath = source->GetSWFileFilepath("Mesh");

    if(materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    if(meshFilepath != "")
    {
        SetMesh( AssetsManager::GetAsset<Mesh>(meshFilepath) );
    }
    else { }
}


void MeshRenderer::Write(std::ostream &f) const
{
    f << "<MeshRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(mesh->GetFilepath(), f);
    FileWriter::WriteFilepath(material->GetFilepath(), f);
    f << "</MeshRenderer>" << std::endl;
}

void MeshRenderer::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this);
    SetMesh( AssetsManager::GetAsset<Mesh>( FileReader::ReadString(f) ) );
    SetMaterial( AssetsManager::GetAsset<Material>( FileReader::ReadString(f) ) );
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif
