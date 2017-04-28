#include "Bang/CircleRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"

CircleRenderer::CircleRenderer()
{
    SetRenderMode(GL::RenderMode::LineStrip);
}

CircleRenderer::~CircleRenderer()
{
}

void CircleRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    CircleRenderer *cr = Object::SCast<CircleRenderer>(clone);
    cr->SetRadius(GetRadius());
    cr->SetSegments(GetSegments());
}

void CircleRenderer::ReloadPoints()
{
    Array<Vector3> points;
    points.Resize(m_segments);
    const float step = (2.0f * Math::PI) / (m_segments - 1);
    for (int i = 0; i < m_segments; ++i)
    {
        points[i] = Vector3(Math::Cos(step*i),
                            Math::Sin(step*i),
                            0.0f) * m_radius;
    }
    SetPoints(points);
}


void CircleRenderer::SetRadius(float radius)
{
    m_radius = radius;
    ReloadPoints();
}

void CircleRenderer::SetSegments(int segments)
{
    m_segments = segments;
    ReloadPoints();
}

float CircleRenderer::GetRadius() const
{
    return m_radius;
}

int CircleRenderer::GetSegments() const
{
    return m_segments;
}

void CircleRenderer::Read(const XMLNode &xmlInfo)
{
    LineRenderer::Read(xmlInfo);
    SetRadius(xmlInfo.GetFloat("Radius"));
    SetSegments(xmlInfo.GetFloat("Segments"));
}

void CircleRenderer::Write(XMLNode *xmlInfo) const
{
    LineRenderer::Write(xmlInfo);

    xmlInfo->SetFloat("Radius", GetRadius());
    xmlInfo->SetInt("Segments", GetSegments());
}
