#include "Bang/LineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Material.h"
#include "Bang/AssetsManager.h"
#include "Bang/ComponentFactory.h"

LineRenderer::LineRenderer()
{
    p_meshRenderer = ComponentFactory::CreateComponent<MeshRenderer>();
    p_meshRenderer->SetMesh( new Mesh() ); // TODO: MEMLEAK
    p_meshRenderer->SetMaterial(
         AssetsManager::Load<Material>(EPATH("Materials/G_DefaultNoSP.bmat")));
    GetMaterial()->SetReceivesLighting(false);
    AddDelegate(p_meshRenderer);

    SetRenderPrimitive(GL::RenderPrimitive::Lines);
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::OnRender()
{
    Renderer::OnRender();
    Mesh *mesh = p_meshRenderer->GetMesh();
    ENSURE(mesh && mesh->GetVAO() && mesh->GetVertexCount() > 0);
    GL::Render(mesh->GetVAO(), GetRenderPrimitive(), mesh->GetVertexCount());
}

void LineRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    LineRenderer *lr = SCAST<LineRenderer*>(clone);
    lr->SetPoints( GetPoints() );
}

const Array<Vector3> &LineRenderer::GetPoints() const
{
    return m_points;
}

void LineRenderer::SetPoint(int i, const Vector3 &point)
{
    if (i >= m_points.Size())
    {
        if (i == m_points.Size()) { m_points.PushBack(point); }
        else { return; }
    }
    else
    {
        m_points[i] = point;
    }
    SetPoints(m_points);
}

void LineRenderer::SetPoints(const Array<Vector3> &points)
{
    m_points = points;
    p_meshRenderer->GetMesh()->LoadPositions(m_points);
}

AABox LineRenderer::GetAABBox() const
{
    if (m_points.IsEmpty()) { return AABox::Empty; }

    Vector3 minp = m_points.Front();
    Vector3 maxp = m_points.Front();
    for (const Vector3 &p : m_points)
    {
       minp = Vector3::Min(minp, p);
       maxp = Vector3::Max(maxp, p);
    }

    // Add a bit in every dimensions, to avoid flattened Rects
    const float strokeAdd = 0.01f;
    minp -= Vector3(strokeAdd);
    maxp += Vector3(strokeAdd);
    return AABox(minp, maxp);

}

void LineRenderer::Read(const XMLNode &xmlInfo)
{
    Renderer::Read(xmlInfo);
}

void LineRenderer::Write(XMLNode *xmlInfo) const
{
    Renderer::Write(xmlInfo);
}
