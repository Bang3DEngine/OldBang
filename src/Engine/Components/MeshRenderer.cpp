#include "Bang/MeshRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/VAO.h"
#include "Bang/MeshIO.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

MeshRenderer::MeshRenderer()
{
    SetRenderPrimitive( GL::Primitives::Triangles );
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMesh(Mesh *m) { p_mesh.Set(m); }
Mesh* MeshRenderer::GetMesh() const { return p_mesh.Get(); }
AABox MeshRenderer::GetAABBox() const
{
    return p_mesh ? p_mesh.Get()->GetAABBox() : AABox::Empty;
}

void MeshRenderer::OnRender()
{
    Renderer::OnRender(); ENSURE(p_mesh);
    GL::Render(p_mesh.Get()->GetVAO(), GetRenderPrimitive(),
               p_mesh.Get()->GetVertexCount());
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = Cast<MeshRenderer*>(clone);
    mr->SetMesh( GetMesh() );
}

void MeshRenderer::ImportXML(const XMLNode &xmlInfo)
{
    Renderer::ImportXML(xmlInfo);
    if (xmlInfo.Contains("Mesh"))
    {
        RH<Mesh> mesh = Resources::Load<Mesh>(xmlInfo.Get<GUID>("Mesh"));
        SetMesh(mesh.Get());
    }
}

void MeshRenderer::ExportXML(XMLNode *xmlInfo) const
{
    Renderer::ExportXML(xmlInfo);
    xmlInfo->Set("Mesh", GetMesh() ? GetMesh()->GetGUID() : GUID::Empty());
}
