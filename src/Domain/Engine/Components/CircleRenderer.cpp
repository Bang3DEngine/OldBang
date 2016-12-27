#include "CircleRenderer.h"

#include "Scene.h"
#include "Camera.h"
#include "XMLNode.h"
#include "Transform.h"
#include "SceneManager.h"

CircleRenderer::CircleRenderer()
{
    this->m_drawLinesMode = RenderMode::LineStrip;
}

CircleRenderer::~CircleRenderer()
{

}

const String CircleRenderer::ToString() const
{
    return GetName();
}
String CircleRenderer::GetName() const
{
    return "CircleRenderer";
}

void CircleRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    CircleRenderer *cr = static_cast<CircleRenderer*>(clone);
    cr->SetRadius(GetRadius());
    cr->SetSegments(GetSegments());
}

ICloneable *CircleRenderer::Clone() const
{
    CircleRenderer *cr = new CircleRenderer();
    CloneInto(cr);
    return cr;
}


void CircleRenderer::GeneratePoints()
{
    m_points.Clear();
    m_points.Resize(m_segments + 1);

    float step = (2.0f * 3.141592f) / (m_segments);
    for (int i = 0;  i < m_segments + 1; ++i)
    {
        m_points[i] = Vector3(glm::cos(step*i), glm::sin(step*i), 0.0f) * m_radius;
    }
}

void CircleRenderer::GetTwoClosestPointsInScreenSpace(
        const Vector2 &sOrigin,
        Vector2 *p0, int *i0,
        Vector2 *p1, int *i1) const
{
    float d0, d1; d0 = d1 = 99999.9f;
    for (int i = 0; i < m_points.Size() - 1; ++i)
    {
        Vector3 objP = m_points[i];
        Matrix4 m; transform->GetLocalToWorldMatrix(&m);
        Vector3 worldP = (m * Vector4(objP,1)).xyz();
        Vector2 sP = SceneManager::GetActiveScene()->GetCamera()->WorldToScreenNDCPoint(worldP);

        float d = Vector2::Distance(sP, sOrigin);
        if (d < d0)
        {
            *p1 = *p0;
            *i1 = *i0;
            d1 = d0;

            *p0 = sP;
            *i0 = i;
            d0 = d;
        }
        else if (d < d1)
        {
            *p1 = sP;
            *i1 = i;
            d1 = d;
        }
    }
}

void CircleRenderer::SetRadius(float radius)
{
    this->m_radius = radius;
    GeneratePoints();
    BindPointsToVAO();
}

void CircleRenderer::SetSegments(int segments)
{
    this->m_segments = segments;
    GeneratePoints();
    BindPointsToVAO();
}

float CircleRenderer::GetRadius() const
{
    return m_radius;
}

int CircleRenderer::GetSegments() const
{
    return m_segments;
}

#ifdef BANG_EDITOR
void CircleRenderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void CircleRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void CircleRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    LineRenderer::ReadXMLInfo(xmlInfo);
    SetRadius(xmlInfo->GetFloat("Radius"));
    SetSegments(xmlInfo->GetFloat("Segments"));
}

void CircleRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    LineRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("CircleRenderer");

    xmlInfo->SetFloat("Radius", GetRadius());
    xmlInfo->SetInt("Segments", GetSegments());
}
