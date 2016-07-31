#include "MeshRenderer.h"
#include "GameObject.h"
#include "FileReader.h"

MeshRenderer::MeshRenderer()
{
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
    glDrawArrays(GLint(m_renderMode), 0, m_mesh->GetVertexCount());
    m_mesh->GetVAO()->UnBind();
}

#ifdef BANG_EDITOR
void MeshRenderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void MeshRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void MeshRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Renderer::ReadXMLInfo(xmlInfo);
    SetMesh( AssetsManager::GetAsset<Mesh>( xmlInfo->GetFilepath("MeshAssetFilepath") ) );
}

void MeshRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Renderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("MeshRenderer");

    xmlInfo->GetAttribute("LineWidth")->SetProperty(XMLProperty::Hidden);
    xmlInfo->SetFilepath("MeshAssetFilepath", m_mesh ? m_mesh->GetFilepath() : "", "*.bmesh");
}
