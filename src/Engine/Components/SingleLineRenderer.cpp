#include "Bang/SingleLineRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/XMLNode.h"

USING_NAMESPACE_BANG

SingleLineRenderer::SingleLineRenderer()
{
    SetPoints( {Vector3::Zero, Vector3::One} );
    SetRenderPrimitive(GL_Primitive::Lines);
}

SingleLineRenderer::~SingleLineRenderer()
{
}

void SingleLineRenderer::SetOrigin(const Vector3 &o) { SetPoint(0, o); }
void SingleLineRenderer::SetDestiny(const Vector3 &d) { SetPoint(1, d); }

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

void SingleLineRenderer::CloneInto(ICloneable *clone) const
{
    LineRenderer::CloneInto(clone);
    SingleLineRenderer *slr = Cast<SingleLineRenderer*>(clone);
    slr->SetOrigin(GetOrigin());
    slr->SetDestiny(GetDestiny());
}

void SingleLineRenderer::ImportXML(const XMLNode &xmlInfo)
{
    LineRenderer::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Origin"))
    { SetOrigin(xmlInfo.Get<Vector3>("Origin")); }

    if (xmlInfo.Contains("Destiny"))
    { SetDestiny(xmlInfo.Get<Vector3>("Destiny")); }
}

void SingleLineRenderer::ExportXML(XMLNode *xmlInfo) const
{
    LineRenderer::ExportXML(xmlInfo);

    xmlInfo->Set("Origin", GetOrigin());
    xmlInfo->Set("Destiny", GetDestiny());
}
