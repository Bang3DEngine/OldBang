#include "MeshRenderer.h"
#include "GameObject.h"
#include "FileReader.h"

#include "Debug.h"

#include "VAO.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "AssetsManager.h"

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
    ASSERT(m_mesh); ASSERT(m_material); ASSERT(m_material->GetShaderProgram());

    m_mesh->BindAllVBOsToShaderProgram(*(m_material->GetShaderProgram()));
    if (!m_mesh->GetFilepath().Empty())
    {
        SetRenderMode(m_mesh->IsATrianglesModel() ?
                      RenderMode::Triangles : RenderMode::Quads);
    }
}

AABox MeshRenderer::GetAABBox() const
{
    return m_mesh ? m_mesh->GetAABBox() : AABox::Empty;
}

const Mesh *MeshRenderer::GetMesh()
{
    return m_mesh;
}

String MeshRenderer::GetName() const
{
    return "MeshRenderer";
}

void MeshRenderer::RenderWithoutBindingMaterial() const
{
    ASSERT(m_mesh);

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
    SetMesh( AssetsManager::Load<Mesh>( xmlInfo->GetFilepath("Mesh") ) );
}

void MeshRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    Renderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("MeshRenderer");

    if (!GetDrawWireframe())
    {
        xmlInfo->GetAttribute("LineWidth")->SetProperty(XMLProperty::Hidden);
    }
    xmlInfo->SetFilepath("Mesh", m_mesh ? m_mesh->GetFilepath() : "", "bmesh");
}
