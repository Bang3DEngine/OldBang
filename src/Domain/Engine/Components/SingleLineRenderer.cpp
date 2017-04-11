#include "Bang/SingleLineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/XMLNode.h"

SingleLineRenderer::SingleLineRenderer()
{
    m_points.Resize(2);
    SetOrigin(Vector3::Zero);
    SetDestiny(Vector3::One);
    RefreshPoints();

    SetRenderMode(GL::RenderMode::Lines);
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
    RefreshPoints();
}

void SingleLineRenderer::SetDestiny(const Vector3 &d)
{
    m_points[1] = d;
    RefreshPoints();
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

    xmlInfo->SetVector3("Origin", GetOrigin());
    xmlInfo->SetVector3("Destiny", GetDestiny());
}

