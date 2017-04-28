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
    SetRenderMode( GL::RenderMode::Triangles );
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = Object::SCast<MeshRenderer>(clone);
    mr->SetMesh( GetMesh() );
}

void MeshRenderer::SetMesh(Mesh *m)
{
    p_mesh = m;
}

AABox MeshRenderer::GetAABBox() const
{
    return p_mesh ? p_mesh->GetAABBox() : AABox::Empty;
}

Mesh *MeshRenderer::GetMesh() const
{
    return p_mesh;
}

void MeshRenderer::RenderWithoutMaterial() const
{
    ENSURE(p_mesh);
    GL::Render(p_mesh->GetVAO(), GetRenderMode(), p_mesh->GetVertexCount());
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
    xmlInfo->SetFilepath("Mesh", GetMesh() ? GetMesh()->GetFilepath() : "",
                         Mesh::GetFileExtensionStatic());
}
