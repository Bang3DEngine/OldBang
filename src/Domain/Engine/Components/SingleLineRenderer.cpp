#include "SingleLineRenderer.h"

SingleLineRenderer::SingleLineRenderer()
{
    m_points.resize(2);
    m_points[0] = Vector3(0.0f);
    m_points[1] = Vector3(1.0f);

    this->m_drawLinesMode = RenderMode::Lines;
}

SingleLineRenderer::~SingleLineRenderer()
{

}

const std::string SingleLineRenderer::ToString() const
{
    return "SingleLineRenderer";
}

std::string SingleLineRenderer::GetName() const { return "SingleLineRenderer"; }

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

#ifdef BANG_EDITOR
void SingleLineRenderer::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void SingleLineRenderer::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif


void SingleLineRenderer::ReadXMLInfo(const XMLNode *xmlInfo)
{
    LineRenderer::ReadXMLInfo(xmlInfo);
    SetMaterial( AssetsManager::GetAsset<Material>(xmlInfo->GetString("materialFilepath") ) );
    SetOrigin(xmlInfo->GetVector3("origin"));
    SetDestiny(xmlInfo->GetVector3("destiny"));
}

void SingleLineRenderer::FillXMLInfo(XMLNode *xmlInfo) const
{
    LineRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("SingleLineRenderer");

    xmlInfo->SetAttribute("id", this);
    xmlInfo->SetAttribute("materialFilepath", m_material->GetFilepath());
    xmlInfo->SetAttribute("origin", GetOrigin());
    xmlInfo->SetAttribute("destiny", GetDestiny());
}

