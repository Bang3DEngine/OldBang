#include "SingleLineRenderer.h"

#include "XMLNode.h"

SingleLineRenderer::SingleLineRenderer()
{
    m_points.Resize(2);
    m_points[0] = Vector3(0.0f);
    m_points[1] = Vector3(1.0f);

    m_drawLinesMode = GL::RenderMode::Lines;
}

SingleLineRenderer::~SingleLineRenderer()
{
}

void SingleLineRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    SingleLineRenderer *slr = Object::SCast<SingleLineRenderer>(clone);
    slr->SetOrigin(GetOrigin());
    slr->SetDestiny(GetDestiny());
}

void SingleLineRenderer::SetOrigin(const Vector3 &o)
{
    m_points[0] = o;
    BindPointsToVAO();
}

void SingleLineRenderer::SetDestiny(const Vector3 &d)
{
    m_points[1] = d;
    BindPointsToVAO();
}

Vector3 SingleLineRenderer::GetOrigin() const
{
    return m_points[0];
}

Vector3 SingleLineRenderer::GetDestiny() const
{
    return m_points[1];
}

void SingleLineRenderer::Read(const XMLNode &xmlInfo)
{
    LineRenderer::Read(xmlInfo);
    SetOrigin(xmlInfo.GetVector3("Origin"));
    SetDestiny(xmlInfo.GetVector3("Destiny"));
}

void SingleLineRenderer::Write(XMLNode *xmlInfo) const
{
    LineRenderer::Write(xmlInfo);
    xmlInfo->SetTagName("SingleLineRenderer");

    xmlInfo->SetVector3("Origin", GetOrigin());
    xmlInfo->SetVector3("Destiny", GetDestiny());
}

