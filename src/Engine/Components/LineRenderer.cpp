#include "Bang/LineRenderer.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

LineRenderer::LineRenderer()
{
    p_mesh = Resources::Create<Mesh>();

    SetMaterial(MaterialFactory::GetDefaultUnLighted().Get());

    SetRenderPrimitive(GL::Primitive::Lines);
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::OnRender()
{
    Renderer::OnRender();
    if (p_mesh.Get()->GetVertexCount() <= 0) { return; }
    GL::Render(p_mesh.Get()->GetVAO(), GetRenderPrimitive(),
               p_mesh.Get()->GetVertexCount());
}

void LineRenderer::SetPoint(int i, const Vector3 &point)
{
    Array<Vector3> newPoints = m_points;
    ASSERT(i >= 0 && i <= newPoints.Size());

    if (i == newPoints.Size()) { newPoints.PushBack(point); }
    else { newPoints[i] = point; }

    SetPoints(newPoints);
}

void LineRenderer::SetPoints(const Array<Vector3> &points)
{
    if ((points.IsEmpty() && GetPoints().IsEmpty()) ||
        points.Size() != GetPoints().Size() ||
        !Containers::Equal(points.Begin(), points.End(), GetPoints().Begin()))
    {
        m_points = points;
        p_mesh.Get()->LoadPositions(GetPoints());
        PropagateRendererChanged();
    }
}

const Array<Vector3> &LineRenderer::GetPoints() const { return m_points; }
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

void LineRenderer::CloneInto(ICloneable *clone) const
{
    Renderer::CloneInto(clone);
    LineRenderer *lr = Cast<LineRenderer*>(clone);
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
