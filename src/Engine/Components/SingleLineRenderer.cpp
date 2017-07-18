#include "Bang/SingleLineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/XMLNode.h"

SingleLineRenderer::SingleLineRenderer()
{
    SetPoints( {Vector3::Zero, Vector3::One} );
    SetRenderMode(GL::RenderMode::Lines);
}

SingleLineRenderer::~SingleLineRenderer()
{
}

void SingleLineRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    SingleLineRenderer *slr = SCAST<SingleLineRenderer*>(clone);
    slr->SetOrigin(GetOrigin());
    slr->SetDestiny(GetDestiny());
}

void SingleLineRenderer::SetOrigin(const Vector3 &o)
{
    SetPoint(0, o);
}

void SingleLineRenderer::SetDestiny(const Vector3 &d)
{
    SetPoint(1, d);
}

Vector3 SingleLineRenderer::GetOrigin() const
{
    const Array<Vector3>& points = GetPoints();
    return points.Front();
}

Vector3 SingleLineRenderer::GetDestiny() const
{
    const Array<Vector3>& points = GetPoints();
    return points.Back();
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

