#include "CircleRenderer.h"

#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "XMLNode.h"
#include "Transform.h"
#include "SceneManager.h"

CircleRenderer::CircleRenderer()
{
    m_drawLinesMode = GL::RenderMode::LineStrip;
}

CircleRenderer::~CircleRenderer()
{
}

String CircleRenderer::GetName() const
{
    return "CircleRenderer";
}

void CircleRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    CircleRenderer *cr = Object::SCast<CircleRenderer>(clone);
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
    m_points.Resize(m_segments);
    const float step = (2.0f * Math::PI) / m_segments;
    for (int i = 0; i < m_segments; ++i)
    {
        m_points[i] =
              Vector3(Math::Cos(step*i), Math::Sin(step*i), 0.0f) * m_radius;
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
    Write(xmlInfo);
}

void CircleRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    Read(xmlInfo);
}
#endif

void CircleRenderer::Read(const XMLNode *xmlInfo)
{
    LineRenderer::Read(xmlInfo);
    SetRadius(xmlInfo->GetFloat("Radius"));
    SetSegments(xmlInfo->GetFloat("Segments"));
}

void CircleRenderer::Write(XMLNode *xmlInfo) const
{
    LineRenderer::Write(xmlInfo);
    xmlInfo->SetTagName("CircleRenderer");

    xmlInfo->SetFloat("Radius", GetRadius());
    xmlInfo->SetInt("Segments", GetSegments());
}
