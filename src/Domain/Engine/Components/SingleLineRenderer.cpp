#include "SingleLineRenderer.h"

SingleLineRenderer::SingleLineRenderer()
{
    #ifdef BANG_EDITOR
        m_inspectorInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Origin", 3));
        m_inspectorInfo.AddSlotInfo(
            new InspectorVFloatSWInfo("Destiny", 3));
    #endif

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
InspectorWidgetInfo* SingleLineRenderer::OnInspectorInfoNeeded()
{
    LineRenderer::OnInspectorInfoNeeded();

    m_inspectorInfo.GetSlotInfo("Origin")->SetVector3(m_points[0]);
    m_inspectorInfo.GetSlotInfo("Destiny")->SetVector3(m_points[1]);

    return &m_inspectorInfo;
}

void SingleLineRenderer::OnInspectorInfoChanged(InspectorWidgetInfo *info)
{
    LineRenderer::OnInspectorInfoChanged(info);

    m_points[0] = info->GetVector3("Origin");
    m_points[1] = info->GetVector3("Destiny");
}
#endif


void SingleLineRenderer::ReadXMLNode(const XMLNode *xmlNode)
{
    LineRenderer::ReadXMLNode(xmlNode);
    SetMaterial( AssetsManager::GetAsset<Material>(xmlNode->GetString("materialFilepath") ) );
    SetOrigin(xmlNode->GetVector3("origin"));
    SetDestiny(xmlNode->GetVector3("destiny"));
}

void SingleLineRenderer::GetXMLNode(XMLNode *xmlNode) const
{
    LineRenderer::GetXMLNode(xmlNode);
    xmlNode->SetTagName("SingleLineRenderer");

    xmlNode->SetAttribute("id", this);
    xmlNode->SetAttribute("materialFilepath", m_material->GetFilepath());
    xmlNode->SetAttribute("origin", GetOrigin());
    xmlNode->SetAttribute("destiny", GetDestiny());
}

