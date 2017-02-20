#include "SingleLineRenderer.h"

#include "XMLNode.h"

SingleLineRenderer::SingleLineRenderer()
{
    m_points.Resize(2);
    m_points[0] = Vector3(0.0f);
    m_points[1] = Vector3(1.0f);

    m_drawLinesMode = RenderMode::Lines;
}

SingleLineRenderer::~SingleLineRenderer()
{
}

String SingleLineRenderer::GetName() const
{
    return "SingleLineRenderer";
}

void SingleLineRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    SingleLineRenderer *slr = static_cast<SingleLineRenderer*>(clone);
    slr->SetOrigin(GetOrigin());
    slr->SetDestiny(GetDestiny());
}

ICloneable *SingleLineRenderer::Clone() const
{
    SingleLineRenderer *slr = new SingleLineRenderer();
    CloneInto(slr);
    return slr;
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

void SingleLineRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    LineRenderer::ReadXMLInfo(xmlInfo);
    SetOrigin(xmlInfo->GetVector3("Origin"));
    SetDestiny(xmlInfo->GetVector3("Destiny"));
}

void SingleLineRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    LineRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("SingleLineRenderer");

    xmlInfo->SetVector3("Origin", GetOrigin());
    xmlInfo->SetVector3("Destiny", GetDestiny());
}

