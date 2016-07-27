#include "MeshRenderer.h"
#include "GameObject.h"
#include "FileReader.h"

MeshRenderer::MeshRenderer()
{
    #ifdef BANG_EDITOR
        m_inspectorInfo.AddSlotInfos(
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

    Matrix4 model, normal, view, projection, pvm;
    GetMatrices(&model, &normal, &view, &projection, &pvm);
    SetMatricesUniforms(model, normal, view, projection, pvm);

    m_mesh->GetVAO()->Bind();
    glDrawArrays(m_renderMode, 0, m_mesh->GetVertexCount());
    m_mesh->GetVAO()->UnBind();
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* MeshRenderer::OnInspectorInfoNeeded()
{
    Renderer::OnInspectorInfoNeeded();

    InspectorSWInfo *meshInfo = m_inspectorInfo.GetSlotInfo("Mesh");
    if (m_mesh)
    {
        if (m_mesh->GetFilepath() != "")
        {
            meshInfo->SetString(m_mesh->GetFilepath());
        }
        else //In case the asset is created in runtime, write its mem address
        {
            std::string memAddress;
            Logger_GetString(memAddress, (void*)m_mesh);
            meshInfo->SetString(memAddress);
        }
    }
    else
    {
        meshInfo->SetString("-");
    }

    return &m_inspectorInfo;
}

void MeshRenderer::OnInspectorInfoChanged(InspectorWidgetInfo *info)
{
    Renderer::OnInspectorInfoChanged(info);

    std::string meshFilepath = info->GetString("Mesh");
    if (meshFilepath != "")
    {
        SetMesh( AssetsManager::GetAsset<Mesh>(meshFilepath) );
    }
}
#endif

void MeshRenderer::ReadXMLNode(const XMLNode *xmlNode)
{
    Renderer::ReadXMLNode(xmlNode);
    SetMesh( AssetsManager::GetAsset<Mesh>( xmlNode->GetString("meshAssetFilepath") ) );
    SetMaterial( AssetsManager::GetAsset<Material>( xmlNode->GetString("materialAssetFilepath") ) );
}

void MeshRenderer::GetXMLNode(XMLNode *xmlNode) const
{
    Renderer::GetXMLNode(xmlNode);
    xmlNode->SetTagName("MeshRenderer");

    xmlNode->AddAttribute("meshAssetFilepath", m_mesh ? m_mesh->GetFilepath() : "");
    xmlNode->AddAttribute("materialAssetFilepath", m_material ? m_material->GetFilepath() : "");
}
