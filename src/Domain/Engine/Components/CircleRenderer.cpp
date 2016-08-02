#include "CircleRenderer.h"

CircleRenderer::CircleRenderer()
{
    this->m_drawLinesMode = RenderMode::LineStrip;
}

CircleRenderer::~CircleRenderer()
{

}

const std::string CircleRenderer::ToString() const
{
    return "CircleRenderer";
}

std::string CircleRenderer::GetName() const { return "CircleRenderer"; }

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
    m_points.clear();
    m_points.resize(m_segments + 1);

    float step = (2.0f * 3.141592f) / (m_segments);
    for (int i = 0;  i < m_segments + 1; ++i)
    {
        m_points[i] = Vector3(glm::cos(step*i), glm::sin(step*i), 0.0f) * m_radius;
    }
}

float CircleRenderer::GetDistanceInScreenSpace(const glm::vec2 &sOrigin,
                                              int pointIndex,
                                              const Matrix4 &modelViewProjMatrix) const
{
    Vector3 objP = m_points[pointIndex];
    Vector4 sP_4 = modelViewProjMatrix * Vector4(objP, 1.0f);
    glm::vec2 sP = sP_4.xy() / sP_4.w;
    sP = sP * 0.5f + 0.5f;

    return glm::distance(sOrigin, sP);
}

void CircleRenderer::GetTwoClosestPointsInScreenSpace(
        const glm::vec2 &sOrigin,
        const Matrix4 &modelViewProjMatrix,
        glm::vec2 *p0, int *i0,
        glm::vec2 *p1, int *i1) const
{
    /**
     * @brief We calculate this step in order to avoid sampling
     * points that are too close to each other, since this will
     * make the rotation axis losing precision in screen terms...
     */

    //const int pointsToSample = 16;
    //int step = points.size() / pointsToSample; // Sample pointsToSample circle points evenly spaced
    //step = glm::max(step, 1); // In case segments < pointsToSample
    int step = 1;

    float d0, d1; d0 = d1 = 99999.9f;
    for (int i = 0; i < m_points.size() - 1; i += step) // -1 because the last point is repeated
    {
        // TODO: not working
        Vector3 objP = m_points[i];
        Vector4 sP_4 = modelViewProjMatrix * Vector4(objP, 1.0f);
        glm::vec2 sP = Canvas::GetCurrentScene()->GetCamera()->WorldToScreenNDCPoint(Vector3(sP_4.xyz()));

        float d = glm::distance(sP, sOrigin);
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
    SetRadius(xmlInfo->GetFloat("radius"));
    SetSegments(xmlInfo->GetFloat("segments"));
}

void CircleRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    LineRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("CircleRenderer");

    xmlInfo->SetFilepath("materialFilepath", m_material->GetFilepath());
    xmlInfo->SetFloat("radius", GetRadius());
    xmlInfo->SetInt("segments", GetSegments());
}
