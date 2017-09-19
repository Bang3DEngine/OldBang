#include "Bang/UIBorderRect.h"

#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/ComponentFactory.h"

USING_NAMESPACE_BANG

UIBorderRect::UIBorderRect()
{
    p_lineRenderer = ComponentFactory::CreateComponent<LineRenderer>();
    p_lineRenderer->SetPoints({});
    p_lineRenderer->UseMaterialCopy();
    p_lineRenderer->SetRenderPrimitive(GL::Primitives::LineStrip);

    // Work in Canvas and NDC space
    p_lineRenderer->SetRenderPass(RenderPass::Canvas);
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::IgnoreBoth);

    SetLineColor(Color::Black);
    SetLineWidth(5.0f);
}

UIBorderRect::~UIBorderRect()
{

}

void UIBorderRect::OnStart()
{
    Component::OnStart();
    p_lineRenderer->OnStart();
}

void UIBorderRect::OnRender(RenderPass rp)
{
    Component::OnRender(rp);
    p_lineRenderer->Renderer::OnRender(rp);
}

void UIBorderRect::SetLineColor(const Color &lineColor)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(lineColor);
}
void UIBorderRect::SetLineWidth(float lineWidth)
{
    p_lineRenderer->SetLineWidth(lineWidth);
}

const Color &UIBorderRect::GetLineColor() const
{
    return p_lineRenderer->GetMaterial()->GetDiffuseColor();
}
float UIBorderRect::GetLineWidth() const
{
    return p_lineRenderer->GetLineWidth();
}

void UIBorderRect::OnSizesCalculated()
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>(); ENSURE(rt);
    Rect boundingRectNDC = rt->GetScreenSpaceRectNDC();
    p_lineRenderer->SetPoints(
           {Vector3(boundingRectNDC.GetMinXMinY(), 0),
            Vector3(boundingRectNDC.GetMinXMaxY(), 0),
            Vector3(boundingRectNDC.GetMaxXMaxY(), 0),
            Vector3(boundingRectNDC.GetMaxXMinY(), 0),
            Vector3(boundingRectNDC.GetMinXMinY(), 0)} );
}

void UIBorderRect::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("LineColor"))
    { SetLineColor( xmlInfo.Get<Color>("LineColor") ); }
}

void UIBorderRect::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("LineColor", GetLineColor());
}
