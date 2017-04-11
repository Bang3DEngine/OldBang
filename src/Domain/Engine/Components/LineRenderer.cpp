#include "Bang/LineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/Math.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"

LineRenderer::LineRenderer()
{
    m_linesMesh = new Mesh();
    SetRenderMode(GL::RenderMode::Lines);
}

void LineRenderer::RefreshPoints()
{
    m_linesMesh->LoadPositions(m_points);
}

void LineRenderer::RenderWithoutMaterial() const
{
    GL::Render(m_linesMesh->GetVAO(), GetRenderMode(),
               m_linesMesh->GetVertexCount());
}

LineRenderer::~LineRenderer()
{
    if (m_linesMesh) { delete m_linesMesh; }
}

AABox LineRenderer::GetAABBox() const
{
    if (m_points.Empty()) { return AABox::Empty; }

    Vector3 minp = m_points.Front();
    Vector3 maxp = m_points.Front();
    for (const Vector3 &p : m_points)
    {
       minp.x = Math::Min(minp.x, p.x);
       minp.y = Math::Min(minp.y, p.y);
       minp.z = Math::Min(minp.z, p.z);
       maxp.x = Math::Max(maxp.x, p.x);
       maxp.y = Math::Max(maxp.y, p.y);
       maxp.z = Math::Max(maxp.z, p.z);
    }

    // Add a bit in every dimensions, to avoid flattened Rects
    const float strokeAdd = 0.01f;
    minp.x -= strokeAdd; maxp.x += strokeAdd;
    minp.y -= strokeAdd; maxp.y += strokeAdd;
    minp.z -= strokeAdd; maxp.z += strokeAdd;
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
