#include "Bang/MeshRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/G_VAO.h"
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
    MeshRenderer *mr = SCAST<MeshRenderer*>(clone);
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

void MeshRenderer::OnRender()
{
    Renderer::OnRender();
    ENSURE(p_mesh);
    GL::Render(p_mesh->GetVAO(), GetRenderMode(), p_mesh->GetVertexCount());
}

void MeshRenderer::Read(const XMLNode &xmlInfo)
{
    Renderer::Read(xmlInfo);
    SetMesh( AssetsManager::Load<Mesh>( xmlInfo.Get<Path>("Mesh") ) );
}

void MeshRenderer::Write(XMLNode *xmlInfo) const
{
    Renderer::Write(xmlInfo);
    xmlInfo->Set("Mesh", GetMesh() ? GetMesh()->GetFilepath() : Path::Empty);
}
