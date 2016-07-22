#include "MeshRenderer.h"
#include "GameObject.h"
#include "FileReader.h"

MeshRenderer::MeshRenderer()
{
    #ifdef BANG_EDITOR
        m_inspectorComponentInfo.SetSlotsInfos(
        {
            new InspectorFileSWInfo("Material",
                                    Material::GetFileExtensionStatic()),
            new InspectorFileSWInfo("Mesh", Mesh::GetFileExtensionStatic()),
        });
#endif
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = static_cast<MeshRenderer*>(clone);
    mr->SetMesh(m_mesh);
}

ICloneable *MeshRenderer::Clone() const
{
    MeshRenderer *mr = new MeshRenderer();
    CloneInto(mr);
    return mr;
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMaterial(Material *m)
{
    m_material = m;
    if (m_mesh  && m_material  &&
       m_material->GetShaderProgram() )
    {
        m_mesh->BindAllVBOsToShaderProgram(*(m_material->GetShaderProgram()));
    }
}

void MeshRenderer::SetMesh(Mesh *m)
{
    m_mesh = m;
    NONULL(m_mesh); NONULL(m_material); NONULL(m_material->GetShaderProgram());

    m_mesh->BindAllVBOsToShaderProgram(*(m_material->GetShaderProgram()));
    if (m_mesh->GetFilepath().length() > 0)
    {
        SetRenderMode(m_mesh->IsATrianglesModel() ?
                      RenderMode::Triangles : RenderMode::Quads);
    }
}

Box MeshRenderer::GetBoundingBox() const
{
    if (m_mesh )
    {
        return m_mesh->GetBoundingBox();
    }
    else
    {
        return Box();
    }
}

const Mesh *MeshRenderer::GetMesh()
{
    return m_mesh;
}

const std::string MeshRenderer::ToString() const
{
    std::ostringstream oss;
    oss << "MeshRenderer: [" << std::endl <<
           "   " << m_mesh << std::endl <<
           "   " << m_material << std::endl <<
           "]";
    return oss.str();
}

std::string MeshRenderer::GetName() const
{
    return "MeshRenderer";
}

void MeshRenderer::RenderWithoutBindingMaterial() const
{
    NONULL(m_mesh);

    Matrix4 model, view, projection, pvm;
    GetMatrices(model, view, projection, pvm);
    SetMatricesUniforms(model, view, projection, pvm);

    m_mesh->GetVAO()->Bind();
    glDrawArrays(m_renderMode, 0, m_mesh->GetVertexCount());
    m_mesh->GetVAO()->UnBind();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* MeshRenderer::GetComponentInfo()
{
    InspectorFileSWInfo* matInfo, *meshInfo;
    matInfo  = static_cast<InspectorFileSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(0));
    meshInfo = static_cast<InspectorFileSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(1));

    if (m_material )
    {
        if (m_material->GetFilepath() != "")
        {
            matInfo->filepath = m_material->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(matInfo->filepath, (void*)m_material);
        }
    }
    else
    {
        matInfo->filepath = "-";
    }

    if (m_mesh )
    {
        if (m_mesh->GetFilepath() != "")
        {
            meshInfo->filepath = m_mesh->GetFilepath();
        }
        else //In case the asset is created in runtime, write its mem address
        {
            Logger_GetString(meshInfo->filepath, (void*)m_mesh);
        }
    }
    else
    {
        meshInfo->filepath = "-";
    }

    return &m_inspectorComponentInfo;
}

void MeshRenderer::OnSlotValueChanged(InspectorWidget *source)
{
    std::string materialFilepath = source->GetSWFileFilepath("Material");
    std::string meshFilepath = source->GetSWFileFilepath("Mesh");

    if (materialFilepath != "")
    {
        SetMaterial( AssetsManager::GetAsset<Material>(materialFilepath) );
    }
    else { }

    if (meshFilepath != "")
    {
        SetMesh( AssetsManager::GetAsset<Mesh>(meshFilepath) );
    }
    else { }
}
#endif


void MeshRenderer::Write(std::ostream &f) const
{
    f << "<MeshRenderer>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::WriteFilepath(m_mesh->GetFilepath(), f);
    FileWriter::WriteFilepath(m_material->GetFilepath(), f);
    f << "</MeshRenderer>" << std::endl;
}

void MeshRenderer::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetMesh( AssetsManager::GetAsset<Mesh>( FileReader::ReadString(f) ) );
    SetMaterial( AssetsManager::GetAsset<Material>( FileReader::ReadString(f) ) );
    FileReader::ReadNextLine(f); //Consume close tag
}

