#include "Bang/MeshRenderer.h"

#include "Bang/GL.h"
#include "Bang/VAO.h"
#include "Bang/Mesh.h"
#include "Bang/ModelIO.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"

USING_NAMESPACE_BANG

MeshRenderer::MeshRenderer()
{
    SetRenderPrimitive( GL::Primitive::Triangles );
    SetMesh( MeshFactory::GetCube().Get() );
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::SetMesh(Mesh *m)
{
    if (GetSharedMesh() != m)
    {
        p_sharedMesh.Set(m);
        p_mesh.Set(nullptr);
    }
}
Mesh* MeshRenderer::GetMesh() const
{
    if (!p_mesh)
    {
        if (GetSharedMesh())
        {
            p_mesh = Resources::Clone<Mesh>(GetSharedMesh());
        }
    }
    return p_mesh.Get();
}
Mesh *MeshRenderer::GetSharedMesh() const { return p_sharedMesh.Get(); }
AABox MeshRenderer::GetAABBox() const
{
    return GetUserMesh() ? GetUserMesh()->GetAABBox() : AABox::Empty;
}

void MeshRenderer::OnRender()
{
    Renderer::OnRender();
    if (!GetUserMesh()) { return; }
    GL::Render(GetUserMesh()->GetVAO(), GetRenderPrimitive(),
               GetUserMesh()->GetVertexCount());
}

Mesh *MeshRenderer::GetUserMesh() const
{
    if (p_mesh) { return GetMesh(); }
    return GetSharedMesh();
}

void MeshRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    MeshRenderer *mr = Cast<MeshRenderer*>(clone);
    mr->SetMesh( GetSharedMesh() );
}

void MeshRenderer::ImportXML(const XMLNode &xmlInfo)
{
    Renderer::ImportXML(xmlInfo);
    if (xmlInfo.Contains("Mesh"))
    {
        RH<Mesh> mesh = Resources::Load<Mesh>(xmlInfo.Get<GUID>("Mesh"));
        SetMesh( mesh.Get() );
    }
}

void MeshRenderer::ExportXML(XMLNode *xmlInfo) const
{
    Renderer::ExportXML(xmlInfo);
    xmlInfo->Set("Mesh", GetSharedMesh() ? GetSharedMesh()->GetGUID() : GUID::Empty());
}
