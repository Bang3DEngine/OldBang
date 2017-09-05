#include "Bang/LineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"

LineRenderer::LineRenderer()
{
    m_mesh = new Mesh();
    SetMaterial(MaterialFactory::GetDefaultUnLighted());

    SetRenderPrimitive(GL::Primitives::Lines);
}

LineRenderer::~LineRenderer()
{
    delete m_mesh;
}

void LineRenderer::OnRender()
{
    Renderer::OnRender(); ENSURE(m_mesh->GetVertexCount() > 0);
    GL::Render(m_mesh->GetVAO(), GetRenderPrimitive(), m_mesh->GetVertexCount());
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
    m_mesh->LoadPositions(m_points);
}

const Array<Vector3> &LineRenderer::GetPoints() const { return m_points; }
AABox LineRenderer::GetAABBox() const
{
    if (m_points.IsEmpty()) { return AABox::Empty; }

    Vector3 minp = m_points.Front();
    Vector3 maxp = m_points.Front();
    for (const Vector3 &p : m_points)
    {
       minp = Vector::Min(minp, p);
       maxp = Vector::Max(maxp, p);
    }

    // Add a bit in every dimensions, to avoid flattened Rects
    const float strokeAdd = 0.01f;
    minp -= Vector3(strokeAdd);
    maxp += Vector3(strokeAdd);
    return AABox(minp, maxp);

}

void LineRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    LineRenderer *lr = SCAST<LineRenderer*>(clone);
    lr->SetPoints( GetPoints() );
}

void LineRenderer::ImportXML(const XMLNode &xmlInfo)
{
    Renderer::ImportXML(xmlInfo);
}

void LineRenderer::ExportXML(XMLNode *xmlInfo) const
{
    Renderer::ExportXML(xmlInfo);
}
