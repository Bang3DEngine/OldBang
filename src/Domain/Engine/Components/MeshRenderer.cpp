#include "MeshRenderer.h"
#include "GameObject.h"
#include "FileReader.h"

MeshRenderer::MeshRenderer()
{
    #ifdef BANG_EDITOR
        inspectorComponentInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material",
                                    Material::GetFileExtensionStatic()),
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
    if(mesh != nullptr && material != nullptr &&
       material->GetShaderProgram() != nullptr)
    {
        mesh->BindAllVBOsToShaderProgram(*(material->GetShaderProgram()));
    }
}

void MeshRenderer::SetMesh(Mesh *m)
{
    mesh = m;
    if(mesh != nullptr && material != nullptr &&
       material->GetShaderProgram() != nullptr)
    {
        mesh->BindAllVBOsToShaderProgram(*(material->GetShaderProgram()));
    }

    if(m->GetFilepath().length() > 0)
    {
        SetRenderMode(m->IsATrianglesModel() ?
                          RenderMode::Triangles : RenderMode::Quads);
    }
}

Box MeshRenderer::GetBoundingBox() const
{
    if(mesh != nullptr)
    {
        return mesh->GetBoundingBox();
    }
    else
    {
        return Box();
    }
}

const Mesh *MeshRenderer::GetMesh()
{
    return mesh;
}

const std::string MeshRenderer::ToString() const
{
    std::ostringstream oss;
    oss << "MeshRenderer: [" << std::endl <<
           "   " << mesh << std::endl <<
           "   " << material << std::endl <<
           "]";
    return oss.str();
}

std::string MeshRenderer::GetName() const
{
    return "MeshRenderer";
}

void MeshRenderer::ActivateStatesBeforeRendering() const
{
    Renderer::ActivateStatesBeforeRendering();
}

void MeshRenderer::RenderWithoutBindingMaterial() const
{
    ActivateStatesBeforeRendering();

    Matrix4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    mesh->GetVAO()->Bind();
    glDrawArrays(renderMode, 0, mesh->GetVertexCount());
    mesh->GetVAO()->UnBind();
}

void MeshRenderer::OnRender()
{
    Camera *cam = GetOwner()->GetScene()->GetCamera();
    if(!CAN_USE_COMPONENT(cam))
    {
        Logger_Warn("Can't render " << GetOwner() << " because "
             << GetOwner()->GetScene() << " does not have a set Camera.");
    }

    Transform *t = owner->GetComponent<Transform>();
    if(!CAN_USE_COMPONENT(t) || mesh == nullptr || material == nullptr)
    {
        if(!CAN_USE_COMPONENT(t))
            Logger_Verbose(owner << " could not be rendered because it does" <<
                           "not have a Transform (or it's disabled')");
        if(mesh == nullptr)
            Logger_Verbose(owner << " could not be rendered because it does" <<
                           "not have a Mesh (or it's disabled')");

        if(material == nullptr)
            Logger_Verbose(owner << " could not be rendered because it does" <<
                           "not have a Material (or it's disabled')");

        return;
    }
    else if(material->GetShaderProgram() == nullptr)
    {
        Logger_Error(owner << " has a Material with no ShaderProgram." <<
                     "Can't render.");
        return;
    }

    //Pass all the checks in order to render ok
    Render();
}

void MeshRenderer::Render() const
{
    material->Bind();
    RenderWithoutBindingMaterial();
    material->UnBind();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* MeshRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo, *meshInfo;
    matInfo  = static_cast<InspectorFileSWInfo*>(inspectorComponentInfo.GetSlotInfo(0));
    meshInfo = static_cast<InspectorFileSWInfo*>(inspectorComponentInfo.GetSlotInfo(1));

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

    return &inspectorComponentInfo;
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
    FileReader::RegisterNextPointerId(f, this);
    SetMesh( AssetsManager::GetAsset<Mesh>( FileReader::ReadString(f) ) );
    SetMaterial( AssetsManager::GetAsset<Material>( FileReader::ReadString(f) ) );
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif
