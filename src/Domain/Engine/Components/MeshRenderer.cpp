#include "MeshRenderer.h"

#include "VAO.h"
#include "Mesh.h"
#include "Debug.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"
#include "FileReader.h"
#include "AssetsManager.h"

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = Object::SCast<MeshRenderer>(clone);
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
    Renderer::SetMaterial(m);
    if (m_mesh  && m_material  &&
        m_material->GetShaderProgram() )
    {
        m_mesh->BindAllVBOsToShaderProgram(*(m_material->GetShaderProgram()));
    }
}

void MeshRenderer::SetMesh(Mesh *m)
{
    m_mesh = m; ASSERT(m_mesh);

    if (!m_mesh->GetFilepath().Empty())
    {
        SetRenderMode(m_mesh->IsATrianglesModel() ?
                      GL::RenderMode::Triangles : GL::RenderMode::Quads);
    }

    BindCurrentMeshToShaderProgram();
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

void MeshRenderer::RenderWithoutMaterial() const
{
    ASSERT(m_mesh);

    BindCurrentMeshToShaderProgram();
    GL::Render(m_mesh->GetVAO(), m_renderMode, m_mesh->GetVertexCount());
}

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
    xmlInfo->SetFilepath("Mesh", m_mesh ? m_mesh->GetFilepath() : "",
                         Mesh::GetFileExtensionStatic());
}

void MeshRenderer::BindCurrentMeshToShaderProgram() const
{
    ASSERT(m_mesh); ASSERT(m_material); ASSERT(m_material->GetShaderProgram());

    m_mesh->BindAllVBOsToShaderProgram(*(m_material->GetShaderProgram()));
}
