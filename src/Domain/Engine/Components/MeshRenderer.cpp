#include "Bang/MeshRenderer.h"

#include "Bang/VAO.h"
#include "Bang/Mesh.h"
#include "Bang/Debug.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/FileReader.h"
#include "Bang/AssetsManager.h"

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

void MeshRenderer::SetMesh(Mesh *m)
{
    m_mesh = m; ENSURE(m_mesh);

    if (!m_mesh->GetFilepath().Empty())
    {
        SetRenderMode(m_mesh->IsATrianglesModel() ?
                      GL::RenderMode::Triangles : GL::RenderMode::Quads);
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

void MeshRenderer::RenderWithoutMaterial() const
{
    ENSURE(m_mesh);
    GL::Render(m_mesh->GetVAO(), m_renderMode, m_mesh->GetVertexCount());
}

void MeshRenderer::Read(const XMLNode &xmlInfo)
{
    Renderer::Read(xmlInfo);
    if (!xmlInfo.GetAttribute("Mesh")->HasProperty(XMLProperty::Hidden))
    {
        SetMesh( AssetsManager::Load<Mesh>( xmlInfo.GetFilepath("Mesh") ) );
    }
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
