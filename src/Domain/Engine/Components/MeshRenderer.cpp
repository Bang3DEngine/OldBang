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

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = Object::SCast<MeshRenderer>(clone);
    mr->SetMesh(m_mesh);
}

void MeshRenderer::SetMaterial(Material *m)
{
    Renderer::SetMaterial(m);
    Material *mat = GetMaterial();
    if (m_mesh && mat && mat->GetShaderProgram() )
    {
        m_mesh->BindAllVBOsToShaderProgram(*(mat->GetShaderProgram()));
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

void MeshRenderer::RenderWithoutMaterial() const
{
    ASSERT(m_mesh);
    BindCurrentMeshToShaderProgram();
    GL::Render(m_mesh->GetVAO(), m_renderMode, m_mesh->GetVertexCount());
}

void MeshRenderer::Read(const XMLNode &xmlInfo)
{
    Renderer::Read(xmlInfo);
    SetMesh( AssetsManager::Load<Mesh>( xmlInfo.GetFilepath("Mesh") ) );
}

void MeshRenderer::Write(XMLNode *xmlInfo) const
{
    Renderer::Write(xmlInfo);

    if (!GetDrawWireframe())
    {
        xmlInfo->GetAttribute("LineWidth")->SetProperty(XMLProperty::Hidden);
    }
    xmlInfo->SetFilepath("Mesh", m_mesh ? m_mesh->GetFilepath() : "",
                         Mesh::GetFileExtensionStatic());
}

void MeshRenderer::BindCurrentMeshToShaderProgram() const
{
    Material *mat = GetMaterial();
    ASSERT(m_mesh); ASSERT(mat); ASSERT(mat->GetShaderProgram());

    m_mesh->BindAllVBOsToShaderProgram(*(mat->GetShaderProgram()));
}
